/****************************************************************************************
 * FileName     : rl_trace.h
 *
 * Description  : This file defines the datatypes.
 *
 ****************************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated. - TI web address www.ti.com
 *---------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
#ifndef TRACE_H
#define TRACE_H

/****************************************************************************************
* INCLUDE FILES
****************************************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

 /****************************************************************************************
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#if RL_DISABLE_LOGGING
#define RL_LOGV_ARG0(x)
#define RL_LOGV_ARG1(x,y)
#define RL_LOGV_ARG2(x,y,z)
#define RL_LOGV_ARG3(w,x,y,z)
#define RL_LOGD_ARG0(x)
#define RL_LOGD_ARG1(x,y)
#define RL_LOGI_ARG0(x)
#define RL_LOGI_ARG1(x,y)
#define RL_LOGW_ARG0(x)
#define RL_LOGW_ARG1(x,y)
#define RL_LOGE_ARG0(x)
#define RL_LOGE_ARG1(x,y)
#define RL_LOGE_ARG2(x,y,z)
#else
#define RL_LOGV_ARG0(x)             {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_VERBOSE);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                fPtr("[VER ] %s:%d::" x, __FUNCTION__, __LINE__);\
            }\
                                }

#define RL_LOGV_ARG1(x,y)      {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_VERBOSE);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[MMW_VER ] %s:%d::" x,\
                __FUNCTION__, __LINE__, (y));\
            }\
        }

#define RL_LOGV_ARG2(x,y,z)    {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_VERBOSE);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[MMW_VER ] %s:%d::" x,\
                __FUNCTION__, __LINE__, (y), (z));\
            }\
        }

#define RL_LOGV_ARG3(w,x,y,z)    {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_VERBOSE);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[MMW_VER ] %s:%d::" w,\
                __FUNCTION__, __LINE__, (x), (y), (z));\
            }\
        }

#define RL_LOGD_ARG0(x)         {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_DEBUG);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[DBG ] %s:%d::" x, __FUNCTION__, __LINE__);\
            }\
        }

#define RL_LOGD_ARG1(x,y)      {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_DEBUG);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[DBG ] %s:%d::" x,\
                __FUNCTION__, __LINE__, (y));\
            }\
        }

#define RL_LOGI_ARG0(x)        {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_INFO);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[INFO] %s:%d::" x,\
                       __FUNCTION__, __LINE__);\
            }\
        }

#define RL_LOGI_ARG1(x,y)       {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_INFO);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[INFO] %s:%d::" x,\
                __FUNCTION__, __LINE__, (y));\
            }\
        }

#define RL_LOGW_ARG0(x)         {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_WARNING);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[WARN] %s:%d::", x, __FUNCTION__, __LINE__);\
            }\
        }

#define RL_LOGW_ARG1(x,y)      {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_WARNING);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[WARN] %s:%d::" x,\
                __FUNCTION__, __LINE__, (y));\
            }\
        }

#define RL_LOGE_ARG0(x)        {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_ERROR);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[ERR ] %s:%d::" x,\
                __FUNCTION__, __LINE__);\
            }\
        }

#define RL_LOGE_ARG1(x,y)      {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_ERROR);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[ERR ] %s:%d::" x,\
                  __FUNCTION__, __LINE__, (y));\
            }\
        }

#define RL_LOGE_ARG2(x,y,z)      {\
            /* get the logging function pointer as per requested Level */\
            rlPrintFptr fPtr = rlGetLogFptr(RL_DBG_LEVEL_ERROR);\
            /* check for NULL Pointer */\
            if(fPtr != NULL)\
            {\
                /* log the data with function name, line no and passed arg */\
                (void)fPtr("[ERR ] %s:%d::" x,\
                  __FUNCTION__, __LINE__, (y), (z));\
            }\
        }
#endif


/* Debug fuunction */
rlPrintFptr rlGetLogFptr(rlUInt8_t dbgLevel);
rlReturnVal_t rlLogInit(void);


#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF rl_trace.h
 */

