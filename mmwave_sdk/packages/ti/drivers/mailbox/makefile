###################################################################################
# Mailbox makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./mailboxlib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/mssTest.mak
else ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/mssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/dssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_standalone/mssTestStandalone.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/dssTestStandalone.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean drv drvClean test testClean help

# This builds the Mailbox Driver
drv: mailboxDrv

# This cleans the Mailbox Driver
drvClean: mailboxDrvClean

###################################################################################
# Test Targets:
# XWR14xx: Build the MSS Unit Test
# XWR16xx/XWR18xx/XWR68xx: Build the MSS & DSS Unit Test
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
testClean: 	mssTestClean
test: 		mssTest
else
testClean: 	mssMsgExchangeTestClean dssMsgExchangeTestClean mssStandaloneTestClean dssStandaloneTestClean
test: 		mssMsgExchangeTest dssMsgExchangeTest mssStandaloneTest dssStandaloneTest 
endif

# Clean: This cleans all the objects
clean: drvClean testClean

# Build driver and all the unit tests
all: drv test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the Mailbox '
	@echo 'clean                -> Clean out all the objects'
	@echo 'drv                  -> Build the Driver only'
	@echo 'drvClean             -> Clean the Driver Library only'
	@echo 'test                 -> Builds all the unit tests for the SOC'
	@echo 'testClean            -> Cleans the unit tests for the SOC'
	@echo '****************************************************************************************'

