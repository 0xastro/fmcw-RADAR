###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: cfarcaFltTest cfarcaFltClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_cfarcaFlt_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_cfarcaFlt_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)

MMWAVELIB_cfarcaFlt_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_cfarcaFlt.map
MMWAVELIB_cfarcaFlt_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_cfarcaFlt.$(C674_EXE_EXT)
MMWAVELIB_cfarcaFlt_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd

MMWAVELIB_cfarcaFlt_TEST_SOURCES  = 	lib_cfarcaall_test.c
								
# Compiler Flags for DSP Builds:
		    
MMWAVELIB_cfarcaFlt_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_cfarcaFlt_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_cfarcaFlt_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_cfarcaFlt_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
cfarcaFltRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_cfarcaFlt_TEST_CONFIGPKG) $(MMWAVELIB_cfarcaFlt_TEST_CFG)
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
cfarcaFltTest: BUILD_CONFIGPKG=$(MMWAVELIB_cfarcaFlt_TEST_CONFIGPKG)
cfarcaFltTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
cfarcaFltTest: buildDirectories dssRTSC $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_cfarcaFlt_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_cfarcaFlt_TEST_MAP) $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_cfarcaFlt_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_cfarcaFlt_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Floating-point CFAR CA, CACC, SO, GO Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
cfarcaFltClean:
	@echo 'Cleaning the cfarcaFltTest objects'
	@rm -f $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS) $(MMWAVELIB_cfarcaFlt_TEST_OUT) $(MMWAVELIB_cfarcaFlt_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_cfarcaFlt_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_cfarcaFlt_TEST_DEPENDS)

