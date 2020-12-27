###################################################################################
#	makefile for mmwavelib DFT single bin related functions tests
###################################################################################

.PHONY: dftSingleBinTest dftSingleBinClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_dftSingleBin_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_dftSingleBin_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVELIB_dftSingleBin_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd


MMWAVELIB_dftSingleBin_TEST_MAP    = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dftSingleBin.map
MMWAVELIB_dftSingleBin_TEST_OUT    =  test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dftSingleBin.$(C674_EXE_EXT)
MMWAVELIB_dftSingleBin_TEST_SOURCES  =  lib_dftsinglebin_test.c 
                                      

MMWAVELIB_dftSingleBin_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_dftSingleBin_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_dftSingleBin_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_dftSingleBin_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))


#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# RTSC Configuration:
###################################################################################
dftSingleBinRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_dftSingleBin_TEST_CONFIGPKG) $(MMWAVELIB_dftSingleBin_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '


###################################################################################
# Build Unit Test:
###################################################################################
dftSingleBinTest: BUILD_CONFIGPKG=$(MMWAVELIB_dftSingleBin_TEST_CONFIGPKG)
dftSingleBinTest : C674_CFLAGS  += -i$(C674x_MATHLIB_INSTALL_PATH)/packages
dftSingleBinTest : C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
#dftSingleBinTest : C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
dftSingleBinTest : C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
                                       -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                                       -i$(C674x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib \
                                       -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
                                       -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
                                       -lmathlib.$(C674_LIB_EXT)

dftSingleBinTest: buildDirectories dftSingleBinRTSC $(MMWAVELIB_dftSingleBin_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_dftSingleBin_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_dftSingleBin_TEST_MAP) $(MMWAVELIB_dftSingleBin_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_dftSingleBin_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_dftSingleBin_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib DFT single bin related function Test OUT'
	@echo '******************************************************************************'



###################################################################################
# Cleanup Unit Test:
###################################################################################
dftSingleBinClean:
	@echo 'Cleaning the DFT single bin related function test objects'
	@rm -f $(MMWAVELIB_dftSingleBin_TEST_OBJECTS) $(MMWAVELIB_dftSingleBin_TEST_OUT) $(MMWAVELIB_dftSingleBin_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_dftSingleBin_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_dftSingleBin_TEST_DEPENDS)
