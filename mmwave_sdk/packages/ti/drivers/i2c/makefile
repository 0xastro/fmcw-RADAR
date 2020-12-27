##################################################################################
# I2C makefile
##################################################################################
include ../../common/mmwave_sdk.mak
include ./i2clib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssMasterTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssSlaveTest.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean test testClean help

##################################################################################
# Build/Clean the driver
##################################################################################

# This builds the UART Driver
drv: i2cDrv i2cSlaveDrv

# This cleans the UART Driver
drvClean: i2cDrvClean i2cSlaveDrvClean

###################################################################################
# Test Targets:
#   Build the MSS Unit Test
###################################################################################
testClean: 	mssMasterTestClean mssSlaveTestClean
test: 		mssMasterTest mssSlaveTest

# Clean: This cleans all the objects
clean: drvClean testClean

# Build everything
all: drv test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the I2C '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Driver only'
	@echo 'drvClean             -> Clean the Driver Library only'
	@echo 'test                 -> Builds all the unit tests for the I2C'
	@echo 'testClean            -> Cleans the unit tests for the I2C'
	@echo '****************************************************************************************'

