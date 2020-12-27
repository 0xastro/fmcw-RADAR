###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: aoaEstFltTest aoaEstFltClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_aoaEstFlt_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_aoaEstFlt_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)

MMWAVELIB_aoaEstFlt_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_aoaEstFlt.map
MMWAVELIB_aoaEstFlt_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_aoaEstFlt.$(C674_EXE_EXT)
MMWAVELIB_aoaEstFlt_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd

MMWAVELIB_aoaEstFlt_TEST_SOURCES  = 	lib_aoaest_test.c
								
# Compiler Flags for DSP Builds:
		    
MMWAVELIB_aoaEstFlt_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_aoaEstFlt_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_aoaEstFlt_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_aoaEstFlt_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
aoaEstFltRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_aoaEstFlt_TEST_CONFIGPKG) $(MMWAVELIB_aoaEstFlt_TEST_CFG)
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
aoaEstFltTest: BUILD_CONFIGPKG=$(MMWAVELIB_aoaEstFlt_TEST_CONFIGPKG)
aoaEstFltTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
aoaEstFltTest: buildDirectories dssRTSC $(MMWAVELIB_aoaEstFlt_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_aoaEstFlt_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_aoaEstFlt_TEST_MAP) $(MMWAVELIB_aoaEstFlt_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_aoaEstFlt_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_aoaEstFlt_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Floating-point AOA Estimation Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
aoaEstFltClean:
	@echo 'Cleaning the aoaEstFltTest objects'
	@rm -f $(MMWAVELIB_aoaEstFlt_TEST_OBJECTS) $(MMWAVELIB_aoaEstFlt_TEST_OUT) $(MMWAVELIB_aoaEstFlt_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_aoaEstFlt_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_aoaEstFlt_TEST_DEPENDS)

