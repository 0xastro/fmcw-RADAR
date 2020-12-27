##################################################################################
# rangeproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
include ./rangeproclib.mak

# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
include ./test/mssHWATest.mak
include ./test/dssHWATest.mak
endif

# DSP applicable to all devices with DSP
include ./test/dssDSPTest.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean test testClean help

##################################################################################
# Build/Clean the library 
##################################################################################

# This builds the lib
lib: rangeprocLib

# This cleans the lib
libClean: rangeprocLibClean

##################################################################################
# Test targets which are device specific
#   Build the MSS and DSS Unit Test
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
testClean: 	mssHWATestClean dssHWATestClean dssDSPTestClean
test: 		mssHWATest dssHWATest dssDSPTest
else
testClean: 	dssDSPTestClean
test: 		dssDSPTest
endif

# Clean: This cleans all the objects
clean: libClean testClean

# Build everything
all: lib test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the Range Proc DPU'
	@echo 'all               -> Build lib and all unit tests'
	@echo 'clean             -> Clean out all the objects'
	@echo 'lib               -> Build the Driver only'
	@echo 'libClean          -> Clean the Driver Library only'
	@echo 'test              -> Build the applicable unit tests'
	@echo 'testClean         -> Clean the unit tests'
	@echo 'dssHWATest        -> Builds DSS Unit test for HWA Range Proc'
	@echo 'dssHWATestClean   -> Cleans DSS Unit test for HWA Range Proc'
	@echo 'mssHWATest        -> Builds MSS Unit test for HWA Range Proc'
	@echo 'mssHWATestClean   -> Cleans MSS Unit test for HWA Range Proc'
	@echo 'dssDSPTest        -> Builds DSS Unit test for DSP Range Proc'
	@echo 'dssDSPTestClean   -> Cleans MSS Unit test for DSP Range Proc'
	@echo '****************************************************************************************'
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif

