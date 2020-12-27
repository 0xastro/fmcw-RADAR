###################################################################################
# CCS Debug makefile
###################################################################################
include ../../common/mmwave_sdk.mak

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
include mssBuild.mak
else
include mssBuild.mak
include dssBuild.mak
endif


###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean help

CCS_DEBUG_BIN         = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug.bin
CCS_DEBUG_SECURE_BIN  = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_secure.bin

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
RADARSS_IMAGE_BIN = $(XWR14XX_RADARSS_IMAGE_BIN)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
RADARSS_IMAGE_BIN = $(XWR16XX_RADARSS_IMAGE_BIN)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr18xx)
RADARSS_IMAGE_BIN = $(XWR18XX_RADARSS_IMAGE_BIN)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr68xx)
RADARSS_IMAGE_BIN = $(XWR68XX_RADARSS_IMAGE_BIN)
else
$(error Unsupported device $(MMWAVE_SDK_DEVICE).)
endif

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
clean: mssCCSDebugClean 
	@$(DEL) $(CCS_DEBUG_BIN)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr68xx)
clean: mssCCSDebugClean dssCCSDebugClean
	@$(DEL) $(CCS_DEBUG_BIN)
else
clean: mssCCSDebugClean dssCCSDebugClean
	@$(DEL) $(CCS_DEBUG_BIN)
	@$(DEL) $(CCS_DEBUG_SECURE_BIN)
endif
	@echo '******************************************************************************'
	@echo 'CCS Debug Utility Clean complete '
	@echo '******************************************************************************'

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
all: mssCCSDebug
	@echo 'Built the CCS Debug MSS. Preparing the METAIMAGE BIN ...'
	@$(GENERATE_METAIMAGE) $(CCS_DEBUG_BIN) $(SHMEM_ALLOC) $(CCS_DEBUG_MSS_OUT) $(RADARSS_IMAGE_BIN) NULL
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr68xx)
all: mssCCSDebug dssCCSDebug
	@echo 'Built the CCS Debug MSS & DSS. Preparing the METAIMAGE BIN ...'
	@$(GENERATE_METAIMAGE) $(CCS_DEBUG_BIN) $(SHMEM_ALLOC) $(CCS_DEBUG_MSS_OUT) $(RADARSS_IMAGE_BIN) $(CCS_DEBUG_DSS_OUT)
else
all: mssCCSDebug dssCCSDebug
	@echo 'Built the CCS Debug MSS & DSS. Preparing the METAIMAGE BIN ...'
	@$(GENERATE_METAIMAGE) $(CCS_DEBUG_BIN) $(SHMEM_ALLOC) $(CCS_DEBUG_MSS_OUT) $(RADARSS_IMAGE_BIN) $(CCS_DEBUG_DSS_OUT)
	@$(GENERATE_HS_METAIMAGE) $(CCS_DEBUG_SECURE_BIN) $(SHMEM_ALLOC) $(CCS_DEBUG_MSS_OUT) $(RADARSS_IMAGE_BIN) $(CCS_DEBUG_DSS_OUT) $(MMWAVE_SECDEV_HSIMAGE_CFG) 
endif

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the CCS Debug Utility '
	@echo 'all                  -> Build the CCS Debug Utility'
	@echo 'clean                -> Clean the CCS Debug Utility'
	@echo '****************************************************************************************'


