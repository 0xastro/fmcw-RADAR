/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== mailbox.c ========
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/mailbox/include/mailbox_internal.h>

/**************************************************************************
 *************************** Global variables******************************
 **************************************************************************/
/**
 * @brief
 *  Global Variable for tracking information required by the mailbox driver.
 */
Mailbox_MCB gMailboxMCB = {0};


/* Default Mailbox configuration structure */
const Mailbox_Config Mailbox_defaultCfg = {
    MAILBOX_MODE_BLOCKING,                        /*Read mode         */
    MAILBOX_MODE_BLOCKING,                        /*Write mode        */
    MAILBOX_WAIT_FOREVER,                         /*Read Timeout      */
    MAILBOX_WAIT_FOREVER,                         /*Write Timeout     */
    NULL,                                         /*Read Callback     */
    MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED,  /*Operation mode    */
    MAILBOX_DATA_TRANSFER_MEMCPY,                 /*Data transfer mode*/
    MAILBOX_CHTYPE_SINGLE,                        /*Channel type      */
    MAILBOX_CH_ID_0                               /*Channel ID        */
};


/**************************************************************************
 ************************ Mailbox Driver Local Functions **********************
 **************************************************************************/

static void Mailbox_hwCfg(Mailbox_Driver* driver);
static void Mailbox_boxFullISRBss     (uintptr_t arg);
static void Mailbox_boxEmptyISRBss    (uintptr_t arg);
#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
static void Mailbox_boxEmptyISRMssDss (uintptr_t arg);
static void Mailbox_boxFullISRMssDss  (uintptr_t arg);
#endif

/**************************************************************************
 ************************ Mailbox Driver Functions   **********************
 **************************************************************************/

/*
 *  ======== Mailbox_init ========
 */
int32_t Mailbox_init(Mailbox_Type localEndpoint)
{
    int32_t    retVal = 0;
    int32_t    i;
    uintptr_t  key;

    #ifdef SOC_XWR14XX 
    if (localEndpoint != MAILBOX_TYPE_MSS)
    #else
    if ((localEndpoint != MAILBOX_TYPE_MSS) && (localEndpoint != MAILBOX_TYPE_DSS))
    #endif
    {
        DebugP_log1 ("MAILBOX: Mailbox_init error! Invalid localEndpoint = %d\n",localEndpoint);
        /* Error: Invalid argument or usage */
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        /* Critical Section Protection*/
        key = HwiP_disable();
        if (gMailboxMCB.initFlag == 0) 
		{            
            /* Initialize global variables */
            gMailboxMCB.bssInstCount         = 0;
            gMailboxMCB.mssDssInstCount      = 0;
            /* Set local endpoint which is the same for all instances of the driver in this processor */
            gMailboxMCB.localEndpoint        = localEndpoint;
            gMailboxMCB.mssDssChType         = MAILBOX_CHTYPE_SINGLE;
            gMailboxMCB.lastMsgSentHandle    = NULL;
            gMailboxMCB.mssDssHwCfgPtr       = NULL;    
            gMailboxMCB.mssDssReadChIDInUse  = MAILBOX_UNUSED_CHANNEL_ID;    
            gMailboxMCB.mssDssWriteChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;    
            
            memset ((void *)&(gMailboxMCB.errCnt), 0, sizeof(gMailboxMCB.errCnt));
            memset ((void *)&(gMailboxMCB.hwiHandles), 0, sizeof(gMailboxMCB.hwiHandles));            
            for(i=0; i <= MAILBOX_CH_ID_MAX; i++)
			{
                gMailboxMCB.handleArray[i] = NULL;  
            }				

            gMailboxMCB.initFlag = 1;                
        }
        else
        {
            DebugP_log0 ("MAILBOX: Mailbox_init error! Mailbox Driver has already been initialized\n");
            retVal = MAILBOX_EINITIALIZED;
        }
        /* Release the critical section: */
        HwiP_restore(key);
    }    
    
    return retVal;
}

/*
 *  ======== Mailbox_open ========
 */
