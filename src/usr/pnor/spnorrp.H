/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/pnor/spnorrp.H $                                      */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2011,2016                        */
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
#ifndef __PNOR_SPNORRP_H
#define __PNOR_SPNORRP_H
#include <pnor/pnorif.H>
#include <sys/msg.h>
#include <stdint.h>
#include <builtins.h>
#include <errl/errlentry.H>
#include <vmmconst.h>
#include <map>
#include "pnor_common.H"
#include "ffs.h"
#include <config.h>

/**
 * Secure PNOR Resource Provider
 */

class SPnorRP
{
  public:
   /**
     * @brief Static Initializer
     * @param[in]  ref to errlHndl_t
     */
    static void init( errlHndl_t &io_rtaskRetErrl );

   /**
     * @brief  Returns true if the initial startup failed for some reason
     * @param[out]  Return code
     * @return  true if startup failed
     */
    bool didStartupFail( uint64_t& o_rc ) const
    {
        if( iv_startupRC )
        {
            o_rc = iv_startupRC;
            return true;
        }
        return false;
    };


  protected:
    /**
     * @brief  Constructor, default TOC offsets to side A
     */
    SPnorRP();

    /**
     * @brief  Destructor
     */
    ~SPnorRP();

  private:

    enum
    {
        SBASE_VADDR = VMM_VADDR_SPNOR_RP, /**< 8GB = 0x200000000*/
        TEMP_VADDR = VMM_VADDR_SPNOR_TEMP, /**< 5GB = 0x140000000 */
        LAST_VADDR = SBASE_VADDR + PNOR::PNOR_SIZE,  /**< End of our VA range */
    };

    /* bitwise enumeration to keep track of verified sections */
    enum
    {
        HBI_SECTION = 0x1,
    };

    /**
     * Pointer to the message queue where we receive messages for
     * secure space.
     */
    msg_q_t iv_msgQ;

    /**
     * Indicates that a section has been verified and is able to be accessed
     * in the secured space now.
     */
    uint64_t iv_sectionVerified;

    /**
     * Remember that we failed during initial startup
     *   This is set by startup methods to indicate to constructor that
     *   something went wrong
     */
    uint64_t iv_startupRC;

    /**
     * Keep track of secured payload size and secure section address
     */
    uint8_t* iv_secAddr;
    size_t iv_textSize;


    /**
     * @brief Initialize the daemon, called by constructor
     */
    void initDaemon();

    /**
     * @brief Load secure sections into temporary address space and verify them
     */
    void verifySections();

    /**
     * @brief  Message receiver for secure space
     */
    void waitForMessage();

    // disable copy ctor
    SPnorRP(const SPnorRP&);

    // disable assignment operator
    SPnorRP& operator=(const SPnorRP&);

    // allow local helper function to call private methods
    friend void* secure_wait_for_message( void* unused );

    /**
     * @brief  A wrapper for mm_alloc_block that encapsulates error log
     *         creation.
     */
    errlHndl_t allocBlock(msg_q_t mq, void* va, uint64_t size) const;

    /**
     * @brief  A wrapper for mm_set_permission that encapsulates error log
     *         creation.
     */
    errlHndl_t setPermission(void* va, uint64_t size,
                                       uint64_t accessType) const;
};


#endif
