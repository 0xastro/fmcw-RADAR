##################################################################################
# CRYPTO makefile
##################################################################################
include ../../common/mmwave_sdk.mak
include ./cryptolib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx), )
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssTest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean test testClean help

##################################################################################
# Build/Clean the driver
##################################################################################

# This builds the CRYPTO Driver
drv: cryptoDrv

# This cleans the CRYPTO Driver
drvClean: cryptoDrvClean

###################################################################################
# Test Targets:
# - XWR16xx/XWR18xx: Build the MSS Unit Test
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx), )
testClean: 	mssTestClean
test: 		mssTest
endif

# Clean: This cleans all the objects
clean: drvClean testClean

# Build everything
all: drv test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the CRYPTO '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Driver only'
	@echo 'drvClean             -> Clean the Driver Library only'
	@echo 'test                 -> Builds all the unit tests for the CRYPTO'
	@echo 'testClean            -> Cleans the unit tests for the CRYPTO'
	@echo '****************************************************************************************'