Mbox_Handle Mailbox_open(Mailbox_Type remoteEndpoint, const Mailbox_Config *cfg,  int32_t* errCode)
{
    Mailbox_Driver*     mailboxDriver;
    Mbox_Handle      retHandle = NULL;
    HwiP_Params         hwiParams;
    SemaphoreP_Params   semParams;
    uintptr_t           key;

    *errCode = 0;

    /* Critical Section Protection*/
    key = HwiP_disable();
    
    /* Sanity Check: Validate the arguments */
    #ifdef SOC_XWR14XX 
    if ((gMailboxMCB.initFlag == 0) ||(cfg == NULL) || (remoteEndpoint != MAILBOX_TYPE_BSS))
    #else
    if ((gMailboxMCB.initFlag == 0) ||(cfg == NULL) || (remoteEndpoint == gMailboxMCB.localEndpoint) || (remoteEndpoint > MAILBOX_TYPE_LAST))
    #endif
    {
        DebugP_log0 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed due to bad input argument.\n");
        /* Error: Invalid argument or usage */
        *errCode = MAILBOX_EINVAL;
        goto exit;
    }
    
    if((remoteEndpoint == MAILBOX_TYPE_BSS) && (gMailboxMCB.bssInstCount > 0))
    {
        DebugP_log0 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed. BSS driver instance already open.\n");
        /* Error: Invalid argument or usage */
        *errCode = MAILBOX_EINVAL;
        goto exit;
    }
    
    #if (defined(SOC_XWR16XX ) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
    /* If this is MSS<->DSS communication, check if this is a valid open of a new instance
       of the driver. Only MSS<->DSS can have multiple instances of the driver. Because the driver
       can only run on MSS or DSS, we just need to check the remoteEndpoint to see if this is a
       driver instance between MSS<->DSS.*/
    if(remoteEndpoint != MAILBOX_TYPE_BSS)
    {
        /*Check if the channel type is valid*/
        if(cfg->chType > MAILBOX_CHTYPE_MULTI)
        {
            DebugP_log1 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle type parameter (%d).\n",cfg->chType);
            /* Error: Invalid configuration */
            *errCode = MAILBOX_EBADCHTYPE;
            goto exit;
        }
    
        /*Check if channel type is consistent*/
        if(gMailboxMCB.mssDssInstCount == 0)
        {
            /*First mss/dss instance. Set global variable to store channel type that must be used for all mss-dss instances.*/
            gMailboxMCB.mssDssChType = cfg->chType;
        }
        else
        {
            /*Multiple mss-dss instances. Therefore, must use mss-dss multi-channels for all instances.*/
            if((gMailboxMCB.mssDssChType != MAILBOX_CHTYPE_MULTI) || (cfg->chType != MAILBOX_CHTYPE_MULTI))
            {
                DebugP_log1 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle type parameter (%d).\n",cfg->chType);
                DebugP_log0 ("MAILBOX: When using multiple mss-dss instances all channels must be of type MAILBOX_CHTYPE_MULTI.\n");
                /* Error: Invalid configuration */
                *errCode = MAILBOX_EBADCHTYPE;
                goto exit;
            }
        }
        
        /*Check if channel ID is consistent. Note that chId is only used for multi-channel.
          Single-channel does not use header or channel ID.*/
        if(cfg->chType == MAILBOX_CHTYPE_MULTI)
        {
            /*For multi-channel need to check ID value*/
            /*Check if the ID is valid*/
            if(cfg->chId > MAILBOX_CH_ID_MAX)
            {
                DebugP_log1 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad Channle ID parameter (%d).\n",cfg->chId);
                /* Error: Invalid configuration */
                *errCode = MAILBOX_EBADCHID;
                goto exit;
            }
            
            /*Check if the ID is already in use*/        
            if(gMailboxMCB.handleArray[cfg->chId] != NULL)
            {
                DebugP_log1 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed. Channel ID (%d) is already in use.\n",cfg->chId);
                /* Error: Invalid configuration */
                *errCode = MAILBOX_EBADCHID;
                goto exit;
            }
        }    
    }
    #endif    
    
    /* Check if configuration is valid */
    if( (cfg->writeMode        == MAILBOX_MODE_CALLBACK) ||
        (cfg->opMode           != MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED) ||
        (cfg->dataTransferMode != MAILBOX_DATA_TRANSFER_MEMCPY) )
    {
        DebugP_log0 ("MAILBOX: Mailbox_open error! Mailbox Driver open failed with bad configuration.\n");
        /* Error: Invalid configuration */
        *errCode = MAILBOX_EINVALCFG;
        goto exit;
    }

    /* Allocate memory for the Mailbox Driver */
    mailboxDriver = MemoryP_ctrlAlloc ((uint32_t)sizeof(Mailbox_Driver), 0);
    if (mailboxDriver == NULL)
    {
        DebugP_log0 ("MAILBOX: Mailbox_open error! Mailbox Driver memory allocation failed.\n");
        /* Error: No memory available. */
        *errCode = MAILBOX_ENOMEM;
        goto exit;
    }
    
    /* Initialize the allocated memory: */
    memset ((void *)mailboxDriver, 0, sizeof(Mailbox_Driver));

    /* Populate the driver with initial configuration */
    memcpy ((void *)&mailboxDriver->cfg, (const void *)cfg, sizeof(Mailbox_Config));
    
    mailboxDriver->remoteEndpoint = remoteEndpoint;
    mailboxDriver->localEndpoint  = gMailboxMCB.localEndpoint;
    
    /* Setup the return handle: */
    retHandle = (Mbox_Handle)mailboxDriver;

    /* Populate hardware configuration */
    Mailbox_hwCfg(mailboxDriver);

    if((remoteEndpoint == MAILBOX_TYPE_BSS) || (gMailboxMCB.mssDssInstCount == 0))
    {        
        /* Clear status register */
        ((mailboxDriver->hwCfg)->baseLocalToRemote.reg)->INT_STS_CLR = CSL_FINSR (((mailboxDriver->hwCfg)->baseLocalToRemote.reg)->INT_STS_CLR, 0U, 0U, 1U);
    
        /* Register the Interrupt Handler: Every mailbox has 2 interrupts: "mailbox full" and "mailbox empty"*/
        /************** Mailbox full ***********/
        HwiP_Params_init(&hwiParams);
        if(remoteEndpoint == MAILBOX_TYPE_BSS)
        {
            hwiParams.name = "MAILBOX_FULL_BSS";
            hwiParams.arg  = (uintptr_t)retHandle;
            gMailboxMCB.hwiHandles.bssFull = HwiP_create((mailboxDriver->hwCfg)->boxFullIntNum, Mailbox_boxFullISRBss, &hwiParams);
            /* Debug Message: */
            DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Full to BSS HWI ISR [%p] for Interrupt %d\n",
                          gMailboxMCB.hwiHandles.bssFull, (mailboxDriver->hwCfg)->boxFullIntNum);
        }
        #if (defined(SOC_XWR16XX ) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
        else
        {
            hwiParams.name = "MAILBOX_FULL_MSSDSS";
            /*If the remote endpoint is MSS or DSS, we may have multiple instances of the driver but only one interrupt.
             In case of multiple instances (multi-channel):
             Argument is NULL and interrupt will have to check hash array to find handle based on the channel ID.
             In case of single channel, there is no header to recover the channel ID, so need to pass the handle in the argument.
            */
            if(cfg->chType == MAILBOX_CHTYPE_SINGLE)
            {
                hwiParams.arg  = (uintptr_t)retHandle;
            }
            else
            {
                hwiParams.arg  = NULL;
            }    
            gMailboxMCB.hwiHandles.mssDssFull = HwiP_create((mailboxDriver->hwCfg)->boxFullIntNum, Mailbox_boxFullISRMssDss, &hwiParams);
            /* Debug Message: */
            DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Full to MSS or DSS HWI ISR [%p] for Interrupt %d\n",
                          gMailboxMCB.hwiHandles.mssDssFull, (mailboxDriver->hwCfg)->boxFullIntNum);
        }
        #endif		

        /************** Mailbox empty ***********/
        HwiP_Params_init(&hwiParams);
        if(remoteEndpoint == MAILBOX_TYPE_BSS)
        {
            hwiParams.name = "MAILBOX_EMPTY_BSS";
            hwiParams.arg  = (uintptr_t)retHandle;
            gMailboxMCB.hwiHandles.bssEmpty = HwiP_create((mailboxDriver->hwCfg)->boxEmptyIntNum, Mailbox_boxEmptyISRBss, &hwiParams);
            /* Debug Message: */
            DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Empty to BSS HWI ISR [%p] for Interrupt %d\n",
                          gMailboxMCB.hwiHandles.bssEmpty, (mailboxDriver->hwCfg)->boxEmptyIntNum);
        }
        #if (defined(SOC_XWR16XX ) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
        else
        {
            hwiParams.name = "MAILBOX_EMPTY_MSSDSS";
            /*If the remote endpoint is MSS or DSS, we may have multiple instances of the driver but only one interrupt.
             In case of multiple instances (multi-channel):
             Argument is NULL and interrupt will have to check hash array to find handle based on the channel ID.
             In case of single channel, there is no header to recover the channel ID, so need to pass the handle in the argument.
            */
            if(cfg->chType == MAILBOX_CHTYPE_SINGLE)
            {
                hwiParams.arg  = (uintptr_t)retHandle;
            }
            else
            {
                hwiParams.arg  = NULL;
            }    
            gMailboxMCB.hwiHandles.mssDssEmpty = HwiP_create((mailboxDriver->hwCfg)->boxEmptyIntNum, Mailbox_boxEmptyISRMssDss, &hwiParams);
            /* Debug Message: */
            DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Empty to MSS or DSS HWI ISR [%p] for Interrupt %d\n",
                          gMailboxMCB.hwiHandles.mssDssEmpty, (mailboxDriver->hwCfg)->boxEmptyIntNum);
        }
		#endif
    }                  
        
    /* Is the write mode blocking? */
    if (mailboxDriver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
    {
        /* YES: Create a binary semaphore which is used to handle the Blocking operation. */
        SemaphoreP_Params_init(&semParams);
        semParams.mode          = SemaphoreP_Mode_BINARY;
        mailboxDriver->writeSem = SemaphoreP_create(0, &semParams);
    }

    /* Is the read mode blocking? */
    if (mailboxDriver->cfg.readMode == MAILBOX_MODE_BLOCKING)
    {
        /* YES: Create a binary semaphore which is used to handle the Blocking operation. */
        SemaphoreP_Params_init(&semParams);
        semParams.mode         = SemaphoreP_Mode_BINARY;
        mailboxDriver->readSem = SemaphoreP_create(0, &semParams);
    }

    /* Instance of the driver for the specific endpoint is created */
    if(remoteEndpoint == MAILBOX_TYPE_BSS)
    {
        /* If this is an instance of the driver to talk to BSS, increment count.*/
        gMailboxMCB.bssInstCount++;
    }
    #if (defined(SOC_XWR16XX ) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
    else
    {
        gMailboxMCB.mssDssInstCount++;
        /* Store handle of driver into hash array */
        gMailboxMCB.handleArray[cfg->chId] = retHandle;
    }
	#endif
    
    DebugP_log1 ("MAILBOX: Mailbox Driver (%p) open successful.\n", mailboxDriver);

exit:    
    /* Release the critical section: */
    HwiP_restore(key);
    return retHandle;
}    

