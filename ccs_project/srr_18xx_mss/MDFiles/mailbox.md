# MAILBOX

```C
#include <ti/drivers/mailbox/mailbox.h>
```

## Initialize Mailbox Drivers within the MSS subsystem

> The Mailbox initialization and configuration is deployed within the main system TASK ``` MSS_initTask ```

```C
Mailbox_Config 			mboxCfg;

Mailbox_init(MAILBOX_TYPE_MSS);

/* Create a binary semaphore to handle mailbox interrupt */
Semaphore_Params_init(&semParams);
semParams.mode           = Semaphore_Mode_BINARY;
gMSSMCB.mboxSemHandle    = Semaphore_create(0, &semParams, NULL);


/* Setup the default mailbox configuration
   Default VALUES
       readMode         = Mailbox_MODE_BLOCKING
       writeMode        = Mailbox_MODE_BLOCKING
       readTimeout      = MAILBOX_WAIT_FOREVER     
       writeTimeout     = MAILBOX_WAIT_FOREVER    
       readCallback     = NULL
       opMode           = MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED
       dataTransferMode = MAILBOX_DATA_TRANSFER_MEMCPY
       chType           = MAILBOX_CHTYPE_SINGLE
       chId             = MAILBOX_CH_ID_0   
 */
Mailbox_Config_init(&mboxCfg);

/* Setup the configuration */
mboxCfg.chType			= MAILBOX_CHTYPE_MULTI;
mboxCfg.chId			= MAILBOX_CH_ID_0;
mboxCfg.writeMode		= MAILBOX_MODE_BLOCKING;
mboxCfg.readMode		= MAILBOX_MODE_CALLBACK;
mboxCfg.readCallback	= &mboxCallbackFxn_MSS_ch0;

```

> ``` mboxCallbackFxn_MSS_ch0 ``` is callback function defined below in the same file scope. Implementation is below:
```C
void mboxCallbackFxn_MSS_ch0(Mbox_Handle handle, Mailbox_Type peer)
{
	Semaphore_post(gMSSMCB.mboxSemHandle);
}
```
``` C
/*
 *  Function is invoked for each received messege from the DSS peer endpoint.
 *  When invoked, release the resources and wakeup the mmWave thread to
 *  process the received message.
 *  \arg[0]  Handle to the Mailbox on which data was received
 *  \arg[1]  Peer from which data was received (DSS)
 *  \return  void
 *  Hence: the address of this function is passed to mboxCfg.readCallback
 *  readCallback is a pointer of a type, named Mailbox_Callback which is 
 *  a pointer to a function that takes a 2 inputs 
 *  handle: Which is returned by Mailbox_open
 *  and  Mailbox_Type which is could be MAILBOX_TYPE_MSS or MAILBOX_TYPE_DSS
 *  and returns nothing, which matches the signature of the function
 *  mboxCallbackFxn_MSS_ch0.
 *  To use it; we assign the function reference to the pointer readCallback 	
 *  The definition within the mailbox header file mailbox.h
 *  typedef void(* Mailbox_Callback) (Mbox_Handle handle, Mailbox_Type remoteEndpoint)
 */
```

## Open MAILBOX Channel 
``` C	
gMSSMCB.mboxHandle = Mailbox_open(MAILBOX_TYPE_DSS, &mboxCfg, &errCode);
```

## Create task to handle mailbox messages

> The Task ``` mboxIn_uartOut_TASK ``` is used to handle the recieved mailbox messages from the DSS Peer over the mailbox virtual communication channel.
```C
Task_Params_init(&taskParams);
taskParams.stackSize = 16*1024;
Task_create(mboxIn_uartOut_TASK, &taskParams, NULL);
```

> The task is common between UART, CAN and Mailbox drivers

> Refer to
[mboxRead_uartWrite.md](mboxRead_uartWrite.md)
