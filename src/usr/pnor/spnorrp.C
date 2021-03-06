/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/pnor/spnorrp.C $                                      */
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
#include "pnorrp.H"
#include "spnorrp.H"
#include <pnor/pnor_reasoncodes.H>
#include <initservice/taskargs.H>
#include <sys/msg.h>
#include <trace/interface.H>
#include <errl/errlmanager.H>
#include <sys/mm.h>
#include <errno.h>
#include <util/align.H>
#include <config.h>
#include "pnor_common.H"
#include <console/consoleif.H>
#include <secureboot/service.H>
#include <secureboot/containerheader.H>

extern trace_desc_t* g_trac_pnor;

// Easy macro replace for unit testing
//#define TRACUCOMP(args...)  TRACFCOMP(args)
#define TRACUCOMP(args...)

using namespace PNOR;

/********************
 Helper Methods
 ********************/

/**
 * @brief  Static function wrapper to pass into task_create
 */
void* secure_wait_for_message( void* unused )
{
    TRACUCOMP(g_trac_pnor, "wait_for_message> " );
    Singleton<SPnorRP>::instance().waitForMessage();
    return NULL;
}


/********************
 Private/Protected Methods
 ********************/

/**
 * @brief  Constructor
 */
SPnorRP::SPnorRP()
:
iv_msgQ(NULL)
,iv_sectionVerified(0)
,iv_startupRC(0)
,iv_secAddr(NULL)
,iv_textSize(0)
{
    TRACFCOMP(g_trac_pnor, "SPnorRP::SPnorRP> " );
    // setup everything in a separate function
    initDaemon();

    TRACFCOMP(g_trac_pnor, "< SPnorRP::PnorRP : Startup Errors=%X ", iv_startupRC );
}

/**
 * @brief  Destructor
 */
SPnorRP::~SPnorRP()
{
    TRACFCOMP(g_trac_pnor, "SPnorRP::~SPnorRP> " );

    // delete the message queue we created
    if( iv_msgQ )
    {
        msg_q_destroy( iv_msgQ );
    }

    TRACFCOMP(g_trac_pnor, "< SPnorRP::~SPnorRP" );
}

/**
 * @brief  A wrapper for mm_alloc_block that encapsulates error log creation.
 */
errlHndl_t SPnorRP::allocBlock(msg_q_t i_mq, void* i_va, uint64_t i_size) const
{
    errlHndl_t l_errhdl = NULL;
    int rc = mm_alloc_block(i_mq, i_va, i_size );
    if( rc )
    {
        TRACFCOMP( g_trac_pnor,"SPnorRP::allocBlock> Error "
            "with mm_alloc_block at address 0x%.16llX : rc=%d", i_va, rc );
        /*@
         * @errortype
         * @moduleid     PNOR::MOD_SPNORRP_ALLOCATE_BLOCK
         * @reasoncode   PNOR::RC_EXTERNAL_ERROR
         * @userdata1    Requested Address
         * @userdata2    rc from mm_alloc_block
         * @devdesc      SPnorRP::initDaemon> Error from mm_alloc_block
         * @custdesc     A problem occurred while initializing secure PNOR
         */
        l_errhdl = new ERRORLOG::ErrlEntry(
                           ERRORLOG::ERRL_SEV_UNRECOVERABLE,
                           PNOR::MOD_SPNORRP_ALLOCATE_BLOCK,
                           PNOR::RC_EXTERNAL_ERROR,
                           TO_UINT64(reinterpret_cast<uint64_t>(i_va)),
                           TO_UINT64(rc),
                           true); //Add HB SW Callout
        l_errhdl->collectTrace(PNOR_COMP_NAME);
    }
    return l_errhdl;
}

/**
 * @brief  A wrapper for mm_set_permission that adds error log creation.
 */
