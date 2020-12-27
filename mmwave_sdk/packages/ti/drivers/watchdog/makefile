##################################################################################
# Watchdog makefile
##################################################################################
include ../../common/mmwave_sdk.mak
include ./watchdoglib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssWatchdogTest.mak
else
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssWatchdogTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/dssWatchdogTest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean test testClean help

##################################################################################
# Build/Clean the driver
##################################################################################

# This builds the Watchdog Driver
drv: watchdogDrv

# This cleans the Watchdog Driver
drvClean: watchdogDrvClean

###################################################################################
# Test Targets:
# XWR14xx: Build the MSS Unit Test
# XWR16xx/XWR18xx/XWR68xx: Build the MSS and DSS Unit Test
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
testClean: 	mssWatchdogTestClean
test: 		mssWatchdogTest
else
testClean: 	mssWatchdogTestClean dssWatchdogTestClean
test: 		mssWatchdogTest dssWatchdogTest
endif

# Clean: This cleans all the objects
clean: drvClean testClean

# Build everything
all: drv test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the Watchdog '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Driver only'
	@echo 'drvClean             -> Clean the Driver Library only'
	@echo 'test                 -> Builds all the unit tests for the Watchdog'
	@echo 'testClean            -> Cleans the unit tests for the Watchdog'
	@echo '****************************************************************************************'

