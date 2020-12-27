###################################################################################
# Makefile for xwr14xx
###################################################################################

#########################################
# Common for both awr14xx and iwr14xx
#########################################
MMWAVE_SDK_DEVICE_TYPE  = xwr14xx
PLATFORM_R4F_LINK_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/r4f_linker.cmd
PLATFORM_DEFINE         = SOC_XWR14XX
PLATFORM_OBJDIR         = obj_$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)

MMWAVE_SHMEM_BANK_SIZE  = 0x10000 #64KB

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
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00000105) # scenario where more code space is needed 
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 5
MMWAVE_SHMEM_TCMA_NUM_BANK = 1
MMWAVE_SHMEM_TCMB_NUM_BANK = 0
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00010005) # scenario where more .bss space is needed
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 5
MMWAVE_SHMEM_TCMA_NUM_BANK = 0
MMWAVE_SHMEM_TCMB_NUM_BANK = 1
else ifeq ($(MMWAVE_SDK_SHMEM_ALLOC), 0x00010203) # scenario where more code and .bss space is needed
SHMEM_ALLOC = $(MMWAVE_SDK_SHMEM_ALLOC)
MMWAVE_L3RAM_NUM_BANK = 3
MMWAVE_SHMEM_TCMA_NUM_BANK = 2
MMWAVE_SHMEM_TCMB_NUM_BANK = 1
else
$(error Invalid SHMEM_ALLOC setting! Please check the environment variable MMWAVE_SDK_SHMEM_ALLOC.)
endif

#########################################
# Specific for awr14xx or iwr14xx
#########################################
ifeq ($(MMWAVE_SDK_DEVICE), awr14xx)
XDC_PLATFORM_DEFINE     = AWR14XX
else ifeq ($(MMWAVE_SDK_DEVICE), iwr14xx)
XDC_PLATFORM_DEFINE     = IWR14XX
endif


