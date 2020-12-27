###################################################################################
# CLI Utility makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./clilib.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

# This builds the CLI Library
lib: cliLib

# This cleans the CLI Library
libClean: cliLibClean

# Clean: This cleans all the objects
clean: libClean

# Build driver and all the unit tests
all: lib

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the CLI '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the Library only'
	@echo 'libClean             -> Clean the Library only'
	@echo '****************************************************************************************'

