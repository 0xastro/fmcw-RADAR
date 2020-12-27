###################################################################################
#	makefile for mmwavelib FFT twiddle factor generation on DSP Sub System
###################################################################################

.PHONY: twiddleFactorGenTest twiddleFactorGenClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c $(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16/c64P
vpath %.c $(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16_imre/c64P
vpath %.c $(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft32x32/c64P

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_twiddleGen_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_twiddleGen_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVELIB_twiddleGen_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd


MMWAVELIB_twiddleGen_TEST_MAP    = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_twiddleGen.map
MMWAVELIB_twiddleGen_TEST_OUT    =  test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_twiddleGen.$(C674_EXE_EXT)
MMWAVELIB_twiddleGen_TEST_SOURCES  =  lib_ffttwiddle_gen_test.c \
                                      gen_twiddle_fft16x16.c \
									  gen_twiddle_fft16x16_imre.c \
                                      gen_twiddle_fft32x32.c

MMWAVELIB_twiddleGen_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_twiddleGen_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_twiddleGen_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_twiddleGen_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))


OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# RTSC Configuration:
###################################################################################
twiddleGenRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_twiddleGen_TEST_CONFIGPKG) $(MMWAVELIB_twiddleGen_TEST_CFG)
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
twiddleFactorGenTest: BUILD_CONFIGPKG=$(MMWAVELIB_twiddleGen_TEST_CONFIGPKG)
twiddleFactorGenTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
twiddleFactorGenTest: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
twiddleFactorGenTest: C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
                                       -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                                       -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
                                       -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

twiddleFactorGenTest: buildDirectories twiddleGenRTSC $(MMWAVELIB_twiddleGen_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_twiddleGen_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_twiddleGen_TEST_MAP) $(MMWAVELIB_twiddleGen_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_twiddleGen_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_twiddleGen_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Generation of FFT Twiddle Factor Test OUT'
	@echo '******************************************************************************'



###################################################################################
# Cleanup Unit Test:
###################################################################################
twiddleFactorGenClean:
	@echo 'Cleaning the FFT twiddle factor generation test objects'
	@rm -f $(MMWAVELIB_twiddleGen_TEST_OBJECTS) $(MMWAVELIB_twiddleGen_TEST_OUT) $(MMWAVELIB_twiddleGen_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_twiddleGen_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_twiddleGen_TEST_DEPENDS)
