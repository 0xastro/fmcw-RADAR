/*
 *   @file  spiTest.cpp
 *
 *   @brief
 *      Unit Test code for the PC FTDI SPI driver. It will work as a SPI master to 
 *  communicate with XWR1xxx device.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#pragma once
#include "stdio.h"
#include <tchar.h>
#include "windows.h"
#include "ftd2xx.h"

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/
/* Data Echo test defines */
#define COMMAND_LENGTH           12
#define MAGIC_NUMBER             0x3456
#define SPI_TEST_MSGLEN          128
#define SPI_TEST_SYNC_MSGLEN     16
#define SPI_TEST_LOOP            5000


/*Port A Pin config,either input (0) & output(1)*/
#define FT_PORTA_BIT0_SPICLK1                 (1)
#define FT_PORTA_BIT1_MOSI1                   (1<<1)
#define FT_PORTA_BIT2_MISO1                   (0<<2)
#define FT_PORTA_BIT3_CS1                     (1<<3)
#define FT_PORTA_BIT4_TSWCONN_MONITOR_END1    (0<<4)
#define FT_PORTA_BIT5_TSWCONN_MONITOR_END2    (0<<5)
#define FT_PORTA_BIT6_FTDI_PMIC_NRST          (1<<6)
#define FT_PORTA_BIT7_120PIN_TMPSNS_ALERT     (0<<7)

/* Default Port A configuration */
#define FT_PORTA_DIR_CONFIG                   (FT_PORTA_BIT0_SPICLK1 | FT_PORTA_BIT1_MOSI1 | \
                                               FT_PORTA_BIT2_MISO1 | FT_PORTA_BIT3_CS1 | \
                                               FT_PORTA_BIT4_TSWCONN_MONITOR_END1 | \
                                               FT_PORTA_BIT5_TSWCONN_MONITOR_END2 | \
                                               FT_PORTA_BIT7_120PIN_TMPSNS_ALERT )

/* FTDI BIT MODE settings */
#define FT_BITMODE_RESET		        0x00
#define FT_BITMODE_ASYNC_BITBANG		0x01
#define FT_BITMODE_MPSSE				0x02
#define FT_BITMODE_SYNC_BITBANG			0x04
#define FT_BITMODE_MCU_HOST				0x08
#define FT_BITMODE_FAST_SERIAL			0x10
#define FT_BITMODE_CBUS_BITBANG			0x20
#define FT_BITMODE_SYNC_FIFO			0x40

/* FTDI MPSSE Command */

/* FTDI MPSSE CPU MODE COMMAND */
#define FT_COMMAND_ERROR_BAD_COMMAND    	    0xFA

/* FTDI DATA TRANSFER COMMAND */
#define FT_COMMAND_DATA_BYTESOUT_PVE_MSB    	0x10
#define FT_COMMAND_DATA_BYTESOUT_NVE_MSB    	0x11
#define FT_COMMAND_DATA_BITSOUT_PVE_MSB	     	0x12
#define FT_COMMAND_DATA_BITSOUT_NVE_MSB	     	0x13
#define FT_COMMAND_DATA_BYTESIN_PVE_MSB 	    0x20
#define FT_COMMAND_DATA_BYTESIN_NVE_MSB	0x24
#define FT_COMMAND_DATA_BITSIN_PVE_MSB      	0x22
#define FT_COMMAND_DATA_BITSIN_NVE_MSB      	0x26
#define FT_COMMAND_DATA_BYTESINOUT_PNVE_MSB 0x31
#define FT_COMMAND_DATA_BYTESINOUT_NPVE_MSB 0x34
#define FT_COMMAND_DATA_BITSINOUT_PNVE_MSB  	0x33
#define FT_COMMAND_DATA_BITSINOUT_NPVE_MSB  	0x36
#define FT_COMMAND_DATA_BYTESOUT_PVE_LSB    	0x18
#define FT_COMMAND_DATA_BYTESOUT_NVE_LSB    	0x19
#define FT_COMMAND_DATA_BITSOUT_PVE_LSB     	0x1A
#define FT_COMMAND_DATA_BITSOUT_NVE_LSB	 	    0x1B
#define FT_COMMAND_DATA_BYTESIN_PVE_LSB 	    0x28
#define FT_COMMAND_DATA_BYTESIN_NVE_LSB     	0x2C
#define FT_COMMAND_DATA_BITSIN_PVE_LSB      	0x2A
#define FT_COMMAND_DATA_BITSIN_NVE_LSB      	0x2E
#define FT_COMMAND_DATA_BYTESINOUT_PNVE_LSB 0x39
#define FT_COMMAND_DATA_BYTESINOUT_NPVE_LSB 0x3C
#define FT_COMMAND_DATA_BITSINOUT_PNVE_LSB  	0x3B
#define FT_COMMAND_DATA_BITSINOUT_NPVE_LSB  	0x3E

/* FTDI DATA DIRECTION COMMAND */
#define FT_COMMAND_SET_DATABITS_LOW         	0x80
#define FT_COMMAND_SET_DATABITS_HIGH        	0x82
#define FT_COMMAND_READ_DATABITS_LOW        	0x81
#define FT_COMMAND_READ_DATABITS_HIGH       	0x83

