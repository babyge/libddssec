#
# DDS Security library
# Copyright (c) 2018, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

cmake_minimum_required(VERSION 3.5)
project("libddssec" VERSION 0.1.0 LANGUAGES C)
set(${PROJECT_NAME}_DESCRIPTION_SUMMARY "DDS Security library")

message(STATUS "${${PROJECT_NAME}_DESCRIPTION_SUMMARY} version ${PROJECT_VERSION}")

# Toolchain options
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(COMPILE_OPTIONS
        -Wall
        -Wextra
        -Werror
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -pedantic
        -pedantic-errors
)

#
# Backend selection
#
set(BACKEND_OPTIONS "openssl" "optee")
set(BACKEND_DEFAULT "openssl")
set(BACKEND ${BACKEND_DEFAULT} CACHE STRING "Select backend (${BACKEND_OPTIONS}, default:${BACKEND_DEFAULT})")
set_property(CACHE BACKEND PROPERTY STRINGS ${BACKEND_OPTIONS})

if(NOT ${BACKEND} IN_LIST BACKEND_OPTIONS)
    message(FATAL_ERROR "Invalid BACKEND. Valid options are: ${BACKEND_OPTIONS}")
endif()

message(STATUS "Backend selected: ${BACKEND}")

#
# Generate version header
#
set(version_header ${CMAKE_CURRENT_BINARY_DIR}/include/dsec_version.h)
message(STATUS "Generating version header in ${version_header}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/dsec_version.h.in ${version_header})

#
# Common settings for all backends
#
set(INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/include
)
set(PROJECT_SOURCE "")

#
# Test support
#

#
# Function to declare a test suite:
#
# NAME <name> - Test suite name used for:
#   * Name of the executable
#   * Name of the target in the build system (e.g. make <name>)
#   * Name used in the the logs when running 'make build_and_test'
#   Note: NAME must not contain special characters or spaces. Name must not
#       conflict other project targets (e.g. clean, all, libddssec, etc).
#
# SOURCE <source1> [source2 ...] - Source code to be included in the build
#   of the test. At least one source code must be passed in.
#
# WRAP [function_name1 ...] - Functions being "wrapped". A wrapped function
#   requires the test case to provide its implementation. See the '--wrap'
#   argument in # 'GNU ld' for more details. This argument can be empty.
#
function(dsec_add_test)
    set(options)
    set(one_value_keywords NAME)
    set(multi_value_keywords SOURCE WRAP)
    cmake_parse_arguments(ARG_TEST "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    # Test must have a NAME
    if(NOT DEFINED ARG_TEST_NAME)
        message(FATAL_ERROR "Test suite must have a NAME.")
    endif()

    # Test must have at least one source code
    list(LENGTH ARG_TEST_SOURCE source_count)
    if( ${source_count} EQUAL 0)
        message(FATAL_ERROR "Test suite must have at least one source code.")
    endif()

    # Look for unknown arguments
    if(DEFINED ARG_TEST_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments used with dsec_add_test(): '${ARG_TEST_UNPARSED_ARGUMENTS}'")
    endif()

    add_executable(${ARG_TEST_NAME} ${TEST_SOURCES} ${ARG_TEST_SOURCE})
    target_compile_options(${ARG_TEST_NAME} PRIVATE ${COMPILE_OPTIONS})
    target_include_directories(${ARG_TEST_NAME} PUBLIC ${TEST_INCLUDES})
    add_test(${ARG_TEST_NAME} ${ARG_TEST_NAME})
    add_dependencies(build_and_test ${ARG_TEST_NAME})
endfunction()

enable_testing()
add_custom_target(build_and_test ${CMAKE_CTEST_COMMAND} -V)
set(TEST_SOURCES dsec_test.c)
set(TEST_INCLUDES ${INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_subdirectory(tests)

#
# Documentation
#
add_subdirectory(doc)

#
# Backend specific settings
#
if(${BACKEND} STREQUAL "openssl")
    find_package(OpenSSL REQUIRED)
    list(APPEND INCLUDES ${OPENSSL_INCLUDE_DIR})
    add_definitions(-DHAVE_OPENSSL=1)
else()
    message(FATAL_ERROR "${BACKEND} is currently not supported")
endif()

#
# Library settings
#
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCE})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})