##################################################################################
# ESM makefile
##################################################################################
include ../../common/mmwave_sdk.mak
include ./esmlib.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean help

# This builds the ESM Driver
drv: esmDrv

# This cleans the ESM Driver
drvClean: esmDrvClean

# Clean: This cleans all the objects
clean: drvClean

# Build everything
all: drv

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the ESM '
	@echo 'drv                  -> Build the Core Library only'
	@echo 'drvClean             -> Clean the Core Library only'
	@echo '****************************************************************************************'

