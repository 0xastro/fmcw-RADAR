###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: dssTest dssTestClean floatTest floatTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c $(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16/c64P
vpath %.c $(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft32x32/c64P
vpath %.c $(C674x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c674

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_DSS_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)

MMWAVELIB_UNIT_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dss.map
MMWAVELIB_UNIT_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dss.$(C674_EXE_EXT)
MMWAVELIB_UNIT_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd

MMWAVELIB_UNIT_TEST_SOURCES  = 	lib_unittest.c\
								gen_twiddle_fft16x16.c\
								gen_twiddle_fft32x32.c\
								lib_abslog2_test.c\
								lib_window_test.c\
								lib_shift_histogram_test.c
								
MMWAVELIB_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_UNIT_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_UNIT_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))


MMWAVELIB_FLOAT_TEST_SOURCES  = libfloat_test.c\
					            gen_twiddle_fft16x16.c

MMWAVELIB_FLOATTEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_FLOAT_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_FLOAT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_FLOAT_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

MMWAVELIB_FLOAT_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_float.map
MMWAVELIB_FLOAT_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_float.$(C674_EXE_EXT)
								
# Compiler Flags for DSP Builds:
C674_CFLAGS  += -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16/c64P	\
				 -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft32x32/c64P \
				 -i$(C674x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c674 
			    
###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_DSS_TEST_CONFIGPKG) $(MMWAVELIB_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
C674_LDFLAGS  += -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
                 -i$(C674x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
                 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
			     -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
				 -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
				 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   \
                 -ldsplib.ae64P \
                 -ldsplib.ae674 
				 
###################################################################################
# Build Unit Test:
###################################################################################
floatTest: BUILD_CONFIGPKG=$(MMWAVELIB_DSS_TEST_CONFIGPKG)
floatTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
floatTest: buildDirectories dssRTSC $(MMWAVELIB_FLOAT_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_FLOAT_TEST_MAP) $(MMWAVELIB_FLOAT_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_UNIT_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_FLOAT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Floating-Point Chain Test OUT'
	@echo '******************************************************************************'

dssTest: BUILD_CONFIGPKG=$(MMWAVELIB_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(MMWAVELIB_UNIT_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_UNIT_TEST_MAP) $(MMWAVELIB_UNIT_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_UNIT_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Fixed-Point Unit Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
floatTestClean:
	@echo 'Cleaning the floating-point chain test objects'
	@$(DEL) $(MMWAVELIB_FLOAT_TEST_OBJECTS) $(MMWAVELIB_FLOAT_TEST_OUT) $(MMWAVELIB_FLOAT_TEST_DEPENDS)
#	@$(DEL) $(MMWAVELIB_DSS_TEST_CONFIGPKG)
dssTestClean:
	@echo 'Cleaning the fixed-point unit test objects'
	@$(DEL) $(MMWAVELIB_UNIT_TEST_OBJECTS) $(MMWAVELIB_UNIT_TEST_OUT) $(MMWAVELIB_UNIT_TEST_DEPENDS)
#	@$(DEL) $(MMWAVELIB_DSS_TEST_CONFIGPKG)


	
# Dependency handling
-include $(MMWAVELIB_UNIT_TEST_DEPENDS)
-include $(MMWAVELIB_FLOAT_TEST_DEPENDS)

