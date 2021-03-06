/*
 * DDS Security library
 * Copyright (c) 2018-2020, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TEST_MANAGE_OBJECT_CA_H
#define TEST_MANAGE_OBJECT_CA_H

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

#include <dsec_ca.h>
#include <stdint.h>
#include <tee_client_api.h>

/* Invoke dsec_ta_load_storage in the TA */
TEEC_Result load_object_storage(const char* name,
                                struct dsec_instance* instance);

/* Invoke dsec_ta_load_builtin in the TA */
TEEC_Result load_object_builtin(const char* name,
                                size_t name_length,
                                struct dsec_instance* instance);

/* Invoke dsec_ta_unload_object_memory in the TA */
TEEC_Result unload_object(struct dsec_instance* instance);

/* Takes data from the Normal World and stores it in the TA's secure storage at
 * a given object ID name.
 */
TEEC_Result create_persistent_object(const uint8_t* buffer,
                                     size_t size,
                                     const char* name,
                                     size_t name_length,
                                     struct dsec_instance* instance);

/* Takes an object ID name and deletes the corresponding file in the
 * TA's secure storage.
 */
TEEC_Result delete_persistent_object(const char* name,
                                     size_t name_length,
                                     struct dsec_instance* instance);

#ifdef __cplusplus
}
#endif /*__cplusplus */

#endif /* TEST_MANAGE_OBJECT_CA_H */
