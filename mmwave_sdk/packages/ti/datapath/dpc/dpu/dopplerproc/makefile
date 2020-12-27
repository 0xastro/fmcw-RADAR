##################################################################################
# dopplerproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr18xx xwr68xx), )
include ./dopplerproclib.mak
include ./test/hwa_mssTest.mak
include ./test/hwa_dssTest.mak


###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean test testClean help

##################################################################################
# Build/Clean the library 
##################################################################################

# This builds the lib
lib: dopplerprocLib

# This cleans the lib
libClean: dopplerprocLibClean


test: hwaMssTest
testClean: hwaMssTestClean


# Clean: This cleans all the objects
clean: libClean testClean

# Build everything
all: lib test 

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for dopplerproc'
	@echo 'all               -> Build lib and all unit tests'
	@echo 'clean             -> Clean out all the objects'
	@echo 'lib               -> Build the Driver only'
	@echo 'libClean          -> Clean the Driver Library only'
	@echo 'test              -> Build the MSS and DSS Unit test for HWA Doppler Proc test'
	@echo 'testClean         -> Clean the MSS and DSS Unit test for HWA Doppler Proc test'
	@echo 'hwaMssTest        -> Build the MSS Unit test for HWA Doppler Proc test'
	@echo 'hwaMssTestClean   -> Clean the MSS Unit test for HWA Doppler Proc test'
	@echo 'hwaDssTest        -> Build the DSS Unit test for HWA Doppler Proc test'
	@echo 'hwaDssTestClean   -> Clean the DSS Unit test for HWA Doppler Proc test'
	@echo '****************************************************************************************'
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif

