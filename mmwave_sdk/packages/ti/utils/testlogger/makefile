###################################################################################
# Test Logger makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./testloggerlib.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

# This builds the test logger library
lib: testLoggerLib

# This cleans the test logger library
libClean: testLoggerLibClean

# Clean: This cleans all the objects
clean: libClean

# Build driver and all the unit tests
all: lib

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the Test Logger '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the mmWave library only'
	@echo 'libClean             -> Clean the mmWave library only'
	@echo '****************************************************************************************'

