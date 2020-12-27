###################################################################################
# mmWave makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./mmwavectrllib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), xwr14xx)
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/full/mssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/mssTest.mak
else
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/full/mssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/full/dssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/mssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/dssTest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean test testClean docs help

# This builds the mmWave library
lib: mmwaveCtrlLib

# This cleans the mmWave library
libClean: mmwaveCtrlLibClean

###################################################################################
# Test Targets:
# XWR14xx: Build the MSS Unit Test
# XWR16xx/XWR18xx/XWR68xx: Build the MSS and DSS Unit Test
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), xwr14xx)
testClean: 	mssFullTestClean mssMinimalTestClean
test: 		mssFullTest mssMinimalTest
else
testClean: 	mssFullTestClean dssFullTestClean mssMinimalTestClean dssMinimalTestClean
test: 		mssFullTest dssFullTest mssMinimalTest dssMinimalTest
endif

# Clean: This cleans all the objects
clean: libClean testClean

# Build driver and all the unit tests
all: lib test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the Mailbox '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the mmWave library only'
	@echo 'libClean             -> Clean the mmWave library only'
	@echo 'test                 -> Builds all the unit tests for the SOC'
	@echo 'testClean            -> Cleans the unit tests for the SOC'
	@echo '****************************************************************************************'

