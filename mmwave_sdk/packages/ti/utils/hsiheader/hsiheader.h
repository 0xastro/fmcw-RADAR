/**
 *   @file  hsiheader.h
 *
 *   @brief
 *      This is the main header file which is exported to the application
 *      developers. This can be used in conjunction with the mmWave SDK
 *      CBUFF driver to create a header which can then be attached to
 *      a data stream.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** @mainpage HSI Header
 *
 * The HSI Header is a utility library which can be used in conjunction with
 * the CBUFF Driver to create a header which can be attached to any data stream
 * which is being transmitted via the High speed interface (HSI).
 *
 * The HSI Header Utility provides the following features:-
 * - Management of Header and creating CBUFF Sessions
 *   Application developers use this feature while developing code on the
 *   XWR1xx device using the mmWave SDK. Developers will use this module
 *   to create a header for a specific CBUFF Driver session.
 *
 *   In order to use this feature application developers would need to include
 *   the following headers:-
 *      @code
            #include <ti/utils/hsiheader/hsiheader.h>
            #include <ti/utils/hsiheader/hsiprotocol.h>
        @endcode
 *
 * - Decoding the Header
 *   Once the data has been streamed out then on the remote peer the header would
 *   need to be decoded to get access to the actual data which has been streamed
 *   out.
 *
 *   In order to use this feature application developers would need to include
 *   the following headers:-
 *      @code
            #include <ti/utils/hsiheader/hsiprotocol.h>
        @endcode
 *   This header file is defined to be a standalone header file with *no* dependency
 *   on any other mmWave SDK component. This will ensure that the file can be compiled
 *   into any application on the remote peer for header decoding.
 */
/** @defgroup HSI_HEADER_UTIL      HSI Header Utility
 */
#ifndef HSIHEADER_H
#define HSIHEADER_H

/* mmWave SDK Include Files: */
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/utils/hsiheader/hsiprotocol.h>

/**
@defgroup HSI_HEADER_UTIL_EXTERNAL_FUNCTION         HSI Header Utility External Functions
@ingroup HSI_HEADER_UTIL
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the module
*/
/**
@defgroup HSI_HEADER_PROTOCOL                       HSI Header Protocol
@ingroup HSI_HEADER_UTIL
@brief
*   The section has information pertinent to the definition of the HSI Header.
*/
/**
@defgroup HSI_HEADER_UTIL_INTERNAL_FUNCTION         HSI Header Utility Internal Functions
@ingroup HSI_HEADER_UTIL
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup HSI_HEADER_UTIL_INTERNAL_DATA_STRUCTURE   HSI Header Utility Internal Data Structures
@ingroup HSI_HEADER_UTIL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the module.
*/
/**
@defgroup HSI_HEADER_UTIL_INTERNAL_DEFINITION       HSI Header Utility Internal Definition
@ingroup HSI_HEADER_UTIL
@brief
*   The section has a list of all internal definitions which are used internally
*   by the module.
*/
/**
@defgroup HSI_HEADER_UTIL_ERROR_CODE                HSI Header Utility Error Codes
@ingroup HSI_HEADER_UTIL
@brief
*   The section has a list of all the error codes which are generated by the module
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup HSI_HEADER_UTIL_ERROR_CODE
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define HSIHEADER_EINVAL                   (MMWAVE_ERRNO_HSI_HDR_BASE-1)

/**
 * @brief   Error Code: Limit Exceeded
 */
#define HSIHEADER_ENOSPACE                 (MMWAVE_ERRNO_HSI_HDR_BASE-2)

/**
@}
*/

/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/

extern int32_t HSIHeader_init (CBUFF_InitCfg* ptrInitCfg, int32_t* errCode);
extern int32_t HSIHeader_createHeader (CBUFF_SessionCfg* ptrSessionCfg, bool bAlignDataCard, HSIHeader* ptrHeader, int32_t* errCode);
extern int32_t HSIHeader_deleteHeader(HSIHeader* ptrHeader, int32_t* errCode);
extern int32_t HSIHeader_deinit (int32_t* errCode);

#ifdef __cplusplus
}
#endif

#endif /* HSIHEADER_H */

