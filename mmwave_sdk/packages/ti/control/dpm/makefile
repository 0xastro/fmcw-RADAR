###################################################################################
# DPM makefile
###################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include ../../common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
include ./dpmlib.mak

include ./test/distributed/dss/dss.mak
include ./test/distributed/mss/mss.mak
include ./test/local/mss/mss.mak
include ./test/remote/dss/dss.mak
include ./test/remote/mss/mss.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean test testClean help

# This builds the DPM Module
lib: dpmLib

# This cleans the DPM Module
libClean: dpmLibClean

###################################################################################
# Test Targets:
###################################################################################
testClean: 	dssDistributedClean mssDistributedClean mssLocalClean mssRemoteClean dssRemoteClean
test: 		dssDistributed mssDistributed mssLocal mssRemote dssRemote

# Clean: This cleans all the objects
clean: libClean testClean

# Build driver and all the unit tests
all: lib test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the DPM '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the Driver only'
	@echo 'libClean             -> Clean the Driver Library only'
	@echo 'test                 -> Build the applicable unit tests'
	@echo 'testClean            -> Clean the unit tests'
	@echo '****************************************************************************************'
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif

