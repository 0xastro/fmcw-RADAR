##################################################################################
# OSAL makefile
##################################################################################
include ../../common/mmwave_sdk.mak
include ./osallib.mak

###################################################################################
# Standard definitions to build the module
###################################################################################
STD_INCL = -i$(XDC_INSTALL_PATH)/packages -i$(BIOS_INSTALL_PATH)

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean help

# This builds the OSAL Porting Driver
drv: osal

# This cleans the OSAL Porting Driver
drvClean: osalClean

# Clean: This cleans all the objects
clean: drvClean

# Build everything
all: drv

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the OSAL '
	@echo 'drv                  -> Build the Core Library only'
	@echo 'drvClean             -> Clean the Core Library only'
	@echo '****************************************************************************************'

