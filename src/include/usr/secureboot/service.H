/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/include/usr/secureboot/service.H $                        */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2013,2016                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#ifndef __SECUREBOOT_SERVICE_H
#define __SECUREBOOT_SERVICE_H

#include <errl/errlentry.H>

typedef uint8_t SHA512_t[64];

/* From sha512.h:  */
#define SHA512_DIGEST_LENGTH 64
typedef uint8_t __attribute__((aligned(8))) sha2_hash_t[ \
                SHA512_DIGEST_LENGTH / sizeof(uint8_t) ];


namespace SECUREBOOT
{

    /******************************************/
    /*  Constants                             */
    /******************************************/

    // Processor Security Switch Register Details
    const uint64_t PROC_SECURITY_SWITCH_REGISTER = 0x00010005;
    const uint64_t
          PROC_SECURITY_SWITCH_JUMPER_STATE_MASK = 0x8000000000000000ull;
    const uint64_t
          PROC_SECURITY_SWITCH_TRUSTED_BOOT_MASK = 0x4000000000000000ull;
    const uint64_t
          PROC_SECURITY_SWITCH_PROTECT_I2C_SEEPROM_MASK = 0x0100000000000000ull;


    /******************************************/
    /*  Functions                             */
    /******************************************/

    /** @brief Perform initialization of Secureboot for the Base image.
     *
     *  - Copy secure header from original location.
     *  - Perform blind-purge of bottom-half of cache.
     *  - Add bottom-half of cache to available memory.
     */
    void* initializeBase(void* unused);

    /**
     * @brief Initialize Secure Rom by loading it into memory and
     *        retrieving Hash Keys
     *
     * @return errlHndl_t  NULL on success
     */
    errlHndl_t initializeSecureROM(void);


    /** @brief Determines if Secureboot is enabled.
     */
    bool enabled();

    /** @brief Returns the state of the secure jumper as reported by the master
     *      processor.
     *
     *  @par Detailed Description:
     *      Returns the state of the secure jumper as reported by the master
     *      processor.  This should NOT be used to determine whether security is
     *      enabled, because several conditions are aggregated together to
     *      determine that.  To query whether security is actually enabled or
     *      not, call the enabled() API.  This is a limited-use API intended to
     *      be called by trusted boot code to determine whether a system shipped
     *      with a secure jumper applied or removed, in order to decide
     *      whether to enforce the "TPM Required" policy or not.
     *
     *  @return Boolean indicating acting master processor's secure jumper state
     *  @retval true Jumper is configured to request HW security.  This does not
     *      necessarily imply security is enabled, because an open SBE can
     *      override the HW policy.  Use the enabled() API to determine whether
     *      security is actually enabled.
     *  @retval false Jumper is configured to disble HW security.
     */
    bool getJumperState();

    /**
     * @brief Verify Signed Container
     *
     * @param[in] i_container  Void pointer to effective address of container
     * @param[in] i_size       Size of container in bytes
     *
     * @return errlHndl_t  NULL on success
     */
    errlHndl_t verifyContainer(void * i_container, size_t i_size);

    /**
     * @brief Hash Signed Blob
     *
     * @param[in]  i_blob    Void pointer to effective address of blob
     * @param[in]  i_size    Size of blob in bytes
     * @param[out] o_hash    SHA512 hash
     *
     * @return errlHndl_t  NULL on success
     */
    errlHndl_t hashBlob(void * i_blob, size_t i_size, SHA512_t o_buf);

    /**
     * @brief Retrieve the internal hardware hash key from secure ROM
     *        object.
     * @param[out] o_hash  Reference to the sha2_hash_t array to copy the
     *                     hash to.
     */
     void getHwHashKeys(sha2_hash_t o_hash);

    /*
     * @brief Hash the concatenation of two Blobs
     *
     *  Asserts if either blobs are NULL
     *
     * @param[in]  i_blob1      Void pointer to effective address of blob1
     * @param[in]  i_blob1Size  Size of blob1 in bytes
     * @param[in]  i_blob2      Void pointer to effective address of blob2
     * @param[in]  i_blob2Size  Size of blob2 in bytes
     * @param[out] o_hash       SHA512 hash
     *
     * @return errlHndl_t  NULL on success
     */
    errlHndl_t hashConcatBlobs (const void* i_blob1, size_t i_blob1Size,
                                const void* i_blob2, size_t i_blob2Size,
                                SHA512_t o_buf);

    /**
     * @brief Secures/Locks I2C access to SBE Seeproms on each functional
     *        processor target in the system
     *
     * @return void   - All error logs are handled internally in this function
     */
    void secureSbeSeeproms( void ) ;

}

#endif
