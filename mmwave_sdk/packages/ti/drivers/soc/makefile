###################################################################################
# SOC makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./soclib.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean help

# This builds the SOC Driver
drv: socDrv

# This cleans the SOC Driver
drvClean: socDrvClean

# Clean: This cleans all the objects
clean: drvClean

# Build driver and all the unit tests
all: drv

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the SOC Driver '
	@echo 'drv                  -> Build the Core Library only'
	@echo 'drvClean             -> Clean the Core Library only'
	@echo '****************************************************************************************'