errlHndl_t SPnorRP::setPermission(void* i_va, uint64_t i_size,
                                              uint64_t i_accessType) const
{
    errlHndl_t l_errhdl = NULL;
    int rc = mm_set_permission(reinterpret_cast<void*>(i_va), i_size,
                                                              i_accessType);
    if ( rc )
    {
        TRACFCOMP( g_trac_pnor, "SPnorRP::setPermission> Error "
            "with mm_set_permission at address 0x%.16llX : rc=%d",i_va, rc );
        /*@
         * @errortype
         * @moduleid      PNOR::MOD_SPNORRP_SET_PERMISSION
         * @reasoncode    PNOR::RC_EXTERNAL_ERROR
         * @userdata1     Requested Address
         * @userdata2     rc from mm_set_permission
         * @devdesc       Could not set permissions of the
         *                given PNOR section
         * @custdesc      A problem occurred while initializing secure PNOR
         */
        l_errhdl = new ERRORLOG::ErrlEntry(
                            ERRORLOG::ERRL_SEV_UNRECOVERABLE,
                            PNOR::MOD_SPNORRP_SET_PERMISSION,
                            PNOR::RC_EXTERNAL_ERROR,
                            TO_UINT64(reinterpret_cast<uint64_t>(i_va)),
                            TO_UINT64(rc),
                            true); // Add HB SW Callout
        l_errhdl->collectTrace(PNOR_COMP_NAME);
    }
    return l_errhdl;
}

/**
 * @brief Initialize the daemon
 */
void SPnorRP::initDaemon()
{
    TRACFCOMP(g_trac_pnor, "SPnorRP::initDaemon> " );
    errlHndl_t l_errhdl = NULL;

    do
    {
        // create a message queue for secure space
        iv_msgQ = msg_q_create();

        // create a Block for temp space
        l_errhdl = allocBlock( NULL, reinterpret_cast<void*>(TEMP_VADDR),
                                                             PNOR_SIZE );
        if( l_errhdl )
        {
            break;
        }

        // set permissions for temp space
        l_errhdl = setPermission(reinterpret_cast<void*>(TEMP_VADDR),
                                    PNOR_SIZE, WRITABLE | ALLOCATE_FROM_ZERO);
        if ( l_errhdl )
        {
            break;
        }

        // create a block for secure space
        l_errhdl = allocBlock( iv_msgQ, reinterpret_cast<void*>(SBASE_VADDR),
                                                                PNOR_SIZE );
        if( l_errhdl )
        {
             break;
        }

        // set permissions for secure space
        l_errhdl = setPermission( reinterpret_cast<void*>(SBASE_VADDR),
                                                        PNOR_SIZE, NO_ACCESS);
        if ( l_errhdl )
        {
            break;
        }

        // TODO RTC 156118 - move to new location for the next story
        verifySections();

        // start task to wait on the queue
        task_create( secure_wait_for_message, NULL );

    } while(0);

    if( l_errhdl )
    {
        iv_startupRC = l_errhdl->reasonCode();
        errlCommit(l_errhdl,PNOR_COMP_ID);
    }

    TRACUCOMP(g_trac_pnor, "< SPnorRP::initDaemon" );
}

/**
 * @brief  Load secure sections into temporary address space and verify them
 */