/* FTDI LOOPBACK MODE COMMAND */
#define FT_COMMAND_TDI_TO_TDO_LOOPBACK      	0x84
#define FT_COMMAND_TDO_TO_TDI_LOOPBACK  	    0x85

/* FTDI CLOCK DIVISOR COMMAND */
#define FT_COMMAND_CLOCK_DIVISOR            	0x86

/* Read one byte from target device */
#define FT_COMMAND_CPUMODE_READ_SHORT_ADDR  0x90
#define FT_COMMAND_CPUMODE_READ_EXT_ADDR	    0x91
#define FT_COMMAND_CPUMODE_WRITE_SHORT_ADDR 0x92
#define FT_COMMAND_CPUMODE_WRITE_EXT_ADDR   	0x93

/* FTDI MPSSE HOST EMULATION MODES COMMAND */
#define FT_COMMAND_FLUSH_BUFFER             	0x87
#define FT_COMMAND_WAIT_IOHIGH              	0x88
#define FT_COMMAND_WAIT_IOLOW               	0x89

/* FTDI MPSSE CLOCK COMMAND */
#define FT_COMMAND_CLOCK_DIVIDE5_DISABLE    	0x8A
#define FT_COMMAND_CLOCK_DIVIDE5_ENABLE     	0x8B
#define FT_COMMAND_CLOCK_3PHASE_ENABLE      	0x8C
#define FT_COMMAND_CLOCK_3PHASE_DISABLE     	0x8D
#define FT_COMMAND_CLOCK_PULSE_ON           	0x8F
#define FT_COMMAND_CLOCK_CONT_IOHIGH        	0x94
#define FT_COMMAND_CLOCK_CONT_IOLOW         	0x95
#define FT_COMMAND_TURNON_ADAPTIVE_CLOCK    	0x96
#define FT_COMMAND_TURNOFF_ADAPTIVE_CLOCK		0x97
#define FT_COMMAND_CLOCK_PULSE_GPIO1_HIGH   	0x9C
#define FT_COMMAND_CLOCK_PULSE_GPIO1_LOW    	0x9D


/* FTDI Response Timeout */
#define FT_FTDI_RESPONSE_TIMEOUT                 (50)