/*
 *  ======== Mailbox_Config_init ========
 */
int32_t Mailbox_Config_init(Mailbox_Config *cfg)
{
    int32_t    retVal = 0;
    if(cfg == NULL)
    {
        retVal = MAILBOX_EINVAL;        
        DebugP_log0 ("MAILBOX: Mailbox_Config_init Error! cfg is NULL\n");
    }
    else
    {
        memcpy ((void *)cfg, (void *)&Mailbox_defaultCfg, sizeof(Mailbox_Config));
    }
    
    return retVal;
}

/*
 *  ======== Mailbox_read ========
 */
int32_t Mailbox_read(Mbox_Handle handle, uint8_t *buffer, uint32_t size)
{
    Mailbox_Driver*     driver;
    SemaphoreP_Status   status;
    int32_t             retVal = 0;

    driver = (Mailbox_Driver*)handle;
    
    /* Sanity Check: Validate the arguments */
    if ((size == 0) || (buffer == NULL) || (handle == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log3 ("MAILBOX: Mailbox_read Error! Invalid param. Size=%d Buffer=(%p) handle=(%p)\n", size, buffer, handle);
        retVal = MAILBOX_EINVAL;
    }
    else
    {    
        /* If the size is bigger than maximum mailbox buffer size, read/copy the max available size */
        if(size + driver->numBytesRead > MAILBOX_DATA_BUFFER_SIZE)
        {
            size = MAILBOX_DATA_BUFFER_SIZE - driver->numBytesRead;
        }
        
        /* First check if this is the first message because it will only block if this is a first message */
        if(driver->numBytesRead == 0)
        {            
            /* If mailbox read mode is "blocking", need to pend on semaphore*/
            if(driver->cfg.readMode == MAILBOX_MODE_BLOCKING)
            {
                /* Pend on semaphore until message arrives in mailbox ("mailbox_full" interrupt is received)*/
                status = SemaphoreP_pend (driver->readSem, driver->cfg.readTimeout);
                if (status == SemaphoreP_TIMEOUT)
                {
                    /* Set error code */
                    retVal = MAILBOX_EREADTIMEDOUT;

                    /* Report the error condition: */
                    DebugP_log2 ("MAILBOX:(%p) Mailbox_read timed out. Number of RX messages = %d.\n",
                                 driver, driver->rxCount);
                }
            }
            
            /* In polling or callback mode the newMessageFlag indicates that a message was received.
               In blocking mode, a posted semaphore indicates that a new message was received.
               If any of these are true, the driver should read the new message.*/
            if( ((driver->cfg.readMode == MAILBOX_MODE_POLLING)  && (driver->newMessageFlag == MAILBOX_NEW_MESSAGE_RECEIVED)) ||
                ((driver->cfg.readMode == MAILBOX_MODE_CALLBACK) && (driver->newMessageFlag == MAILBOX_NEW_MESSAGE_RECEIVED)) ||
                ((driver->cfg.readMode == MAILBOX_MODE_BLOCKING) && (status != SemaphoreP_TIMEOUT)) )
            {
                driver->newMessageFlag = MAILBOX_NEW_MESSAGE_NOT_RECEIVED;                
                /* Increment RX count */
                driver->rxCount++;
                
                /* Copy data from mailbox buffer into application buffer */
                if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
                {
                    if((driver->remoteEndpoint != MAILBOX_TYPE_BSS) && (driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
                    {
                        /*Read message. Need to account for internal header*/
                        memcpy ((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead + MAILBOX_MULTI_CH_HEADER_SIZE), size); 
                    }
                    else
                    {
                        /*Read message.*/
                        memcpy ((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead), size); 
                    }    
                }
                else
                {
                    DebugP_log0 ("MAILBOX: Mailbox_read Error! Only memcpy dataTransferMode is supported\n");
                    retVal = MAILBOX_EINVALCFG;
                    goto exit;
                }

                /* Set return value */
                retVal = (int32_t)size;
                
                /* Update number of bytes read for this message*/
                driver->numBytesRead = driver->numBytesRead + size;
            }    
        }
        else
        {
            /* This is a subsequent read for a message that is already in the mailbox buffer */            
            /* Copy data from mailbox buffer into application buffer */
            if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
            {
                if((driver->remoteEndpoint != MAILBOX_TYPE_BSS) && (driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
                {
                    /*Read message. Need to account for internal header*/
                    memcpy ((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead + MAILBOX_MULTI_CH_HEADER_SIZE), size); 
                }
                else
                {
                    /*Read message.*/
                    memcpy ((void *)buffer, (void *)((uint8_t *)((driver->hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead), size); 
                }    
            }
            else
            {
                DebugP_log0 ("MAILBOX: Mailbox_read Error! Only memcpy dataTransferMode is supported\n");
                retVal = MAILBOX_EINVALCFG;
                goto exit;
            }
            
            /* Set return value */
            retVal = (int32_t)size;
            
            /* Update number of bytes read for this message*/
            driver->numBytesRead = driver->numBytesRead + size;
        }
    }
exit:    
    return retVal;
}


/*
 *  ======== Mailbox_readFlush ========
 */
int32_t Mailbox_readFlush(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    int32_t             retVal = 0;
    uintptr_t           key;
    

    /* Sanity Check: Validate the arguments */
    if (handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0 ("MAILBOX: Mailbox_readFlush Error! Null handle\n");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        driver = (Mailbox_Driver*)handle;

        /* If this is MSS-DSS multi-channel, make sure mailbox is not being used by another instance */
        if((driver->remoteEndpoint != MAILBOX_TYPE_BSS) && (driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
        {
            /* Critical Section Protection*/
            key = HwiP_disable();
            if(gMailboxMCB.mssDssReadChIDInUse != driver->cfg.chId)
            {
                retVal = MAILBOX_ECHINUSE;
                /* Release the critical section: */
                HwiP_restore(key);
                goto exit;
            }
            else
            {
                /* Indicate that channel is no longer in use */
                gMailboxMCB.mssDssReadChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
                /* Release the critical section: */
                HwiP_restore(key);
            }
        }
        
        driver->readFlushCount++;
        
        /* Reset number of bytes read*/
        driver->numBytesRead = 0;
            
        if(driver->hwCfg != NULL)
        {
            /* Send acknowledgement to remote endpoint */
            ((driver->hwCfg)->baseLocalToRemote.reg)->INT_TRIG = CSL_FINSR (((driver->hwCfg)->baseLocalToRemote.reg)->INT_TRIG, 1U, 1U, 1U);
        }
        else
        {
            /* Error: Invalid Arguments */
            DebugP_log0 ("MAILBOX: Mailbox_readFlush Error! Null hardware configuration.\n");
            retVal = MAILBOX_EINVAL;
        }
    }    

exit:    
    return retVal;    
}    

/*
 *  ======== Mailbox_write ========
 */
int32_t Mailbox_write(Mbox_Handle handle, const uint8_t *buffer, uint32_t size)
{
    Mailbox_Driver*     driver;
    SemaphoreP_Status   status;
    int32_t             retVal = 0;
    uint32_t            header = 0;
    uintptr_t           key;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if ((size == 0) || (size > MAILBOX_DATA_BUFFER_SIZE) || (buffer == NULL) || (handle == NULL) || (driver->hwCfg == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log4 ("MAILBOX: Mailbox_write Error! Invalid param. Size=%d Buffer=(%p) handle=(%p) hwCfgPtr=(%p)\n", size, buffer, handle, driver->hwCfg);
        retVal = MAILBOX_EINVAL;
        goto exit;
    }
    
    if((driver->remoteEndpoint != MAILBOX_TYPE_BSS) && (driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
    {
        /* Critical Section Protection*/
        key = HwiP_disable();
        if(gMailboxMCB.mssDssWriteChIDInUse == MAILBOX_UNUSED_CHANNEL_ID)
        {
            /* Mark that the TX mailbox is now in use*/
            gMailboxMCB.mssDssWriteChIDInUse = driver->cfg.chId;
            /* Release the critical section: */
            HwiP_restore(key);
        }
        else
        {
            /* Error: TX mailbox is being used by another mailbox instance*/
            DebugP_log2 ("MAILBOX: Mailbox_write Error! handle=(%p). Write attempt with TX box in use by channel ID %d\n",driver, gMailboxMCB.mssDssWriteChIDInUse);
            retVal = MAILBOX_ECHINUSE;
            /* Release the critical section: */
            HwiP_restore(key);
            goto exit;
        }
    }    
    
    if(driver->txBoxStatus == MAILBOX_TX_BOX_FULL)
    {
        /* Error: TX mailbox is full, can not write new message until acknowledge is received from remote endpoint */
        /* Note that this should take care that the DMA has been completed as well because this flag is cleaned only after
           copy is done */
        DebugP_log1 ("MAILBOX: Mailbox_write Error! handle=(%p). Write attempt with txBoxStatus == MAILBOX_TX_BOX_FULL\n", handle);
        retVal = MAILBOX_ETXFULL;
        goto exit;
    }
    
    /* A write operation is starting, need to set TXbox flag to full to block any other write to this instance of mailbox*/
    driver->txBoxStatus = MAILBOX_TX_BOX_FULL;
    
    /* Copy data from application buffer to mailbox buffer */
    if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
    {
        if((driver->remoteEndpoint != MAILBOX_TYPE_BSS) && (driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
        {
            /*Write internal header*/
            header = driver->cfg.chId;
            memcpy ((void *)(driver->hwCfg)->baseLocalToRemote.data, (void *)(&header), sizeof(header));
            /*Write message. Need to account for internal header size*/
            memcpy ((void *)((uint8_t *)((driver->hwCfg)->baseLocalToRemote.data) + MAILBOX_MULTI_CH_HEADER_SIZE), (const void *)buffer, size);
        }
        else
        {
            /*Write message.*/
            memcpy ((void *)(driver->hwCfg)->baseLocalToRemote.data, (const void *)buffer, size);
        }
        
        #ifdef SUBSYS_MSS
        MEM_BARRIER();
        #endif
    }
    else
    {
        DebugP_log0 ("MAILBOX: Mailbox_write Error! Only memcpy dataTransferMode is supported\n");
        retVal = MAILBOX_EINVALCFG;
        goto exit;
    }
    
    /* Store handle of instance in case this is a MSS/DSS driver instance*/
    if(driver->remoteEndpoint != MAILBOX_TYPE_BSS)
    {
        gMailboxMCB.lastMsgSentHandle = handle;
    }
    
    /* Trigger "mailbox full" interrupt to remote endpoint*/
    ((driver->hwCfg)->baseLocalToRemote.reg)->INT_TRIG = CSL_FINSR (((driver->hwCfg)->baseLocalToRemote.reg)->INT_TRIG, 0U, 0U, 1U);
    
    /* Next action depends on the mailbox write mode*/
    if(driver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
    {
        /* Pend on semaphore until acknowledge ("mailbox_empty" interrupt) from remote endpoint is received*/
        status = SemaphoreP_pend (driver->writeSem, driver->cfg.writeTimeout);
        if (status == SemaphoreP_TIMEOUT)
        {
            /* Set error code */
            retVal = MAILBOX_ETXACKTIMEDOUT;

            /* Report the error condition: */
            DebugP_log2 ("MAILBOX:(%p) Write acknowledge timed out. Ack was never received. Number of received TX messages = %d.\n",
                         driver, driver->txCount);
        }
    }
    
    /* If write is blocking mode and semaphore did not timeout, write succeeded and ack received.
       If write is polling mode and we reached this point, write was done but not sure if ack has been received. */
    if( ((driver->cfg.writeMode == MAILBOX_MODE_BLOCKING) && (status != SemaphoreP_TIMEOUT)) ||
        (driver->cfg.writeMode == MAILBOX_MODE_POLLING) )
    {        
        /* Increment TX count */
        driver->txCount++;
        
        /* Set return value */
        retVal = (int32_t)size;            
    }
    
exit:    
    return retVal;
}

/*
 *  ======== Mailbox_writeReset ========
 *  Used when a write is never acknowledged by the receiver,
 *  do a writeReset so a new message can be sent.
 */
 
int32_t Mailbox_writeReset(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    int32_t             retVal = 0;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if ((handle == NULL) || (driver->hwCfg == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log0 ("MAILBOX: Mailbox_writeReset Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        /* make sure "mailbox empty" interrupt is clear */
        ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_ACK = CSL_FINSR (((driver->hwCfg)->baseRemoteToLocal.reg)->INT_ACK, 1U, 1U, 1U);

        /* Reset mailbox TX status to empty so a new message can be sent*/
        driver->txBoxStatus = MAILBOX_TX_BOX_EMPTY;

        if(driver->remoteEndpoint != MAILBOX_TYPE_BSS)
        {
            /* reset last msg handle in case this is a MSS/DSS driver instance*/
            gMailboxMCB.lastMsgSentHandle = NULL;
            /* update TX box multichannel status */
            gMailboxMCB.mssDssWriteChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
        }

    }

    return retVal;
}

/*
 *  ======== Mailbox_close ========
 */
int32_t Mailbox_close(Mbox_Handle handle)
{
    int32_t            retVal = 0;
    Mailbox_Driver*    driver;
    uintptr_t          key;

    /* Get the mailbox driver */
    driver = (Mailbox_Driver*)handle;

    if (handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0 ("MAILBOX: Mailbox_close Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    else
    {

        if(driver->remoteEndpoint == MAILBOX_TYPE_BSS)
        {
            gMailboxMCB.bssInstCount = 0;
            
            /* Was the Full HWI registered?  */
            if (gMailboxMCB.hwiHandles.bssFull)
            {
                /* YES: Delete and unregister the interrupt handler. */
                HwiP_delete(gMailboxMCB.hwiHandles.bssFull);
                gMailboxMCB.hwiHandles.bssFull = NULL;
                gMailboxMCB.errCnt.bssFull = 0;
            }

            /* Was the Empty HWI registered?  */
            if (gMailboxMCB.hwiHandles.bssEmpty)
            {
                /* YES: Delete and unregister the interrupt handler. */
                HwiP_delete(gMailboxMCB.hwiHandles.bssEmpty);
                gMailboxMCB.hwiHandles.bssEmpty = NULL;
                gMailboxMCB.errCnt.bssEmpty = 0;
            }
        }
        else
        {
            /* Critical Section Protection: mss/dss close needs to be
             * protected against multiple threads */
            key = HwiP_disable();
            /* Reset the array for this instance of the driver */
            gMailboxMCB.handleArray[driver->cfg.chId] = NULL;

            gMailboxMCB.mssDssInstCount--;
            
            /*Unregister ISRs if there are no more instances to MSS/DSS*/
            if(gMailboxMCB.mssDssInstCount == 0)
            {
                /* Was the Full HWI registered?  */
                if (gMailboxMCB.hwiHandles.mssDssFull)
                {
                    /* YES: Delete and unregister the interrupt handler. */
                    HwiP_delete(gMailboxMCB.hwiHandles.mssDssFull);
                    gMailboxMCB.hwiHandles.mssDssFull = NULL;
                    gMailboxMCB.errCnt.mssDssFull = 0;
                }

                /* Was the Empty HWI registered?  */
                if (gMailboxMCB.hwiHandles.mssDssEmpty)
                {
                    /* YES: Delete and unregister the interrupt handler. */
                    HwiP_delete(gMailboxMCB.hwiHandles.mssDssEmpty);
                    gMailboxMCB.hwiHandles.mssDssEmpty = NULL;
                    gMailboxMCB.errCnt.mssDssEmpty = 0;
                }
            }
            /* Release the critical section: */
            HwiP_restore(key);            
        }

        /* Was the driver operating in Write Blocking mode? */
        if (driver->writeSem)
        {
            /* YES: Delete the write semaphore */
            SemaphoreP_delete (driver->writeSem);
        }

        /* Was the driver operating in Read Blocking mode? */
        if (driver->readSem)
        {
            /* YES: Delete the read semaphore */
            SemaphoreP_delete (driver->readSem);
        }

        /* Cleanup the memory: */
        MemoryP_ctrlFree (driver, (uint32_t)sizeof(Mailbox_Driver));
    }
    
    return retVal;
}

/*
 *  ======== Mailbox_getStats ========
 */
int32_t Mailbox_getStats(Mbox_Handle handle, Mailbox_Stats * stats)
{
    int32_t             retVal = 0;
    Mailbox_Driver*     driver;

    /* Get the mailbox driver */
    driver = (Mailbox_Driver*)handle;

    if (handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0 ("MAILBOX: Mailbox_getStats Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    else if (stats == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0 ("MAILBOX: Mailbox_getStats Error! Null pointer");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        stats->txBoxStatus         = (uint32_t) driver->txBoxStatus;  
        stats->txCount             = driver->txCount;  
        stats->rxCount             = driver->rxCount;  
        stats->newMessageFlag      = (uint32_t) driver->newMessageFlag;
        stats->numBytesRead        = driver->numBytesRead;
        stats->boxFullIsrCount     = driver->boxFullIsrCount;  
        stats->boxEmptyIsrCount    = driver->boxEmptyIsrCount;  
        stats->readFlushCount      = driver->readFlushCount;  
        stats->boxEmptyIsrErrCount = gMailboxMCB.errCnt.bssEmpty;  
        stats->boxFullIsrErrCount  = gMailboxMCB.errCnt.bssFull;  
        stats->regLclToRemRaw      = (uint32_t)(((driver->hwCfg)->baseLocalToRemote.reg)->INT_STS_RAW); 
        stats->regRemToLocRaw      = (uint32_t)(((driver->hwCfg)->baseRemoteToLocal.reg)->INT_STS_RAW); 
        stats->regLclToRemMask     = (uint32_t)(((driver->hwCfg)->baseLocalToRemote.reg)->INT_MASK); 
        stats->regRemToLocMask     = (uint32_t)(((driver->hwCfg)->baseRemoteToLocal.reg)->INT_MASK); 
    }
    
    return retVal;
}

/*************** Driver Private APIs below *******************/


/**
 * @brief
 *  Global Variable to store hardware base addresses and interrupt numbers.
 *  It is populated in the platform specific files.
 */
#ifdef SOC_XWR14XX
/*local endpoint MSS*/
extern Mailbox_HwCfg   gMailboxMssBssHwCfg;
#else
#ifdef SUBSYS_MSS
/*local endpoint MSS*/
extern Mailbox_HwCfg   gMailboxMssBssHwCfg;
/*local endpoint MSS*/
extern Mailbox_HwCfg   gMailboxMssDssHwCfg;
#else
/*local endpoint DSS*/
extern Mailbox_HwCfg   gMailboxDssMssHwCfg;
/*local endpoint DSS*/
extern Mailbox_HwCfg   gMailboxDssBssHwCfg;
#endif
#endif

/**
 *  @b Description
 *  @n
 *      The function configures the hardware base addresses and interrupt numbers
 *      It populates the driver with hard coded information derived from the platform
 *      files. 
 *
 *  @param[in]  driver handle
 *      
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 *
 */
static void Mailbox_hwCfg(Mailbox_Driver* driver)
{
    #ifdef SOC_XWR14XX
    /*This is XWR14xx*/
    driver->hwCfg = &gMailboxMssBssHwCfg;
    #else
    /*This is XWR16xx/XWR18xx/XWR68xx*/
    #ifdef SUBSYS_MSS
    /*This is XWR16xx/XWR18xx/XWR68xx local endpoint MSS*/
    if(driver->remoteEndpoint == MAILBOX_TYPE_BSS)
    {
        driver->hwCfg = &gMailboxMssBssHwCfg;
    }
    else /*remote endpoint is DSS */
    {
        driver->hwCfg = &gMailboxMssDssHwCfg;
        gMailboxMCB.mssDssHwCfgPtr = &gMailboxMssDssHwCfg; 
    }
    #else
    /* This is XWR16xx/XWR18xx/XWR68xx local endpoint DSS */
    if(driver->remoteEndpoint == MAILBOX_TYPE_BSS)
    {
        driver->hwCfg = &gMailboxDssBssHwCfg;
    }
    else /*remote endpoint is MSS */
    {
        driver->hwCfg = &gMailboxDssMssHwCfg;
        gMailboxMCB.mssDssHwCfgPtr = &gMailboxDssMssHwCfg; 
    }
    #endif
    #endif
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt.
 *
 *  @param[in]  driver
 *      Driver handle
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISRProcessing(Mailbox_Driver* driver)
{
    if(driver != NULL)
    {
        /* Clear the status register */
        if(driver->hwCfg != NULL)
		{
            ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_ACK = CSL_FINSR (((driver->hwCfg)->baseRemoteToLocal.reg)->INT_ACK, 0U, 0U, 1U);
		}	

        driver->boxFullIsrCount++;
        
        /* set flag to inform that new message received */
        driver->newMessageFlag = MAILBOX_NEW_MESSAGE_RECEIVED;

        /* Action depends on the read mode */
        if(driver->cfg.readMode == MAILBOX_MODE_POLLING)
        {
            /* Do nothing. Flag set above is enough */
        }
        else if(driver->cfg.readMode == MAILBOX_MODE_BLOCKING)
        {
            /* Post semaphore */
            if (driver->readSem)
            {
                /* Post the semaphore to unblock calling thread. */
                SemaphoreP_postFromISR (driver->readSem);
            }
        }
        else /* Call back read mode */
        {  
            /* Check if call back function is implemented */
            if(driver->cfg.readCallback != NULL)
            {
                (*(driver->cfg.readCallback))(driver, driver->remoteEndpoint);
            }
        }
    }    
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt for the BSS remote endpoint.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISRBss (uintptr_t arg)
{
    /* Sanity Check: Validate the arguments */
    if (arg != NULL)
    {
        Mailbox_boxFullISRProcessing((Mailbox_Driver*)arg);
    }
    else
    {
        gMailboxMCB.errCnt.bssFull++;
    }

    return;
}

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt for the MSS/DSS remote endpoint.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISRMssDss (uintptr_t arg)
{
    Mbox_Handle   handle = NULL;
    uint32_t         header;
    uint8_t          id = MAILBOX_UNUSED_CHANNEL_ID;
    Mailbox_Driver*  driver;
    
    if(gMailboxMCB.mssDssChType == MAILBOX_CHTYPE_SINGLE)
    {
        /* Sanity Check: Validate the arguments */
        if (arg != NULL)
        {
            Mailbox_boxFullISRProcessing((Mailbox_Driver*)arg);
        }
        else
        {
            gMailboxMCB.errCnt.mssDssFull++;
        }
    }
    else
    {
        /* First need to find the ID in the received message*/
        if(gMailboxMCB.mssDssHwCfgPtr != NULL)
        {
            memcpy ((void *)&header, (void *)(gMailboxMCB.mssDssHwCfgPtr->baseRemoteToLocal.data), sizeof(header)); 
            id = (uint8_t)(header & 0x7U);
        }    
        
        if(id > MAILBOX_CH_ID_MAX)
        {
            /*error*/
            handle = NULL;
        }
        else
        {
            handle = gMailboxMCB.handleArray[id];
        }
            
        /*First need to recover the driver instance for the last message that was sent*/
        if (handle != NULL)
        {
            driver = (Mailbox_Driver*)handle;
            /*Mark the channel as used so other instances can not read_flush it.*/
            if((driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
            {
                gMailboxMCB.mssDssReadChIDInUse = driver->cfg.chId;
            }    

            Mailbox_boxFullISRProcessing(driver);
        }
        else
        {
            gMailboxMCB.errCnt.mssDssFull++;
        }
    }    

    return;
}
#endif
/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  driver
 *      Driver handle.
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISRProcessing(Mailbox_Driver* driver)
{
    if(driver != NULL)
    {
        /* Acknowldedgement from remote endpoint has been received */
        /* Local endpoint clears the "mailbox empty" interrupt (bit 1 of register) */
        if(driver->hwCfg != NULL)
		{
            ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_ACK = CSL_FINSR (((driver->hwCfg)->baseRemoteToLocal.reg)->INT_ACK, 1U, 1U, 1U);
		}	

        driver->boxEmptyIsrCount++;
        
        /* update txBox status flag */
        driver->txBoxStatus = MAILBOX_TX_BOX_EMPTY;
        
        /* update TX box multichannel status */
        if((driver->remoteEndpoint != MAILBOX_TYPE_BSS) && (driver->cfg.chType == MAILBOX_CHTYPE_MULTI))
        {
            gMailboxMCB.mssDssWriteChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
        }    

        /* Action depends on the write mode */
        if(driver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
        {
            if (driver->writeSem)
            {
                /* Post the semaphore to unblock calling thread. */
                SemaphoreP_postFromISR (driver->writeSem);
            }
        }
    }    
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt for BSS remote endpoint.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISRBss (uintptr_t arg)
{
    /* Sanity Check: Validate the arguments */
    if (arg != NULL)
    {
        Mailbox_boxEmptyISRProcessing((Mailbox_Driver*)arg);
    }
    else
    {
        gMailboxMCB.errCnt.bssEmpty++;
    }

    return;
}
#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt for MSS/DSS remote endpoint.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISRMssDss (uintptr_t arg)
{
    /*First need to recover the driver instance for the last message that was sent*/
    if (gMailboxMCB.lastMsgSentHandle != NULL)
    {
        Mailbox_boxEmptyISRProcessing((Mailbox_Driver*)gMailboxMCB.lastMsgSentHandle);
    }
    else
    {
        gMailboxMCB.errCnt.mssDssEmpty++;
    }

    return;
}
#endif