void SPnorRP::verifySections()
{
    SectionInfo_t l_info = {PNOR::INVALID_SECTION};
    errlHndl_t l_errhdl = NULL;
    bool failedVerify = false;

    do {

        // TODO RTC 156118 - make more generic for each secure section
        l_errhdl = getSectionInfo(HB_EXT_CODE, l_info);

        if (l_errhdl)
        {
            TRACFCOMP(g_trac_pnor,
                "< SPnorrRP::verifySections - getSectionInfo failed");

            break;
        }
        TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections getSectionInfo succeeded");

        // Note: A pointer to virtual memory in one PNOR space can be converted
        // to a pointer to any of the other two PNOR spaces and visa versa.
        // These are unsecured space, temp space, and secured space. They are
        // evenly separated by VMM_VADDR_SPNOR_DELTA and in the above order.
        // l_info.vaddr points to secure space, so we subtract a delta value
        // from it to calculate its corresponding address in temp space.
        uint8_t* l_tempAddr = reinterpret_cast<uint8_t*>(l_info.vaddr)
                                                       - VMM_VADDR_SPNOR_DELTA;

        // calcluate unsecured address from temp address
        uint8_t* l_unsecuredAddr = l_tempAddr - VMM_VADDR_SPNOR_DELTA;

        // get size of text section (secured portion)
        // Note: the textSize we get back is untrusted until verification
        // completes and should not be treated as correct until then.
        SECUREBOOT::ContainerHeader l_conHdr(l_unsecuredAddr);
        iv_textSize = ALIGN_PAGE(l_conHdr.payloadTextSize());

        TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections section start address "
                    "in temp space is 0x%.16llX\n"
                    "section start address in unsecured space is 0x%.16llX\n"
                    "l_info.size = 0x%.16llX\n"
                    "payload size = 0x%.16llX\n",
                    l_tempAddr,l_unsecuredAddr, l_info.size, iv_textSize);

        TRACDBIN(g_trac_pnor,"SPnorRP::verifySections unsecured mem now: ",
                             l_unsecuredAddr, 128);

        TRACDCOMP(g_trac_pnor,"SPnorRP::verifySections about to do memcpy");

        // copy from unsecured PNOR space to temp PNOR space
        memcpy(l_tempAddr, l_unsecuredAddr, iv_textSize
                                          + PAGESIZE); // plus header size

        TRACDCOMP(g_trac_pnor,"SPnorRP::verifySections did memcpy");
        TRACDBIN(g_trac_pnor,"SPnorRP::verifySections temp mem now: ",
                             l_tempAddr, 128);

        // rename secure space pointer for readability
        iv_secAddr = reinterpret_cast<uint8_t*>(l_info.vaddr);

        TRACFCOMP(g_trac_pnor,"section start address in secure space is "
                              "0x%.16llX",iv_secAddr);

        // verify while in temp space
        if (SECUREBOOT::enabled())
        {
            l_errhdl = SECUREBOOT::verifyContainer(l_tempAddr, l_info.size
                                                        + PAGESIZE); // header

            if (l_errhdl)
            {
                TRACFCOMP(g_trac_pnor, "< SPnorrRP::verifySections"
                      " - HBI section failed verification");
                failedVerify = true;
                break;
            }
        }

        // verification succeeded

        // Indicate that HBI section has successfully been verified.
        iv_sectionVerified |= HBI_SECTION;

        // skip the header to block secure header access
        uint8_t* l_securePayloadStart = iv_secAddr + PAGESIZE;

        // set permissions on the secured pages to writable
        l_errhdl = setPermission(l_securePayloadStart - PAGESIZE,
                                          iv_textSize + PAGESIZE, WRITABLE);
        // TODO RTC 156118 - change above two lines of code to below:
        //l_errhdl = setPermission(l_securePayloadStart, iv_textSize, WRITABLE);
        if(l_errhdl)
        {
            TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections set permissions "
                                  "failed on text section");
            break;
        }

        // set permissions on the unsecured pages to write tracked so that any
        // unprotected payload pages with dirty writes can flow back to PNOR.
        l_errhdl = setPermission(l_securePayloadStart + iv_textSize,
                                       l_info.size - iv_textSize,
                                       WRITABLE | WRITE_TRACKED);
        if(l_errhdl)
        {
            TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections set permissions "
                                  "failed on data section");
            break;
        }

        TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections set permissions "
                              "complete");
    } while(0);

    if( l_errhdl )
    {
        iv_startupRC = l_errhdl->reasonCode();
        errlCommit(l_errhdl,PNOR_COMP_ID);
        TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections there was an error");
        if (failedVerify) {
            // TODO RTC 156118
            // Halt the machine since we've failed verification.
            TRACFCOMP(g_trac_pnor,"SPnorRP::verifySections failed verify");
        }
    }
}



/**
 * @brief  Message receiver for secure space
 */
