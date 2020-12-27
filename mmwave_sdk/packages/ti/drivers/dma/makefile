###################################################################################
# DMA makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./dmalib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
include ./test/mssTest.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean test testClean help

# This builds the DMA Driver
drv: dmaDrv

# This cleans the DMA Driver
drvClean: dmaDrvClean

###################################################################################
# Test Targets:
#    Build the MSS Unit Test
###################################################################################
testClean: 	mssTestClean
test: 		mssTest

# Clean: This cleans all the objects
clean: drvClean testClean

# Build driver and all the unit tests
all: drv test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the DMA '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Driver only'
	@echo 'drvClean             -> Clean the Driver Library only'
	@echo 'test                 -> Builds all the unit tests for the SOC'
	@echo 'testClean            -> Cleans the unit tests for the SOC'
	@echo '****************************************************************************************'

