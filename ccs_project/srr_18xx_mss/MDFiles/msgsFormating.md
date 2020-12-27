# Messeges format for communication between MSS and DSS

>  The structure defines the message structure used to commuincate between MSS and DSS.


```C
/* Within the MSS */
MmwDemo_message      message;
```

> MmwDemo_message Structure is defined in ```  mmw_messages.h ```

```C
typedef struct MmwDemo_message_t
{
    MmwDemo_message_type		type;
    int8_t						subFrameNum;    
    MmwDemo_message_body      	body;

} MmwDemo_message;
```

>  The enum is used to hold all the messages types used for Mailbox communication between MSS and DSS
 ```C
typedef enum MmwDemo_message_type_e 
{
    MMWDEMO_MSS2DSS_GUIMON_CFG = 0xFEED0001,
    MMWDEMO_MSS2DSS_CFAR_RANGE_CFG,
    MMWDEMO_MSS2DSS_CFAR_DOPPLER_CFG,
    MMWDEMO_MSS2DSS_PEAK_GROUPING_CFG,
    MMWDEMO_MSS2DSS_MULTI_OBJ_BEAM_FORM,
    MMWDEMO_MSS2DSS_CALIB_DC_RANGE_SIG,
    MMWDEMO_MSS2DSS_DETOBJ_SHIPPED,
    MMWDEMO_MSS2DSS_SET_DATALOGGER,
    MMWDEMO_MSS2DSS_ADCBUFCFG,
    MMWDEMO_MSS2DSS_EXTENDED_MAX_VELOCITY,
    MMWDEMO_MSS2DSS_CLUTTER_REMOVAL,
    MMWDEMO_MSS2DSS_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE,
    MMWDEMO_MSS2DSS_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE,
    MMWDEMO_DSS2MSS_CONFIGDONE = 0xFEED0100,
    MMWDEMO_DSS2MSS_DETOBJ_READY,
    MMWDEMO_DSS2MSS_STOPDONE,
    MMWDEMO_DSS2MSS_ASSERT_INFO,
    MMWDEMO_DSS2MSS_ISR_INFO_ADDRESS,
    MMWDEMO_DSS2MSS_MEASUREMENT_INFO
}MmwDemo_message_type;
```


> The union defines the message body for various configuration messages to be passed from MSS to DSS

```C
typedef union MmwDemo_message_body_u 
{

    uint8_t						dataLogger;
    uint32_t  					dss2mssISRinfoAddress;
    MmwDemo_detInfoMsg			detObj;
    MmwDemo_dssAssertInfoMsg	assertInfo;
} MmwDemo_message_body;
```

> The structure defines the message body for reporting DSS assertion information on a DSS exception that should be forwarded to the MSS.

```C
typedef struct MmwDemo_dssAssertInfoMsg_t
{
    char     file[MMWDEMO_MAX_FILE_NAME_SIZE];
    uint32_t line;
} MmwDemo_dssAssertInfoMsg;
```

> The structure defines the message body for reporting detection information from data path to MSS.
```C
typedef struct MmwDemo_detObjMsg_t
{
    MmwDemo_output_		message_header header;
    MmwDemo_msgTlv		tlv[MMWDEMO_OUTPUT_MSG_MAX];
} MmwDemo_detInfoMsg;
```

> The structure describes the TLV part of the message from DSS to MSS on data path detection information.
```C
typedef struct MmwDemo_msgTlv_t
{
    uint32_t    type;
    uint32_t    length;
    uint32_t   address;
} MmwDemo_msgTlv;
```


## MOVING TO ANOTHER FILE

> MmwDemo_message Structure is defined in ``` mrr_output.h ```

> The structure defines the message header for reporting detection information from data path

 ```C
typedef struct MmwDemo_output_message_header_t
{
    uint16_t    magicWord[4];
    uint32_t    version;
    uint32_t    totalPacketLen;
    uint32_t    platform;
    uint32_t    frameNumber;
    uint32_t    timeCpuCycles;
    uint32_t    numDetectedObj;
    uint32_t    numTLVs;
    uint32_t    subFrameNumber;
} MmwDemo_output_message_header;
```

> The rest structures in the mrr_output.h file are used by the DSS.