###################################################################################
# Makefile for iwr68xx
###################################################################################

MMWAVE_SDK_DEVICE_TYPE  = xwr68xx
PLATFORM_R4F_LINK_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/r4f_linker.cmd
PLATFORM_C674X_LINK_CMD = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/c674x_linker.cmd
PLATFORM_DEFINE         = SOC_XWR68XX
XDC_PLATFORM_DEFINE     = IWR68XX
PLATFORM_OBJDIR         = obj_$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)

MMWAVE_SHMEM_BANK_SIZE  = 0x20000 #128KB

ifeq ("$(MMWAVE_SDK_SHMEM_ALLOC)", "")
SHMEM_ALLOC = 0x02000006                          # default case
MMWAVE_L3RAM_NUM_BANK = 6
MMWAVE_SHMEM_TCMA_NUM_BANK = 0
MMWAVE_SHMEM_TCMB_NUM_BANK = 0
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x02000006) # default case
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 6
MMWAVE_SHMEM_TCMA_NUM_BANK = 0
MMWAVE_SHMEM_TCMB_NUM_BANK = 0
else
$(error Invalid SHMEM_ALLOC setting! Please check the environment variable MMWAVE_SDK_SHMEM_ALLOC.)
endif
