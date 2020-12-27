###################################################################################
#	makefile for mmwavelib window coefficients generation on DSP Sub System
###################################################################################

.PHONY: windowCoefGenTest windowCoefGenClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_winCoefGen_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_winCoefGen_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVELIB_winCoefGen_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd


MMWAVELIB_winCoefGen_TEST_MAP    = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_winCoefGen.map
MMWAVELIB_winCoefGen_TEST_OUT    =  test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_winCoefGen.$(C674_EXE_EXT)
MMWAVELIB_winCoefGen_TEST_SOURCES  =  lib_windowCoef_gen_test.c 
                                      
MMWAVELIB_winCoefGen_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_winCoefGen_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_winCoefGen_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_winCoefGen_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))


#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# RTSC Configuration:
###################################################################################
winCoefGenRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_winCoefGen_TEST_CONFIGPKG) $(MMWAVELIB_winCoefGen_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
#STD_LIBS = -llibmmwavelib_$(MMWAVE_SDK_DEVICE_TYPE).$(DSP_LIB_EXT)    \
#           -ldsplib.lib    \
#LOC_LIBS = -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
#           -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib



       
###################################################################################
# Build Unit Test:
###################################################################################
windowCoefGenTest: BUILD_CONFIGPKG=$(MMWAVELIB_winCoefGen_TEST_CONFIGPKG)
windowCoefGenTest : C674_CFLAGS  += -i$(C674x_MATHLIB_INSTALL_PATH)/packages
windowCoefGenTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
#windowCoefGenTest: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
windowCoefGenTest: C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
                                       -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                                       -i$(C674x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib \
                                       -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
                                       -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
                                       -lmathlib.$(C674_LIB_EXT)

windowCoefGenTest: buildDirectories winCoefGenRTSC $(MMWAVELIB_winCoefGen_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_winCoefGen_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_winCoefGen_TEST_MAP) $(MMWAVELIB_winCoefGen_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_winCoefGen_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_winCoefGen_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Window Factor Generation Test OUT'
	@echo '******************************************************************************'



###################################################################################
# Cleanup Unit Test:
###################################################################################
windowCoefGenClean:
	@echo 'Cleaning the FFT twiddle factor generation test objects'
	@rm -f $(MMWAVELIB_winCoefGen_TEST_OBJECTS) $(MMWAVELIB_winCoefGen_TEST_OUT) $(MMWAVELIB_winCoefGen_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_winCoefGen_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_winCoefGen_TEST_DEPENDS)
