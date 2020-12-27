# #################################################################################
#   mmWaveLib makefile
###################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
include ./mmwavelib.mak

include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/dssTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/winFltTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/accumPowerFltTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/cfarcaFltTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/aoaestFltTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/dbscanClusteringTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/cfarosTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/vecmathTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/peakPruningTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/twiddleGenTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/dftSingleBinTest.mak
include ./test/$(MMWAVE_SDK_DEVICE_TYPE)/winCoefGenTest.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################

.PHONY: all clean lib libClean allTest allTestClean test testClean help

##################################################################################
# Build/Clean the library
##################################################################################

# This builds the mmWavelib library
lib: mmwavelib

# This cleans the mmWavelib library
libClean: mmwavelibClean

##################################################################################
# Test targets which are device specific
##################################################################################
testClean: dssTestClean
test:      dssTest

# Clean: This cleans all the test objects
allTestClean: testClean winFltClean accumPowerFltClean cfarcaFltClean aoaEstFltClean floatTestClean \
	dbscanClusteringClean cfarosClean vecmathClean peakPruningClean twiddleFactorGenClean dftSingleBinClean \
	windowCoefGenClean

# Builds all the tests
allTest: test winFltTest accumPowerFltTest cfarcaFltTest aoaEstFltTest floatTest \
	dbscanClusteringTest cfarosTest vecmathTest peakPruningTest twiddleFactorGenTest dftSingleBinTest \
	windowCoefGenTest

# Clean: This cleans all the objects
clean: libClean allTestClean

# Build everything
all: lib allTest

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo '* Makefile Targets for alg\mmWaveLib '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the Core Library only'
	@echo 'libClean             -> Clean the Core Library only'
	@echo 'allTest              -> Builds all the Unit Tests (fixed-point & float point)'
	@echo 'allTestClean         -> Cleans all the Unit Tests (fixed-point & float point)'
	@echo 'test                 -> Builds the Unit Tests (fixed-point)'
	@echo 'testClean            -> Cleans the Unit Tests (fixed-point)'
	@echo 'floatTest            -> Builds the floating-point chain test'
	@echo 'floatTestClean       -> Cleans the floating-point chain test'
	@echo 'winFltTest           -> Builds the floating-point windowing Tests'
	@echo 'winFltClean          -> Cleans the floating-point windowing Tests'
	@echo 'accumPowerFltTest    -> Builds the floating-point power accumulation Tests'
	@echo 'accumPowerFltClean   -> Cleans the floating-point power accumulation Tests'
	@echo 'cfarcaFltTest        -> Builds the floating-point CFAR CA, CACC, SO, GO Tests'
	@echo 'cfarcaFltClean       -> Cleans the floating-point CFAR CA, CACC, SO, GO Tests'
	@echo 'aoaEstFltTest        -> Builds the floating-point AOA Estimation Tests'
	@echo 'aoaEstFltClean       -> Cleans the floating-point AOA Estimation Tests'
	@echo 'dbscanClusteringTest      -> Builds the DBSCAN fixed-point Tests'
	@echo 'dbscanClusteringClean     -> Cleans the DBSCAN fixed-point Tests'
	@echo 'cfarosTest           -> Builds the fixed-point CFAR OS Unit Tests'
	@echo 'cfarosClean          -> Cleans the fixed-point CFAR OS Unit Tests'
	@echo 'vecmathTest          -> Builds Vector math and Matrix utility Unit Tests'
	@echo 'vecmathClean         -> Cleans Vector math and Matrix utility Unit Tests'
	@echo 'peakPruningTest      -> Builds the fixed-point peak pruning Unit Tests'
	@echo 'peakPruningClean     -> Cleans the fixed-point peak pruning Unit Tests'
	@echo 'twiddleFactorGenTest      -> Builds the FFT Twiddle factor generation Unit Tests'
	@echo 'twiddleFactorGenClean     -> Cleans the FFT Twiddle factor generation Unit Tests'
	@echo 'dftSingleBinTest     -> Builds the DFT single bin related functions Unit Tests'
	@echo 'dftSingleBinClean    -> Cleans the DFT single bin related functions Unit Tests'
	@echo 'windowCoefGenTest     -> Builds the window coefficient generation functions Unit Tests'
	@echo 'windowCoefGenClean    -> Cleans the window coefficient generation functions Unit Tests'
	@echo '****************************************************************************************'
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif

