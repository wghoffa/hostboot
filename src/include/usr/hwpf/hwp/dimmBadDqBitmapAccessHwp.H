/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/include/usr/hwpf/hwp/dimmBadDqBitmapAccessHwp.H $         */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2012,2014              */
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
// $Id: dimmBadDqBitmapAccessHwp.H,v 1.3 2013/10/03 20:40:52 dedahle Exp $
/**
 *  @file dimmBadDqBitmapAccessHwp.H
 *
 *  @brief FW Team HWP that accesses the Bad DQ Bitmap.
 */

/*
 * Change Log ******************************************************************
 * Flag     Defect/Feature  User        Date        Description
 * ------   --------------  ----------  ----------- ----------------------------
 *                          mjjones     02/17/2012  Created.
 */

#ifndef DIMMBADDQBITMAPACCESSHWP_H_
#define DIMMBADDQBITMAPACCESSHWP_H_

#include <fapi.H>
#include <dimmConsts.H>

typedef fapi::ReturnCode (*dimmBadDqBitmapAccessHwp_FP_t)(
    const fapi::Target &,
    const fapi::Target &,
    const uint8_t (&)[DIMM_DQ_MAX_DIMM_RANKS][DIMM_DQ_RANK_BITMAP_SIZE],
    const bool);

extern "C"
{

/**
 * @brief FW Team HWP that accesses the Bad DQ Bitmap. It accesses the raw data
 *        from DIMM SPD and does any necessary processing to turn it into a
 *        bitmap from a Centaur DQ point of view. If the data in SPD is not
 *        valid then it has never been written and all zeroes are returned (no
 *        bad DQs).
 *
 * This HWP should be called by HWP/PLAT code to access the BAD DQ Bitmap
 *
 * Note that the MSB of each byte corresponds to the lowest DQ.
 * if (data[1][0] == 0x80) then rank 1, Centaur DQ0 is bad
 * if (data[1][0] == 0x40) then rank 1, Centaur DQ1 is bad
 * if (data[1][1] == 0x20) then rank 1, Centaur DQ10 is bad
 *
 * @param[in] i_mba   Reference to MBA Target
 * @param[in] i_dimm  Reference to DIMM Target
 * @param[io] io_data Reference to bad DQ bitmap data for the DIMM.
 * @param[in] i_get   True if getting DQ Bitmap data. False if setting data.
 *
 * @return ReturnCode
 */
fapi::ReturnCode dimmBadDqBitmapAccessHwp(
    const fapi::Target & i_mba,
    const fapi::Target & i_dimm,
    uint8_t (&io_data)[DIMM_DQ_MAX_DIMM_RANKS][DIMM_DQ_RANK_BITMAP_SIZE],
    const bool i_get);

}

#endif
