/*
 * DDS Security library
 * Copyright (c) 2018-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSEC_IH_CA_H
#define DSEC_IH_CA_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \addtogroup GroupIdentityHandle
 *
 * Function for managing Certificate Authority inside an Identity Handle.
 * \{
 */

#include <dsec_ca.h>
#include <stdint.h>

/*!
 * \brief Maximum size of a filename to describe a path to a Certificate
 *     Authority file.
 */
#define DSEC_IH_CA_MAX_FILENAME (2048UL)

/*
 * Extra care is taken here to make sure the maximum size of the filename
 * cannot exceed UINT32_MAX. This is because OPTEE-OS parameters are uint32_t.
 */
#if (DSEC_IH_CA_MAX_FILENAME > UINT32_MAX)
#error "DSEC_IH_CA_MAX_FILENAME cannot be more than UINT32_MAX"
#endif

/*!
 * \brief Load a Certificate Authority from file name for an Identity Handle.
 *
 * \details Calls the Trusted Application to load a Certificate Authority for
 * a specific Identity Handle ID.
 *
 * \param instance Initialized instance to access the Trusted Application.
 * \param ih_id Handle ID of the Identity Handle.
 * \param filename Byte array NULL terminated containing the file name.
 *
 * \retval ::DSEC_SUCCESS Certificate Authority has been loaded.
 * \return TEE_Result from the function DSEC_TA_CMD_IH_CA_LOAD invoked in the TA
 *     converted to a DSEC_E_
 */
int32_t dsec_ih_ca_load(const struct dsec_instance* instance,
                        int32_t ih_id,
                        const char* filename);

/*!
 * \brief Unload a Certificate Authority of a Identity Handle.
 *
 * \details Calls the Trusted Application to unload a Certificate Authority for
 * a specific Identity Handle ID.
 *
 * \param instance Initialized instance to access the Trusted Application.
 * \param ih_id Handle ID of the Identity Handle.
 *
 * \retval ::DSEC_SUCCESS Certificate Authority has been unloaded.
 * \return TEE_Result from the function DSEC_TA_CMD_IH_CA_UNLOAD invoked in the
 *     TA converted to a DSEC_E_
 */
int32_t dsec_ih_ca_unload(const struct dsec_instance* instance, int32_t ih_id);

/*!
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DSEC_IH_CA_H */