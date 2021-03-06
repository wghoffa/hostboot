/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/include/usr/isteps/istep10list.H $                        */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2016                        */
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
#ifndef __ISTEPS_ISTEP10LIST_H
#define __ISTEPS_ISTEP10LIST_H


/**
 * @file    istep10list.H
 *
 *  Step 10 Hostboot SBE Centaur Init
 *  IPL FLow Doc        v1.28 (12/03/12)
 *
 *    10.1    host_prd_hwreconfig
 *          : Hook for prd to handle reconfig
 *    10.2    cen_sbe_tp_chiplet_init1
 *          : TP Chiplet init
 *    10.3    cen_sbe_pll_initf
 *          : Program Nest PLL
 *    10.4    cen_sbe_pll_setup
 *          : Setup Nest PLL
 *    10.5    cen_sbe_tp_chiplet_init2
 *          : Cen TP Chiplet init 2
 *    10.6    cen_sbe_tp_arrayinit 
 *          : Cen TP Chiplet array init
 *    10.7    cen_sbe_tp_chiplet_init3
 *          : Cen TP Chiplet Start clocks
 *    10.8    cen_sbe_chiplet_init
 *          : Cen Chiplet Init
 *    10.9    cen_sbe_arrayinit
 *          : Chiplet arrayinit
 *    10.10   cen_sbe_dts_init
 *          : Cen DTS init
 *    10.11   cen_sbe_initf
 *          : Cen Scan overrides
 *    10.12   cen_sbe_do_manual_inits
 *          : Manual Cen Scans
 *    10.13   cen_sbe_nest_startclocks
 *          : Start Cen Nest
 *    10.14   cen_sbe_scominits
 *          : Perform any Cen scom inits

 *  Please see the note in initsvcstructs.H for description of
 *      the ISTEPNAME macro.
 *
 */

#include    <initservice/initsvcstructs.H>
#include    <initservice/initsvcreasoncodes.H>

//  include prototypes file
#include    "../../../usr/hwpf/hwp/sbe_centaur_init/sbe_centaur_init.H"
#include <config.h>

namespace   INITSERVICE
{

const   TaskInfo    g_istep10[]  =   {
        {
                "",         // dummy, index 0
                NULL,
                { NONE, EXT_IMAGE, IPL_NOOP, false }
        },

// Reduce extended img size for VPO
#ifndef CONFIG_VPO_COMPILE
        {
                ISTEPNAME(10,01,"host_prd_hwreconfig"),
                HWAS::host_prd_hwreconfig,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true,
                        MEMORY_INIT }
        },
        {
                ISTEPNAME(10,02,"cen_sbe_tp_chiplet_init1"),
                SBE_CENTAUR_INIT::call_sbe_centaur_init,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,03,"cen_sbe_pll_initf"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,04,"cen_sbe_pll_setup"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,05,"cen_sbe_tp_chiplet_init2"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,06,"cen_sbe_tp_arrayinit"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,07,"cen_sbe_tp_chiplet_init3"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,08,"cen_sbe_chiplet_init"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,09,"cen_sbe_arrayinit"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,10,"cen_sbe_dts_init"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,11,"cen_sbe_initf"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,12,"cen_sbe_do_manual_inits"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,13,"cen_sbe_nest_startclocks"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
        {
                ISTEPNAME(10,14,"cen_sbe_scominits"),
                HWAS::host_stub,
                { START_FN, EXT_IMAGE, NORMAL_IPL_OP, true }
        },
#endif

};

const DepModInfo g_istep10Dependancies = {
    {
#ifndef CONFIG_VPO_COMPILE
        DEP_LIB(libsbe_centaur_init.so),
        DEP_LIB(libfapiporeve.so),
        DEP_LIB(libporeve.so),
        DEP_LIB(libbuild_winkle_images.so),
        DEP_LIB(libsbe.so),
        DEP_LIB(libproc_hwreconfig.so),
        DEP_LIB(libslave_sbe.so),
#ifdef CONFIG_IPLTIME_CHECKSTOP_ANALYSIS
        DEP_LIB(libocc.so),
#endif
#ifdef CONFIG_SECUREBOOT
        DEP_LIB(libsecureboot_base.so),
#endif
#endif
        NULL
    }
};

const ExtTaskInfo g_istep10TaskList = {
    &(g_istep10[0]),
    ( sizeof(g_istep10)/sizeof(TaskInfo) ),
    &g_istep10Dependancies
};

};  // end namespace

#endif
