###################################################################################
# SBL makefile
###################################################################################
include ../../common/mmwave_sdk.mak
include ./sbl.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean sbl sblClean help

# Clean: This cleans all the objects
clean: sblClean

# Builds SBL
all: sbl
ifndef SBL_SOURCES
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE_TYPE)"
else
	@echo 'Built the SBL image. Preparing the METAIMAGE BIN ...'
	@$(GENERATE_METAIMAGE) $(SBL_BIN) $(SHMEM_ALLOC) $(SBL_OUT) NULL NULL
endif

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for the SBL '
	@echo 'all                  -> Build the SBL meta image binary'
	@echo 'clean                -> Clean out all the objects'
	@echo 'sbl                  -> Build SBL out file'
	@echo 'sblClean             -> Clean SBL objects only'
	@echo '****************************************************************************************'