void SPnorRP::waitForMessage()
{
    TRACFCOMP(g_trac_pnor, "SPnorRP::waitForMessage>" );

    errlHndl_t l_errhdl = NULL;
    msg_t* message = NULL;
    uint8_t* user_addr = NULL;
    uint8_t* eff_addr = NULL;
    int rc = 0;
    uint64_t status_rc = 0;

    while(1)
    {
        status_rc = 0;
        TRACUCOMP(g_trac_pnor, "SPnorRP::waitForMessage> waiting for message" );
        message = msg_wait( iv_msgQ );
        if( message )
        {
            if( message->type !=  PNOR::MSG_SHUTDOWN )
            {
                // data[0] = virtual address requested
                // data[1] = address to place contents
                eff_addr = reinterpret_cast<uint8_t*>(message->data[0]);
                user_addr = reinterpret_cast<uint8_t*>(message->data[1]);
            }

            switch(message->type)
            {
                case( PNOR::MSG_SHUTDOWN ):
                    {
                        // we got a message saying there is a shutdown
                        // TODO RTC 156118
                        // As we secure more sections we are going to
                        // encounter sections that flush data back to PNOR
                        // We'll need to have:
                        // a) Register a cleanup handler with the init service
                        //    to actually call shutdown
                        // b) Add code to the handler to flush any dirty write
                        //    tracked pages back to PNOR
                    }
                    break;

                case( MSG_MM_RP_READ ):
                    {
                        uint64_t delta = VMM_VADDR_SPNOR_DELTA;

                        TRACDCOMP( g_trac_pnor, "SPnorRP::waitForMessage got a"
                            " request to read from secure space - "
                            "message : user_addr=%p, eff_addr=%p, msgtype=%d, "
                            "textSize=0x%.16llX secAddr0x%.16llX", user_addr,
                            eff_addr, message->type, iv_textSize, iv_secAddr);

                        // determine the source of the data depending on
                        // whether it is part of the secure payload.
                        // by the way, this if could be removed to make this
                        // purely arithmetic
                        if (eff_addr >= (iv_secAddr + iv_textSize))
                        {
                            delta += VMM_VADDR_SPNOR_DELTA;
                        }
                        TRACDCOMP( g_trac_pnor, "SPnorRP::waitForMessage "
                            "source address: %p", eff_addr - delta);

                        // depending on the value of delta, memcpy from either
                        // temp space or unsecured pnor space over to secure
                        // pnor space
                        memcpy(user_addr, eff_addr - delta, PAGESIZE);
                        // if the page came from temp space then free up
                        // the temp page now that we're done with it
                        if (eff_addr < (iv_secAddr + iv_textSize))
                        {
                            mm_remove_pages(RELEASE, eff_addr - delta,
                                            PAGESIZE);
                        }
                    }
                    break;

                case( MSG_MM_RP_WRITE ):
                    {
                        TRACDCOMP( g_trac_pnor, "SPnorRP::waitForMessage "
                            "someone is trying to write to a secure section!");
                        // TODO RTC 156118
                        // As we secure more sections we are going to
                        // encounter sections that flush data back to PNOR
                        // dirty writes need to be flushed back to PNOR
                        assert(false); // should never get here
                    }
                    break;

                default:
                    TRACDCOMP( g_trac_pnor, "SPnorRP::waitForMessage> "
                    "Unrecognized message type : user_addr=%p, eff_addr=%p,"
                    " msgtype=%d", user_addr, eff_addr, message->type );
                    /*@
                     * @errortype
                     * @moduleid     PNOR::MOD_SPNORRP_WAITFORMESSAGE
                     * @reasoncode   PNOR::RC_INVALID_MESSAGE_TYPE
                     * @userdata1    Message type
                     * @userdata2    Requested Virtual Address
                     * @devdesc      PnorRP::waitForMessage> Unrecognized
                     *               message type
                     * @custdesc     A problem occurred while accessing
                     *               the boot flash.
                     */
                    l_errhdl = new ERRORLOG::ErrlEntry(
                                           ERRORLOG::ERRL_SEV_UNRECOVERABLE,
                                           PNOR::MOD_SPNORRP_WAITFORMESSAGE,
                                           PNOR::RC_INVALID_MESSAGE_TYPE,
                                           TO_UINT64(message->type),
                                           reinterpret_cast<uint64_t>(eff_addr),
                                           true /*Add HB SW Callout*/);
                    l_errhdl->collectTrace(PNOR_COMP_NAME);
                    status_rc = -EINVAL; /* Invalid argument */
            }

            assert(msg_is_async(message) == false);

            if( l_errhdl )
            {
                errlCommit(l_errhdl,PNOR_COMP_ID);
            }

            /*  Expected Response:
             *      data[0] = virtual address requested
             *      data[1] = rc (0 or negative errno value)
             *      extra_data = Specific reason code.
             */
            message->data[1] = status_rc;
            message->extra_data = 0;
            rc = msg_respond( iv_msgQ, message );
            if( rc )
            {
                TRACFCOMP(g_trac_pnor,
                          "SPnorRP::waitForMessage> Error from msg_respond, "
                          "giving up : rc=%d", rc );
                break;
            }
        }
        assert(message != NULL);
    }

    TRACFCOMP(g_trac_pnor, "< SPnorRP::waitForMessage" );
}
