###################################################################################
# PINMUX makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./pinmuxlib.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean help

# This builds the PINMUX Driver
drv: pinmuxDrv

# This cleans the PINMUX Driver
drvClean: pinmuxDrvClean

# Clean: This cleans all the objects
clean: drvClean

# Build driver and all the unit tests
all: drv

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the PINMUX '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Core Library only'
	@echo 'drvClean             -> Clean the Core Library only'
	@echo '****************************************************************************************'

