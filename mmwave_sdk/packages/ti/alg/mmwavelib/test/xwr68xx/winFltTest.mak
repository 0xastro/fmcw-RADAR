###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: winFltTest winFltClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_winFlt_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_winFlt_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)

MMWAVELIB_winFlt_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_windowFlt.map
MMWAVELIB_winFlt_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_windowFlt.$(C674_EXE_EXT)
MMWAVELIB_winFlt_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd

MMWAVELIB_winFlt_TEST_SOURCES  = 	lib_windowflt_test.c
								
# Compiler Flags for DSP Builds:
		    
MMWAVELIB_winFlt_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_winFlt_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_winFlt_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_winFlt_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
winFltRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_winFlt_TEST_CONFIGPKG) $(MMWAVELIB_winFlt_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
#STD_LIBS = -llibmmwavelib_$(MMWAVE_SDK_DEVICE_TYPE).$(DSP_LIB_EXT)    \
#           -ldsplib.lib    \
#LOC_LIBS = -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
#           -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib

C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
			     -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                 -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
				 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   
                 
###################################################################################
# Build Unit Test:
###################################################################################
winFltTest: BUILD_CONFIGPKG=$(MMWAVELIB_winFlt_TEST_CONFIGPKG)
winFltTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
winFltTest: buildDirectories dssRTSC $(MMWAVELIB_winFlt_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_winFlt_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_winFlt_TEST_MAP) $(MMWAVELIB_winFlt_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_winFlt_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_winFlt_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Floating-point Windowing Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
winFltClean:
	@echo 'Cleaning the winFltTest objects'
	@$(DEL) $(MMWAVELIB_winFlt_TEST_OBJECTS) $(MMWAVELIB_winFlt_TEST_OUT) $(MMWAVELIB_winFlt_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_winFlt_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_winFlt_TEST_DEPENDS)

