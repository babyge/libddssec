#
# DDS Security library
# Copyright (c) 2018-2020, Arm Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

if(${PROJECT_NAME} STREQUAL "Project")
    message(FATAL_ERROR "Error: Do not use trusted application's CMakeLists.txt\
        directly. Use the top-level CMakeLists.txt instead.")
endif()

#
# Builtin assets
#
add_subdirectory(builtins)

include(${DSEC_CMAKE_MODULE_PATH}/util_function.cmake)

set(LIBDDSSEC_TA_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(LIBDDSSEC_TA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")

macro(dsec_build_ta)

    set(OPTIONS BUILD_TEST_TA)
    set(ONE_VALUE_KEYWORDS TARGET_NAME OUTPUT_DIR TA_UUID_STRING)
    set(MULTI_VALUE_KEYWORDS FILE_PATH)
    cmake_parse_arguments(ARGS
        "${OPTIONS}"
        "${ONE_VALUE_KEYWORDS}"
        "${MULTI_VALUE_KEYWORDS}"
        ${ARGN}
    )

    if(DEFINED ARGS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments: ${ARGS_UNPARSED_ARGUMENTS}")
    endif()

    if(ARGS_BUILD_TEST_TA)
        set(BUILD_TEST 1)
    else()
        set(BUILD_TEST 0)
    endif()

    if(NOT DEFINED ARGS_TARGET_NAME OR
       NOT DEFINED ARGS_TA_UUID_STRING)

        message(FATAL_ERROR "TARGET_NAME or TA_UUID undefined")
    endif()

    dsec_get_uuid_structure(${ARGS_TA_UUID_STRING} DSEC_TA_UUID_STRUCTURE)

    add_custom_target(
        ${ARGS_TARGET_NAME}

        COMMAND make O=${CMAKE_CURRENT_BINARY_DIR}/build
            PROJECT_VERSION=${PROJECT_VERSION}
            PROJECT_DESCRIPTION_SUMMARY=${${PROJECT_NAME}_DESCRIPTION_SUMMARY}
            CFG_TEE_TA_LOG_LEVEL=0
            DSEC_TEST=${BUILD_TEST}
            DSEC_TA_UUID=${DSEC_TA_UUID_STRUCTURE}
            DSEC_TA_UUID_STRING=${ARGS_TA_UUID_STRING}

        WORKING_DIRECTORY ${LIBDDSSEC_TA_SOURCE_DIR}/src
        COMMENT "Building trusted application target: ${ARGS_TARGET_NAME}"

        VERBATIM)


    # Optionally copy the trusted application binary to an output directory
    if(ARGS_OUTPUT_DIR)
        add_custom_command(
            TARGET
                ${ARGS_TARGET_NAME}
            POST_BUILD
            COMMENT
                "Copying trusted application to ${ARGS_OUTPUT_DIR}"
            COMMAND
                ${CMAKE_COMMAND} -E make_directory ${ARGS_OUTPUT_DIR}
            COMMAND
                ${CMAKE_COMMAND} -E copy
                  ${CMAKE_CURRENT_BINARY_DIR}/build/${ARGS_TA_UUID_STRING}.ta
                  ${ARGS_OUTPUT_DIR}
        )
    endif()

    dsec_embed_asset_ta_files(
        ${ARGS_TARGET_NAME}
        ${ARGS_TARGET_NAME}-builtins
        FILE_PATH
            ${ARGS_FILE_PATH}
    )

endmacro()

dsec_build_ta(
    TARGET_NAME
        ta
    OUTPUT_DIR
        ${LIBDDSSEC_TA_BINARY_DIR}
    TA_UUID_STRING
        ${DSEC_TA_UUID_STRING}
    FILE_PATH
        ""
)
