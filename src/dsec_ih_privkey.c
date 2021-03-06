/*
 * DDS Security library
 * Copyright (c) 2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <dsec_ih_privkey.h>
#include <dsec_errno.h>
#include <dsec_print.h>
#include <dsec_ta.h>
#include <string.h>

int32_t dsec_ih_privkey_load(const struct dsec_instance* instance,
                             int32_t ih_id,
                             const char* filename,
                             const char* password,
                             uint32_t password_size)
{

    TEEC_Result teec_result = 0;
    int32_t result = 0;
    uint32_t return_origin = 0;
    TEEC_Operation operation = {0};

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_NONE);

    operation.params[0].value.a = (uint32_t)ih_id;

    operation.params[1].tmpref.buffer = (void*)filename;
    operation.params[1].tmpref.size =
        (uint32_t)(strnlen(filename, DSEC_IH_PRIVKEY_MAX_FILENAME) + 1);

    operation.params[2].tmpref.buffer = (void*)password;
    operation.params[2].tmpref.size = password_size;

    teec_result = dsec_ca_invoke(instance,
                                 DSEC_TA_CMD_IH_PRIVKEY_LOAD,
                                 &operation,
                                 &return_origin);

    result = dsec_ca_convert_teec_result(teec_result);
    if (result != DSEC_SUCCESS) {
        (void)dsec_print("An error occurred: TEEC_Result=0x%x, DSEC_E=0x%x\n",
                         teec_result,
                         result);
    }

    return result;
}

int32_t dsec_ih_privkey_unload(const struct dsec_instance* instance,
                               int32_t ih_id)
{
    TEEC_Result teec_result = 0;
    int32_t result = 0;
    uint32_t return_origin = 0;
    TEEC_Operation operation = {0};

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
                                            TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_NONE);

    operation.params[0].value.a = (uint32_t)ih_id;

    teec_result = dsec_ca_invoke(instance,
                                 DSEC_TA_CMD_IH_PRIVKEY_UNLOAD,
                                 &operation,
                                 &return_origin);

    result = dsec_ca_convert_teec_result(teec_result);
    if (result != DSEC_SUCCESS) {
        (void)dsec_print("An error occurred: TEEC_Result=0x%x, DSEC_E=0x%x\n",
                         teec_result,
                         result);
    }

    return result;
}

int32_t dsec_ih_privkey_sign(void* signature,
                             uint32_t* signature_size,
                             const struct dsec_instance* instance,
                             int32_t lih_id,
                             const void* input_buffer,
                             uint32_t input_size)
{
    TEEC_Result teec_result = 0;
    int32_t result = 0;
    uint32_t return_origin = 0;
    TEEC_Operation operation = {0};

    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_NONE);

    operation.params[0].tmpref.buffer = (void*)signature;
    operation.params[0].tmpref.size = *signature_size;

    operation.params[1].value.a = (uint32_t)lih_id;

    operation.params[2].tmpref.buffer = (void*)input_buffer;
    operation.params[2].tmpref.size = input_size;

    teec_result = dsec_ca_invoke(instance,
                                 DSEC_TA_CMD_IH_PRIVKEY_SIGN,
                                 &operation,
                                 &return_origin);

    if (teec_result == TEEC_SUCCESS) {
        result = DSEC_SUCCESS;
        *signature_size = operation.params[0].tmpref.size;
    } else {
        result = dsec_ca_convert_teec_result(teec_result);
        *signature_size = 0;
        (void)dsec_print("An error occurred: 0x%x.\n", result);
    }

    return result;
}
