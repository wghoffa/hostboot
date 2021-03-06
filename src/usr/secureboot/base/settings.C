/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/secureboot/base/settings.C $                          */
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
#include <errl/errlentry.H>
#include <devicefw/userif.H>
#include <secureboot/service.H>

#include "settings.H"

// SECUREBOOT : General driver traces
trace_desc_t* g_trac_secure = NULL;
TRAC_INIT(&g_trac_secure, SECURE_COMP_NAME, KILOBYTE); //1K


namespace SECUREBOOT
{
    void Settings::_init()
    {
        errlHndl_t l_errl = NULL;
        size_t size = sizeof(iv_regValue);

        // Read / cache security switch setting from processor.
        l_errl = deviceRead(TARGETING::MASTER_PROCESSOR_CHIP_TARGET_SENTINEL,
                            &iv_regValue, size,
                            DEVICE_SCOM_ADDRESS(PROC_SECURITY_SWITCH_REGISTER));

        // If this errors, we're in bad shape and shouldn't trust anything.
        assert(NULL == l_errl);
    }

    bool Settings::getEnabled() const
    {
        return 0 != (getSecuritySwitch()
                     & PROC_SECURITY_SWITCH_TRUSTED_BOOT_MASK);
    }

    bool Settings::getJumperState() const
    {
        return 0 != (getSecuritySwitch()
                     & PROC_SECURITY_SWITCH_JUMPER_STATE_MASK);
    }

    uint64_t Settings::getSecuritySwitch() const
    {
        return iv_regValue;
    }
}
