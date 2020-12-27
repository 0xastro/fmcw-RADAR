###################################################################################
# HSI Header Utility makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./hsiheaderlib.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

# This builds the HSI Header Library
lib: hsiHeaderLib

# This cleans the HSI Header Library
libClean: hsiHeaderLibClean

# Clean: This cleans all the objects
clean: libClean

# Build driver and all the unit tests
all: lib

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the HSI Header '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the Library only'
	@echo 'libClean             -> Clean the Library only'
	@echo '****************************************************************************************'