typedef FT_STATUS(WINAPI *PtrToOpen)(PVOID, FT_HANDLE *);
typedef FT_STATUS(WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *);
typedef FT_STATUS(WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
typedef FT_STATUS(WINAPI *PtrToClose)(FT_HANDLE);
typedef FT_STATUS(WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
typedef FT_STATUS(WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
typedef FT_STATUS(WINAPI *PtrToResetDevice)(FT_HANDLE);
typedef FT_STATUS(WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
typedef FT_STATUS(WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
typedef FT_STATUS(WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
typedef FT_STATUS(WINAPI *PtrToSetBitMode)(FT_HANDLE, UCHAR, UCHAR);
typedef FT_STATUS(WINAPI *PtrToSetUSBParameters)(FT_HANDLE, ULONG, ULONG);
typedef FT_STATUS(WINAPI *PtrToSetLatencyTimer)(FT_HANDLE, UCHAR);

/**************************************************************************
 *************************** Local Data Structure ******************************
 **************************************************************************/

/* SPI test master control block */
typedef struct spiTestMCB
{
	HMODULE m_hmodule;

	FT_HANDLE m_ftHandle;

	PtrToOpen m_pOpen;

	PtrToOpenEx m_pOpenEx;

	PtrToListDevices m_pListDevices;

	PtrToClose m_pClose;

	PtrToRead m_pRead;

	PtrToWrite m_pWrite;

	PtrToResetDevice m_pResetDevice;

	PtrToPurge m_pPurge;

	PtrToSetTimeouts m_pSetTimeouts;

	PtrToGetQueueStatus m_pGetQueueStatus;

	PtrToSetBitMode  m_pSetBitMode;

	PtrToSetUSBParameters m_pSetUSBParameters;

	PtrToSetLatencyTimer m_pSetLatencyTimer;

	unsigned int board_present;

}spiTestMCB;

/* SPI test message format */
typedef struct spiTestMsg
{
	unsigned short magicNumber;
	unsigned short seqNo;
	unsigned short dataLen;
	unsigned short checksum;
	unsigned char data[SPI_TEST_MSGLEN - 8];
}spiTestMsg;

/**************************************************************************
 *************************** Global Definitions ****************************
 **************************************************************************/

/* Global test master control blcok */
spiTestMCB       spiTestMcb;

/* Global test sequence number */
unsigned short   gTestMsgSeqNo = 0;

/* FTDI device id for SPI */
unsigned short   gSPIDevId = 0;

/**************************************************************************
 *************************** Local Function Prototype***************************
 **************************************************************************/
void Loadem(unsigned int);
FT_STATUS Write(LPVOID, DWORD, LPDWORD);
FT_STATUS OpenBy(unsigned int);
void LoadDLL();
FT_STATUS Open(PVOID);
FT_STATUS OpenEx(PVOID, DWORD);
FT_STATUS ListDevices(PVOID, PVOID, DWORD);
FT_STATUS Close();
FT_STATUS Read(LPVOID, DWORD, LPDWORD);
FT_STATUS ResetDevice();
FT_STATUS Purge(ULONG);
FT_STATUS SetTimeouts(ULONG, ULONG);
FT_STATUS GetQueueStatus(LPDWORD);
FT_STATUS SetBitMode(UCHAR, UCHAR);
FT_STATUS SetUSBParameters(ULONG, ULONG);
FT_STATUS SetLatencyTimer(UCHAR);


/**
 *  @b Description
 *  @n
 *      This function Loads FTDI driver dll file.
 *
 *  @retval
 *      Not Applicable.
 */
void LoadDLL(void)
{
	spiTestMcb.m_hmodule = LoadLibrary(_T("ftd2xx.dll"));
	if (spiTestMcb.m_hmodule == NULL)
	{
		printf("Error: Can't Load Ftd2xx.dll");
		return;
	}

	spiTestMcb.m_pWrite = (PtrToWrite)GetProcAddress(spiTestMcb.m_hmodule, "FT_Write");
	if (spiTestMcb.m_pWrite == NULL)
	{
		printf("Error: Can't Find FT_Write");
		return;
	}

	spiTestMcb.m_pRead = (PtrToRead)GetProcAddress(spiTestMcb.m_hmodule, "FT_Read");
	if (spiTestMcb.m_pRead == NULL)
	{
		printf("Error: Can't Find FT_Read");
		return;
	}

	spiTestMcb.m_pOpen = (PtrToOpen)GetProcAddress(spiTestMcb.m_hmodule, "FT_Open");
	if (spiTestMcb.m_pOpen == NULL)
	{
		printf("Error: Can't Find FT_Open");
		return;
	}

	spiTestMcb.m_pOpenEx = (PtrToOpenEx)GetProcAddress(spiTestMcb.m_hmodule, "FT_OpenEx");
	if (spiTestMcb.m_pOpenEx == NULL)
	{
		printf("Error: Can't Find FT_OpenEx");
		return;
	}

	spiTestMcb.m_pListDevices = (PtrToListDevices)GetProcAddress(spiTestMcb.m_hmodule, "FT_ListDevices");
	if (spiTestMcb.m_pListDevices == NULL)
	{
		printf("Error: Can't Find FT_ListDevices");
		return;
	}

	spiTestMcb.m_pClose = (PtrToClose)GetProcAddress(spiTestMcb.m_hmodule, "FT_Close");
	if (spiTestMcb.m_pClose == NULL)
	{
		printf("Error: Can't Find FT_Close");
		return;
	}

	spiTestMcb.m_pResetDevice = (PtrToResetDevice)GetProcAddress(spiTestMcb.m_hmodule, "FT_ResetDevice");
	if (spiTestMcb.m_pResetDevice == NULL)
	{
		printf("Error: Can't Find FT_ResetDevice");
		return;
	}

	spiTestMcb.m_pPurge = (PtrToPurge)GetProcAddress(spiTestMcb.m_hmodule, "FT_Purge");
	if (spiTestMcb.m_pPurge == NULL)
	{
		printf("Error: Can't Find FT_Purge");
		return;
	}

	spiTestMcb.m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(spiTestMcb.m_hmodule, "FT_SetTimeouts");
	if (spiTestMcb.m_pSetTimeouts == NULL)
	{
		printf("Error: Can't Find FT_SetTimeouts");
		return;
	}

	spiTestMcb.m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(spiTestMcb.m_hmodule, "FT_GetQueueStatus");
	if (spiTestMcb.m_pGetQueueStatus == NULL)
	{
		printf("Error: Can't Find FT_GetQueueStatus");
		return;
	}

	spiTestMcb.m_pSetBitMode = (PtrToSetBitMode)GetProcAddress(spiTestMcb.m_hmodule, "FT_SetBitMode");
	if (spiTestMcb.m_pSetBitMode == NULL)
	{
		printf("Error: Can't Find FT_SetBitMode");
		return;
	}

	spiTestMcb.m_pSetUSBParameters = (PtrToSetUSBParameters)GetProcAddress(spiTestMcb.m_hmodule, "FT_SetUSBParameters");
	if (spiTestMcb.m_pSetUSBParameters == NULL)
	{
		printf("Error: Can't Find FT_SetUSBParameters");
		return;
	}

	spiTestMcb.m_pSetLatencyTimer = (PtrToSetLatencyTimer)GetProcAddress(spiTestMcb.m_hmodule, "FT_SetLatencyTimer");
	if (spiTestMcb.m_pSetLatencyTimer == NULL)
	{
		printf("Error: Can't Find FT_SetLatencyTimer");
		return;
	}
}

/**
 *  @b Description
 *  @n
 *      This function set CS signal level
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS SetCSLevel(UCHAR level)
{
	UCHAR buffer[12];
	DWORD sendBytes = 0;

	if (!spiTestMcb.m_pWrite)
	{
		printf("FT_Write is not valid!");
		return FT_INVALID_HANDLE;
	}
	
	/* Set Data bits LowByte */
	buffer[0] = FT_COMMAND_SET_DATABITS_LOW;
	/* Set CS low */
	if (level == 0)
		buffer[1] = 0xC0;
	else
		buffer[1] = 0xC8;
	
	/* Set Pin Direction */
	buffer[2] = FT_PORTA_DIR_CONFIG;

	/* Write to FTDI MPSSE */
	(*spiTestMcb.m_pWrite)(spiTestMcb.m_ftHandle, buffer, 3, &sendBytes);
	
	if (sendBytes != 3)
		return FT_IO_ERROR;
	
        return FT_OK;
}

#if 0    //Test code to send 2 bytes then toggle CS
/**
 *  @b Description
 *  @n
 *      This function reads data from FTDI Spi interface
 *
 *   @param[in] lpvBuffer                   Buffer to store the read data
 *   @param[in] dwBytesToRead         Number of bytes to read
 *   @param[in] lpdwBytesRead          Number of bytes actually read

 *  @retval
 *      FT Command Status.
 */
FT_STATUS Read(LPVOID lpvBuffer, DWORD dwBytesToRead, LPDWORD lpdwBytesRead)
{
	UCHAR   buffer[SPI_TEST_MSGLEN + COMMAND_LENGTH];
	DWORD   inBytes = 0;
	DWORD   outBytes = 0;
	DWORD   inDataAvail = 0;
	DWORD   i;
	FT_STATUS       status = FT_OK;
	DWORD   timeoutCnt = 0;
	UCHAR*  pInbuff = (UCHAR *)lpvBuffer;
	DWORD bytesToSend = 0;
	UCHAR dataLen = 2;

	if (!spiTestMcb.m_pRead)
	{
		printf("FT_Read is not valid!");
		return FT_INVALID_HANDLE;
	}
	
	*lpdwBytesRead = 0;
	for (i = 0; i < dwBytesToRead; i += dataLen, bytesToSend=0)
	{

		buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
		
		/* Set CS low */
  	        buffer[bytesToSend++] = 0xC0;

		/* Set Pin Direction */
		buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

		/* Data in only, bytes, clock in on rising edge */
		buffer[bytesToSend++] = FT_COMMAND_DATA_BYTESIN_PVE_MSB;
		
		/* Write Length LSB */
		buffer[bytesToSend++] = dataLen - 1;
		/* Write Length MSB */
		buffer[bytesToSend++] = 0;

		buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
		
		/* Set CS High */
		buffer[bytesToSend++] = 0xC8;

		/* Set Pin Direction */
		buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

		/* Send Immediate */
		buffer[bytesToSend++] = FT_COMMAND_FLUSH_BUFFER;
		
		status = (*spiTestMcb.m_pWrite)(spiTestMcb.m_ftHandle, buffer, bytesToSend, &outBytes);
		if (FT_OK != status)
		{
			printf("Error, couldn't send command to MSPEE \n");
			break;
		}

		do
		{
			/* Check if data is received */
			status = GetQueueStatus((DWORD *)&inDataAvail);
			Sleep(1);
			timeoutCnt++;
		} while ((FT_OK == status) && (inDataAvail < dataLen) &&
			(timeoutCnt < FT_FTDI_RESPONSE_TIMEOUT));

		if (FT_OK != status)
		{
			printf("Error: SPI Read Wait Data Available Timeout\n");
		}

		/* Read SPI data */
		status = (*spiTestMcb.m_pRead)(spiTestMcb.m_ftHandle, (LPVOID)(pInbuff + i), dataLen, &inBytes);
		*lpdwBytesRead = *lpdwBytesRead + inBytes;
		//Sleep(1);

	}

	return status;
}
#else //Test code that expecting CS hold on XWR1xxx so that it can tranfer 128 bytes with 1 CS togging
/**
 *  @b Description
 *  @n
 *      This function reads data from FTDI Spi interface
 *
 *   @param[in] lpvBuffer             Buffer to store the read data
 *   @param[in] dwBytesToRead         Number of bytes to read
 *   @param[in] lpdwBytesRead         Number of bytes actually read

 *  @retval
 *      FT Command Status.
 */
FT_STATUS Read(LPVOID lpvBuffer, DWORD dwBytesToRead, LPDWORD lpdwBytesRead)
{
	UCHAR   buffer[SPI_TEST_MSGLEN + COMMAND_LENGTH];
	DWORD   outBytes = 0;
	DWORD   inDataAvail = 0;
	DWORD   i;
	FT_STATUS  status = FT_OK;
	DWORD   timeoutCnt = 0;
	DWORD bytesToSend = 0;

	if (!spiTestMcb.m_pRead)
	{
		printf("FT_Read is not valid!");
		return FT_INVALID_HANDLE;
	}
	
	*lpdwBytesRead = 0;

	buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
	
	/* Set CS low */
    buffer[bytesToSend++] = 0xC0;

	/* Set Pin Direction */
	buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

	/* Data in only, bytes, clock in on rising edge */
	buffer[bytesToSend++] = FT_COMMAND_DATA_BYTESIN_PVE_MSB;
	
	/* Write Length LSB */
	buffer[bytesToSend++] = dwBytesToRead -1 ; //dataLen - 1;
	/* Write Length MSB */
	buffer[bytesToSend++] = 0;

	buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
	
	/* Set CS High */
	buffer[bytesToSend++] = 0xC8;

	/* Set Pin Direction */
	buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

	/* Send Immediate */
	buffer[bytesToSend++] = FT_COMMAND_FLUSH_BUFFER;
	
	status = (*spiTestMcb.m_pWrite)(spiTestMcb.m_ftHandle, buffer, bytesToSend, &outBytes);
	if (FT_OK != status)
	{
		printf("Error, couldn't send command to MSPEE \n");
	}
    else
	{
		do
		{
			/* Check if data is received */
			status = GetQueueStatus((DWORD *)&inDataAvail);
			timeoutCnt++;
		} while ((FT_OK == status) && (inDataAvail < dwBytesToRead) &&
			(timeoutCnt < FT_FTDI_RESPONSE_TIMEOUT));

		if (FT_OK != status)
		{
			printf("Error: SPI Read Wait Data Available Timeout\n");
		}

		/* Read SPI data */
		status = (*spiTestMcb.m_pRead)(spiTestMcb.m_ftHandle, lpvBuffer, dwBytesToRead, lpdwBytesRead);
   	}
Exit:
	return status;
}

#endif

#if 0 //Test code to send 2 bytes then toggle CS
/**
 *  @b Description
 *  @n
 *      This function writes data to FTDI Spi interface
 *
 *   @param[in] lpvBuffer                   Buffer to store the read data
 *   @param[in] dwBuffSize                Number of bytes to write
 *   @param[in] lpdwBytes                  Number of bytes actually write

 *  @retval
 *      FT Command Status.
 */
FT_STATUS Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	UCHAR buffer[SPI_TEST_MSGLEN + COMMAND_LENGTH];
	UCHAR *pInBuff = (UCHAR *)lpvBuffer;
	UCHAR i=0, bytesToSend=0, j;
	UCHAR dataLen = 2;

	if (!spiTestMcb.m_pWrite)
	{
		printf("FT_Write is not valid!");
		return FT_INVALID_HANDLE;
	}

	for (i = 0; i < dwBuffSize; i += dataLen, bytesToSend = 0)
	{
		buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
		
		/* Set CS low */
  	        buffer[bytesToSend++] = 0xC0;

		/* Set Pin Direction */
		buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

		/* write only in BYTES, data out on falling edge (-ve) */
		buffer[bytesToSend++] = FT_COMMAND_DATA_BYTESOUT_NVE_MSB;
		
		/* Write Length MSB */
		buffer[bytesToSend++] = dataLen - 1;
		
		/* Write Length LSB */
		buffer[bytesToSend++] = 0;

		/* Fill in Data in buffer */
		for (j = 0; j < dataLen; j++)
		{
			buffer[bytesToSend++] = pInBuff[j + i];
		}

		buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
		
		/* Set CS high */
		buffer[bytesToSend++] = 0xC8;

		/* Set Pin Direction */
		buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

		/* Send Immediate */
		buffer[bytesToSend++] = FT_COMMAND_FLUSH_BUFFER;

		/* Write to FTDI MPSSE */
		(*spiTestMcb.m_pWrite)(spiTestMcb.m_ftHandle, buffer, bytesToSend, lpdwBytes);

		/* Delay between CS toggling for every 2 bytes. Limitation of MibSPI on XWR1xxx */
		//Sleep(1);
	}

	return 0;
}
#else  //Test code that expecting CS hold on XWR1xxx so that it can tranfer 128 bytes with 1 CS togging
/**
 *  @b Description
 *  @n
 *      This function writes data to FTDI Spi interface
 *
 *   @param[in] lpvBuffer          Buffer to store the read data
 *   @param[in] dwBuffSize         Number of bytes to write
 *   @param[in] lpdwBytes          Number of bytes actually write

 *  @retval
 *      FT Command Status.
 */
FT_STATUS Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	UCHAR buffer[SPI_TEST_MSGLEN + COMMAND_LENGTH];
	UCHAR bytesToSend=0;

	if (!spiTestMcb.m_pWrite)
	{
		printf("FT_Write is not valid!");
		return FT_INVALID_HANDLE;
	}

	buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
	
	/* Set CS low */
    buffer[bytesToSend++] = 0xC0;

	/* Set Pin Direction */
	buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

	/* write only in BYTES, data out on falling edge (-ve) */
	buffer[bytesToSend++] = FT_COMMAND_DATA_BYTESOUT_NVE_MSB;
	
	/* Write Length LSB */
	buffer[bytesToSend++] = (dwBuffSize -1) & 0xFF; 
	/* Write Length MSB */
	buffer[bytesToSend++] = (dwBuffSize -1) >>8; 

	/* Fill in Data in buffer */
	memcpy((void *)(&buffer[bytesToSend]), lpvBuffer, dwBuffSize);

	bytesToSend += dwBuffSize;
	buffer[bytesToSend++] = FT_COMMAND_SET_DATABITS_LOW;
	
	/* Set CS high */
	buffer[bytesToSend++] = 0xC8;

	/* Set Pin Direction */
	buffer[bytesToSend++] = FT_PORTA_DIR_CONFIG;

	/* Send Immediate */
	buffer[bytesToSend++] = FT_COMMAND_FLUSH_BUFFER;

	/* Write to FTDI MPSSE */
	(*spiTestMcb.m_pWrite)(spiTestMcb.m_ftHandle, buffer, bytesToSend , lpdwBytes);
	
	return 0;
}

#endif

/**
 *  @b Description
 *  @n
 *      This function opens FTDI Spi device
 *
 *  @param[in] pvDevice                   FTDI SPI device id
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS Open(PVOID pvDevice)
{
	if (!spiTestMcb.m_pOpen)
	{
		printf("FT_Open is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pOpen)(pvDevice, &spiTestMcb.m_ftHandle);
}

/**
 *  @b Description
 *  @n
 *      This function opens FTDI Spi device
 *
 *  @param[in] pvDevice                   FTDI device handle 
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS OpenEx(PVOID pvDevice, DWORD dwFlags)
{
	if (!spiTestMcb.m_pOpenEx)
	{
		printf("FT_OpenEx is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pOpenEx)(pvDevice, dwFlags, &spiTestMcb.m_ftHandle);
}

/**
 *  @b Description
 *  @n
 *      This function lists available FTDI devices
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
	if (!spiTestMcb.m_pListDevices)
	{
		printf("FT_ListDevices is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pListDevices)(pArg1, pArg2, dwFlags);
}
/**
 *  @b Description
 *  @n
 *      This function Closes FTDI device
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS Close()
{
	if (!spiTestMcb.m_pClose)
	{
		printf("FT_Close is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pClose)(spiTestMcb.m_ftHandle);
}

/**
 *  @b Description
 *  @n
 *      This function resets FTDI device
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS ResetDevice()
{
	if (!spiTestMcb.m_pResetDevice)
	{
		printf("FT_ResetDevice is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pResetDevice)(spiTestMcb.m_ftHandle);
}



/**
 *  @b Description
 *  @n
 *      This function purges FTDI device transmission
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS Purge(ULONG dwMask)
{
	if (!spiTestMcb.m_pPurge)
	{
		printf("FT_Purge is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pPurge)(spiTestMcb.m_ftHandle, dwMask);
}

/**
 *  @b Description
 *  @n
 *      This function sets read/write timeout value for FTDI devices
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!spiTestMcb.m_pSetTimeouts)
	{
		printf("FT_SetTimeouts is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pSetTimeouts)(spiTestMcb.m_ftHandle, dwReadTimeout, dwWriteTimeout);
}

/**
 *  @b Description
 *  @n
 *      This function gets the status of the queue
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS GetQueueStatus(LPDWORD lpdwAmountInRxQueue)
{
	if (!spiTestMcb.m_pGetQueueStatus)
	{
		printf("FT_GetQueueStatus is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pGetQueueStatus)(spiTestMcb.m_ftHandle, lpdwAmountInRxQueue);
}

/**
 *  @b Description
 *  @n
 *      This function enable/disables bit mode
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS SetBitMode(UCHAR mask, UCHAR enable)
{
	if (!spiTestMcb.m_pSetBitMode)
	{
		printf("FT_SetBitMode is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pSetBitMode)(spiTestMcb.m_ftHandle, mask, enable);
}

/**
 *  @b Description
 *  @n
 *      This function sets USB parameters
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS SetUSBParameters(ULONG inTransferSize, ULONG outTransferSize)
{
	if (!spiTestMcb.m_pSetUSBParameters)
	{
		printf("FT_SetUSBParameters is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pSetUSBParameters)(spiTestMcb.m_ftHandle, inTransferSize, outTransferSize);
}

/**
 *  @b Description
 *  @n
 *      This function sets Latency timer value
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS SetLatencyTimer(UCHAR latency)
{
	if (!spiTestMcb.m_pSetLatencyTimer)
	{
		printf("FT_SetLatencyTimer is not valid!");
		return FT_INVALID_HANDLE;
	}

	return (*spiTestMcb.m_pSetLatencyTimer)(spiTestMcb.m_ftHandle, latency);
}

/**
 *  @b Description
 *  @n
 *      This function open FTDI SPI device by device number
 *
 *  @retval
 *      FT Command Status.
 */
FT_STATUS OpenBy(unsigned int devNo)
{
	FT_STATUS status;
	if ((devNo < 0) || (devNo > 15))
	{
		printf("Please enter a valid device number (0-15).");
		return FT_DEVICE_NOT_OPENED;
	}
	status = Open((PVOID)devNo);

	return status;
}


/**
 *  @b Description
 *  @n
 *      This function lists availabe  FTDI  devices
 *
 *  @retval
 *      FT Command Status.
 */
void Search(void)
{
	//search for Descriptions or Serial Numbers depending on the current mode
	FT_STATUS ftStatus;
	DWORD numDevs;
	DWORD index;
	char *BufDescription[64]; // pointer to array of 16 pointers
	char *BufSerialNumber[64];

    //must be closed to perform the ListDevices() function
	Close();

	ftStatus = ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
	if (ftStatus == FT_OK)
	{
		// FT_ListDevices OK, show number of devices
		printf("%d device(s) attached:\n", (int)numDevs);
		/* List all devices that are attached */
		for (index = 0; index < numDevs; index++)
		{
			BufDescription[index] = new char[64];
			BufSerialNumber[index] = new char[64];
		}

		BufDescription[index] = NULL;
		BufSerialNumber[index] = NULL;

		ftStatus = ListDevices(BufDescription, &numDevs, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION);
		ftStatus = ListDevices(BufSerialNumber, &numDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
		if (FT_SUCCESS(ftStatus))
		{
			/* List all devices that are attached */
			for (index = 0; index < numDevs; index++)
			{
				printf("Dev%d: %s - %s\n", index, BufSerialNumber[index], BufDescription[index]);
			}
		}
		else
		{
			printf("Error: List attached device failed: %d\n", ftStatus);
		}

		//free ram to avoid memory leaks
		for (index = 0; index < numDevs; index++)
		{
			delete BufDescription[index];
			delete BufSerialNumber[index];
		}
	}
	else
	{
		// FT_ListDevices failed
		printf("FT_ListDevices failed\n");
	}
}


/**
 *  @b Description
 *  @n
 *      This function opens SPI device and initializes SPI device
 *
 *  @retval
 *      FT Command Status.
 */
void LoadSPIDevice()
{
	unsigned char  txbuf[SPI_TEST_MSGLEN], rxbuf[SPI_TEST_MSGLEN];
	DWORD          ret_bytes;
	unsigned int   devNo;
	unsigned char  txConfBuffer[16];
	unsigned char  rxConfBuffer[16];
	DWORD          inBytes;
	DWORD          outBytes;
	spiTestMsg*    pTestMsgOut;
	spiTestMsg*    pTestMsgIn;
 
	Search();

	//open the device
	FT_STATUS status = OpenBy(gSPIDevId);
	if (status)
	{
		printf("Error: XWR1xxx board is not responding\n");
		spiTestMcb.board_present = 0;
	}
	else
	{
		printf("SPI device(%d) is opened, handle = %x\n", gSPIDevId, spiTestMcb.m_ftHandle);
		ResetDevice();

		/* Enable SPI Device */
		status |= SetBitMode(0, FT_BITMODE_RESET);
		if (status != FT_OK)
		{
			printf("MPSSE Init Error\n");
		}

		SetTimeouts(3000, 3000);//extend timeout while board DLP-USB2 finishes reset
		Purge(FT_PURGE_RX || FT_PURGE_TX);

		/* Set Usb Parameters*/
		status |= SetUSBParameters(4096, 4096);

		/* Set Latency Timer */
		status |= SetLatencyTimer(16);
		if (status != FT_OK)
		{
			printf("SPI USB parameters config Error\n");
		}
		/* Enable SPI Device */
		status |= SetBitMode(0, FT_BITMODE_MPSSE);
		if (status != FT_OK)
		{
			printf("MPSSE Init Error\n");
		}

		/* Use 60MHz master clock (disable divide by 5) */
		txConfBuffer[0] = FT_COMMAND_CLOCK_DIVIDE5_DISABLE;
		/* Turn off adaptive clocking (may be needed for ARM) */
		txConfBuffer[1] = FT_COMMAND_TURNOFF_ADAPTIVE_CLOCK;
		/* Disable three-phase clocking */
		txConfBuffer[2] = FT_COMMAND_CLOCK_3PHASE_DISABLE;
		/* Set divider command */
		txConfBuffer[3] = FT_COMMAND_CLOCK_DIVISOR;
		/* Divider low value . 60Mhz/(((high * 256+low)+1)*2)) ,
		   0xE -> 2MHz SPI clock
		   0x5 -> 5MHz
		   0x4 -> 6MHz
		   0x2 -> 10MHz		   
		   0x1 -> 15MHz
		   0x0 -> 30MHz
		*/
		txConfBuffer[4] = 0x4;
		/* Divider high value */
		txConfBuffer[5] = 0x00;
		/* Configure initial state of pins command: */
		txConfBuffer[6] = FT_COMMAND_SET_DATABITS_LOW;
		/* initial value: */
		txConfBuffer[7] = 0xC8;
		/* Direction: */
		txConfBuffer[8] = FT_PORTA_DIR_CONFIG;

		status = (*spiTestMcb.m_pWrite)(spiTestMcb.m_ftHandle, txConfBuffer, 9, &outBytes);
		if (status != FT_OK)
		{
			printf("MSPEE Config Error\n");
		}
		/* Clear Rx queue: */
		do
		{
			status = GetQueueStatus(&inBytes);
			if (FT_OK != status)
			{
				printf("Communication time out!\n");
				break;
			}
			(*spiTestMcb.m_pRead)(spiTestMcb.m_ftHandle, (LPVOID)&rxConfBuffer[0], inBytes, &outBytes);
		} while (inBytes > 0);

		//test for presence of XWR1xxx board
		pTestMsgOut = (spiTestMsg*)&txbuf[0];
		pTestMsgIn = (spiTestMsg*)&rxbuf[0];

		pTestMsgOut->magicNumber = MAGIC_NUMBER;
		pTestMsgOut->seqNo = gTestMsgSeqNo;
		pTestMsgOut->dataLen = SPI_TEST_SYNC_MSGLEN;
		pTestMsgOut->data[0] = SPI_TEST_LOOP & 0xff;
		pTestMsgOut->data[1] = (SPI_TEST_LOOP>>8) & 0xff;

		Write((LPVOID)pTestMsgOut, SPI_TEST_MSGLEN, &ret_bytes);
		
		Sleep(3000);
		while (1)
		{
			Sleep(3000);
			Read(pTestMsgIn, SPI_TEST_MSGLEN, &ret_bytes);
			Sleep(3000);
			if (pTestMsgIn->magicNumber != MAGIC_NUMBER)
			{
 		        printf("Incorrect magic number %x\n", pTestMsgIn->magicNumber);
				Write((LPVOID)pTestMsgOut, SPI_TEST_MSGLEN, &ret_bytes);
				
				continue;
			}
			if (pTestMsgIn->dataLen != SPI_TEST_SYNC_MSGLEN)
			{
			    printf("Incorrect data length %x\n", pTestMsgIn->dataLen);
				Write((LPVOID)pTestMsgOut, SPI_TEST_MSGLEN, &ret_bytes);
				continue;
			}
			if (pTestMsgIn->seqNo != (gTestMsgSeqNo + 1))
			{
			    printf("Incorrect seq number %x\n", pTestMsgIn->seqNo);
				Write((LPVOID)pTestMsgOut, SPI_TEST_MSGLEN, &ret_bytes);
				continue;
			}

			/* Get the correct Sync message back */
			break;
		}
		
		printf("XWR1xxx board is ready for test.\n");
		spiTestMcb.board_present = 1;
		gTestMsgSeqNo += 2;
	}

	SetTimeouts(300, 300);
}

static void transfter_delay(unsigned int delay)
{
    unsigned int i;
	
    for(i = 0; i< delay *1000; i++)
	{
	}
}

/**
 *  @b Description
 *  @n
 *      This function performs Data Echo test with XWR1xxx device.
 *
 *  @retval
 *      FT Command Status.
 */
int dataEchoTest(void)
{
	char 			rxbuf[SPI_TEST_MSGLEN];
	char 			txbuf[SPI_TEST_MSGLEN];
	int 			loop;
	int 			failed = 0;
	DWORD 			ret_bytes;
	UCHAR 			retry = 0;
	UCHAR 			i, j;
	spiTestMsg*   	pTestMsgOut;
	spiTestMsg*   	pTestMsgIn;

    /* Get in/Out message pointer */
	pTestMsgOut = (spiTestMsg*)&txbuf[0];
	pTestMsgIn = (spiTestMsg*)&rxbuf[0];

	/* Board level Sync message is received from slave, started data echo test */
	for (loop = 0; loop < SPI_TEST_LOOP; loop++)
	{
		retry = 0;

		/* Initialize the data pattern */
		for (i = 0; i < SPI_TEST_MSGLEN - 8; i++)
			pTestMsgOut->data[i] = loop + i;

        /* Construct test message header*/
		pTestMsgOut->magicNumber = MAGIC_NUMBER;
		pTestMsgOut->dataLen = SPI_TEST_MSGLEN;
		pTestMsgOut->seqNo = gTestMsgSeqNo;

		do
		{
			transfter_delay(100);
			Write((LPVOID)pTestMsgOut, SPI_TEST_MSGLEN, &ret_bytes);

			/* Wait for slave is ready to send out a response message */
			transfter_delay(100);

			Read(pTestMsgIn, SPI_TEST_MSGLEN, &ret_bytes);

			if ((pTestMsgIn->magicNumber != MAGIC_NUMBER) ||
				(pTestMsgIn->dataLen != SPI_TEST_MSGLEN) ||
				(pTestMsgIn->seqNo != (gTestMsgSeqNo + 1)))
			{
				retry++;
				printf("Incorrect Message received from slave: magic =0x%x, dataLen=%d, seqNo=%d\n", 
							pTestMsgIn->magicNumber, pTestMsgIn->dataLen, pTestMsgIn->seqNo);
			}
			else
	        {
				break;
		    }
		} while (1);

        /* Received message from slave with data , verify the data */
		if (memcmp((void *)pTestMsgIn->data, (void *)pTestMsgOut->data, pTestMsgOut->dataLen - 8) != 0)
		{
			failed++;
			printf("Data validation failed for loop=%d\n",loop);
		}
		gTestMsgSeqNo += 2;

	}
	printf("Echo test failed %d out of %d\n", failed, SPI_TEST_LOOP);
	return 0;
}

/* Test Program for SPI test */
int _tmain(int argc, _TCHAR* argv[])
{
	printf("SPI Test Program started.\n");

	/* Initialize MCB */
	memset((void *)&spiTestMcb, 0, sizeof(spiTestMCB));

	/* Load FTD library and initialize SPI device */
	LoadDLL();
	printf("FTD dll is loaded \n");

	LoadSPIDevice();

	/* Sync is done, ready for data pattern test */
	dataEchoTest();

	return 0;
}

