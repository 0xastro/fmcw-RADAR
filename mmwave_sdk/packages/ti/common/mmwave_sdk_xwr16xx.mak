###################################################################################
# Makefile for xwr16xx
###################################################################################

#########################################
# Common for both awr16xx and iwr16xx
#########################################
MMWAVE_SDK_DEVICE_TYPE  = xwr16xx
PLATFORM_R4F_LINK_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/r4f_linker.cmd
PLATFORM_C674X_LINK_CMD = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/c674x_linker.cmd
PLATFORM_DEFINE         = SOC_XWR16XX
PLATFORM_OBJDIR         = obj_$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)

MMWAVE_SHMEM_BANK_SIZE  = 0x20000 #128KB

ifeq ("$(MMWAVE_SDK_SHMEM_ALLOC)", "")
SHMEM_ALLOC = 0x00000006                          # default case
MMWAVE_L3RAM_NUM_BANK = 6
MMWAVE_SHMEM_TCMA_NUM_BANK = 0
MMWAVE_SHMEM_TCMB_NUM_BANK = 0
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00000006) # default case
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 6
MMWAVE_SHMEM_TCMA_NUM_BANK = 0
MMWAVE_SHMEM_TCMB_NUM_BANK = 0
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00000204) # scenario where more MSS code space is needed
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 4
MMWAVE_SHMEM_TCMA_NUM_BANK = 2
MMWAVE_SHMEM_TCMB_NUM_BANK = 0
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00010005) # scenario where more MSS .bss space is needed
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 5
MMWAVE_SHMEM_TCMA_NUM_BANK = 0
MMWAVE_SHMEM_TCMB_NUM_BANK = 1
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00010104) # scenario where more MSS code and MSS .bss space is needed
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 4
MMWAVE_SHMEM_TCMA_NUM_BANK = 1
MMWAVE_SHMEM_TCMB_NUM_BANK = 1
else
$(error Invalid SHMEM_ALLOC setting! Please check the environment variable MMWAVE_SDK_SHMEM_ALLOC.)
endif

#########################################
# Specific for awr16xx or iwr16xx
#########################################
ifeq ($(MMWAVE_SDK_DEVICE), awr16xx)
XDC_PLATFORM_DEFINE     = AWR16XX
else ifeq ($(MMWAVE_SDK_DEVICE), iwr16xx)
XDC_PLATFORM_DEFINE     = IWR16XX
endif
