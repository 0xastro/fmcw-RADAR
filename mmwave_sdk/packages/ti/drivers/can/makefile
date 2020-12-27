###################################################################################
# CAN makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./canlib.mak

##################################################################################
# CAN Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr14xx xwr16xx xwr18xx), )
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssTest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean test testClean help

# This builds the CAN Driver
drv: canDrv

# This cleans the CAN Driver
drvClean: canDrvClean

###################################################################################
# Test Targets:
#  Build the MSS Unit Test
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr14xx xwr16xx xwr18xx), )
testClean: 	mssTestClean
test: 		mssTest
endif

# Clean: This cleans all the objects
clean: drvClean testClean

# Build driver and all the unit tests
all: drv test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the CAN '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Driver only'
	@echo 'drvClean             -> Clean the Driver Library only'
	@echo 'test                 -> Builds all the unit tests for the CAN'
	@echo 'testClean            -> Cleans the unit tests for the CAN'
	@echo '****************************************************************************************'


