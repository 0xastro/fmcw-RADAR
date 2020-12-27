##################################################################################
# staticclutterproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
include ./staticclutterproclib.mak

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/msstest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/dsstest.mak
else ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx), )
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/dsstest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

##################################################################################
# Build/Clean the library 
##################################################################################

# This builds the lib
lib: staticclutterprocLib

# This cleans the lib
libClean: staticclutterprocLibClean

##################################################################################
# Test targets which are device specific
#   Build the MSS and DSS Unit Test
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
testClean: 	mssTestClean dssTestClean
test: 		mssTest dssTest
else ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx), )
testClean: 	dssTestClean
test: 		dssTest
endif

# Clean: This cleans all the objects
clean: libClean testClean

# Build everything
all: lib test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for staticclutterproc'
	@echo 'all               -> Build lib and all unit tests'
	@echo 'clean             -> Clean out all the objects'
	@echo 'lib               -> Build the Driver only'
	@echo 'libClean          -> Clean the Driver Library only'
	@echo 'test              -> Build the applicable unit tests'
	@echo 'testClean         -> Clean the unit tests'
	@echo 'dssTest        -> Builds DSS Unit test for Static Clutter Proc'
	@echo 'dssTestClean   -> Cleans DSS Unit test for Static Clutter Proc'
	@echo 'mssTest        -> Builds MSS Unit test for Static Clutter Proc'
	@echo 'mssTestClean   -> Cleans MSS Unit test for Static Clutter Proc'
	@echo '****************************************************************************************'
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif

