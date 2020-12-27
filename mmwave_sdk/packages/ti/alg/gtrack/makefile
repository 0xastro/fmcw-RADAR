##################################################################################
# GTRACK makefile
##################################################################################
include ../../common/mmwave_sdk.mak
include ./gtracklib.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
include ./test/usecases/mss/gtrackTestMss.mak
else
include ./test/usecases/mss/gtrackTestMss.mak
include ./test/usecases/dss/gtrackTestDss.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean test testClean help
.PHONY: gtracklib2d gtracklib3d
.PHONY: gtracktest2d gtracktest3d gtracktestMss gtracktestDss gtrackTestMss2d gtrackTestMss3d gtrackTestDss2d gtrackTestDss3d

##################################################################################
# Build/Clean the driver
##################################################################################

# This builds GTRACK Libraries
lib:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtracklib
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtracklib

# This builds the 2D GTRACK Library
gtracklib2d:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtracklib

# This builds the 3D GTRACK Library
gtracklib3d:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtracklib
		
# This cleans GTRACK Libraries
libClean:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtracklibClean
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtracklibClean

# This cleans the 2D GTRACK Library
gtracklib2dClean:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtracklibClean

# This cleans the 3D GTRACK Library
gtracklib3dClean:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtracklibClean
	
##################################################################################
# Test targets which are device specific
# XWR14xx: Build the MSS Unit Test
# XWR16xx/XWR18xx/XWR68xx: Build the MSS and DSS Unit Test
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
testClean: 	
test: 		
else ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
test:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestMss
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestMss

	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestDss
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestDss
testClean:
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestMssClean
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestMssClean

	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestDssClean
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestDssClean
endif

gtracktest2d:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestMss
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestDss
endif

gtracktest3d:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestMss
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestDss
endif

gtracktestMss:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestMss
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestMss
endif

gtracktestDss:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestDss
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestDss
endif

gtracktestMss2d:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestMss
endif

gtracktestMss3d:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestMss
endif

gtracktestDss2d:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=2D gtrackTestDss
endif

gtracktestDss3d:
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr16xx)
	$(MAKE) -s MMWAVE_SDK_LIB_BUILD_OPTION=3D gtrackTestDss
endif


# Clean: This cleans all the objects
clean: libClean testClean

# Build everything
all: lib test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for GTRACK '
	@echo 'all            -> Build gtrack lib and all functional unit tests'
	@echo 'clean          -> Clean out all the gtrack objects'
	@echo 'lib            -> Build the gtrack algorithm Library only'
	@echo 'libClean       -> Clean the gtrack algorithm Library only'
	@echo 'test           -> Builds all the gtrack unit tests for the SOC'
	@echo 'testClean      -> Cleans all the gtrack unit tests for the SOC'
	@echo '****************************************************************************************'

