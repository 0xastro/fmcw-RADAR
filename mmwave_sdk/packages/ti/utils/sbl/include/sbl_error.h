/*
 *  @file sbl_error.h
 *  @brief This file contains the defines for all the errors and status
 *  used flags used during the metaimage validation and loading.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

#ifndef SBL_ERROR_H
#define SBL_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#define SBL_RPRC_IMG1_AUTH_FAILURE                          ((uint64_t)(0x0000000000000001U))  /* BIT0 */
#define SBL_RPRC_IMG2_AUTH_FAILURE                          ((uint64_t)(0x0000000000000002U))  /* BIT1 */
#define SBL_RPRC_IMG3_AUTH_FAILURE                          ((uint64_t)(0x0000000000000004U))  /* BIT2 */
#define SBL_RPRC_HDR_NOT_FOUND                              ((uint64_t)(0x0000000000000008U))  /* BIT3 */
#define SBL_METAHEADER_NOT_FOUND                            ((uint64_t)(0x0000000000000010U))  /* BIT4 */

#define SBL_CERT_FIELD_INVALID_SUBSYSTEM                    ((uint64_t)(0x0000000000000020U))  /* BIT5 */

#define SBL_RPRC_PARSER_FILE_LENGTH_MISMATCH                ((uint64_t)(0x0000000000000040U))  /* BIT6 */
#define SBL_RPRC_PARSER_MSS_FILE_OFFSET_MISMATCH            ((uint64_t)(0x0000000000000080U))  /* BIT7 */
#define SBL_RPRC_PARSER_BSS_FILE_OFFSET_MISMATCH            ((uint64_t)(0x0000000000000100U))  /* BIT8 */
#define SBL_RPRC_PARSER_DSS_FILE_OFFSET_MISMATCH            ((uint64_t)(0x0000000000000200U))  /* BIT9 */

#define SBL_MSSIMAGE_NOT_FOUND                              ((uint64_t)(0x0000000000000400U))  /* BIT10*/
#define SBL_METAHEADER_NUMFILES_ERROR                       ((uint64_t)(0x0000000000000800U))  /* BIT11*/
#define SBL_METAHEADER_CRC_FAILURE                          ((uint64_t)(0x0000000000001000U))  /* BIT12*/
#define SBL_RPRC_IMG4_AUTH_FAILURE                          ((uint64_t)(0x0000000000002000U))  /* BIT13*/
#define SBL_RPRC_PARSER_CONFIG_FILE_OFFSET_MISMATCH         ((uint64_t)(0x0000000000004000U))  /* BIT14*/

#ifdef __cplusplus
}
#endif

#endif /* SBL_ERROR_H */

