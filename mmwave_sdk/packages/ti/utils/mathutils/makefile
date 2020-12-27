###################################################################################
# MATH Utility makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./mathutils.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

# This builds the MATH utils Library
lib: mathUtilsLib

# This cleans the MATH utils Library
libClean: mathUtilsLibClean

# Clean: This cleans all the objects
clean: libClean

# Build driver and all the unit tests
all: lib

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the MATH utils '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the Library only'
	@echo 'libClean             -> Clean the Library only'
	@echo '****************************************************************************************'

