###################################################################################
# mmWave Demo Level makefile
###################################################################################
ifeq ($(CCS_MAKEFILE_BASED_BUILD), 1)
ifeq ($(OS),Windows_NT)
include $(MMWAVE_SDK_INSTALL_PATH)/scripts/windows/setenv.mak
else
include $(MMWAVE_SDK_INSTALL_PATH)/scripts/unix/setenv.mak
endif
endif


include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak
include ./mss/mmw_mss.mak
include ./dss/mmw_dss.mak

MMW_DEMO_BIN         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo.bin
MMW_DEMO_SECURE_BIN  = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_secure.bin

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean
.NOTPARALLEL:

# Clean: This cleans all the objects
clean: mssDemoClean dssDemoClean
	@$(DEL) $(MMW_DEMO_BIN)
	@$(DEL) $(MMW_DEMO_SECURE_BIN)

# This builds all the supported demos
all: mssDemo dssDemo
	@$(GENERATE_METAIMAGE) $(MMW_DEMO_BIN) $(SHMEM_ALLOC) $(MSS_MMW_DEMO_OUT) $(XWR18XX_RADARSS_IMAGE_BIN) $(DSS_MMW_DEMO_OUT)
	@$(GENERATE_HS_METAIMAGE) $(MMW_DEMO_SECURE_BIN) $(SHMEM_ALLOC) $(MSS_MMW_DEMO_OUT) $(XWR18XX_RADARSS_IMAGE_BIN) $(DSS_MMW_DEMO_OUT) $(MMWAVE_SECDEV_HSIMAGE_CFG)

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the DEMO '
	@echo 'all	       -> Builds the mmw Demo (both MSS and DSS binaries)'
	@echo 'clean           -> Cleans the mmw Demo (both MSS and DSS binaries)'
	@echo 'mssDemo         -> Builds the MSS binary for the mmw Demo'
	@echo 'mssDemoClean    -> Cleans the MSS binary for mmw Demo '
	@echo 'dssDemo         -> Builds the DSS binary for the mmw Demo'
	@echo 'dssDemoClean    -> Cleans the DSS binary for mmw Demo '
	@echo '****************************************************************************************'

