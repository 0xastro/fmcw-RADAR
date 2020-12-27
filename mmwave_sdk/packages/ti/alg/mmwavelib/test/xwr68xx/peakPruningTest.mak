###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: peakPruningTest peakPruningClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_peakPruning_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_peakPruning_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVELIB_peakPruning_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd


MMWAVELIB_peakPruning_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_peakPruning.map
MMWAVELIB_peakPruning_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_peakPruning.$(C674_EXE_EXT)
MMWAVELIB_peakPruning_TEST_SOURCES  = 	lib_peakpruning_test.c
MMWAVELIB_peakPruning_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_peakPruning_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_peakPruning_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_peakPruning_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# Debug Test Files
###################################################################################
lib_peakPruning_DEBUG_MAP  = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_libdebug_peakPruning.map
lib_peakPruning_DEBUG_OUT  = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_libdebug_peakPruning.$(C674_EXE_EXT)
lib_peakPruning_DEBUG_SOURCES  =  lib_peakpruning_test.c \
                                  lib_peakpruning_debug.c  
lib_peakPruning_DEBUG_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(lib_peakPruning_DEBUG_SOURCES:.c=.$(C674_DEP_EXT)))
lib_peakPruning_DEBUG_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(lib_peakPruning_DEBUG_SOURCES:.c=.$(C674_OBJ_EXT)))

OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# RTSC Configuration:
###################################################################################
peakPruningRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_peakPruning_TEST_CONFIGPKG) $(MMWAVELIB_peakPruning_TEST_CFG)
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
# Build Debug Test:
###################################################################################
peakPruningDebug: BUILD_CONFIGPKG=$(MMWAVELIB_peakPruning_TEST_CONFIGPKG)
peakPruningDebug: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
peakPruningDebug: C674_CFLAGS += --define=DEBUGPEAKPRUNING
#peakPruningDebug: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
peakPruningDebug: C674_peakprunign_LDFLAGS = -mv6740 --abi=eabi -g --define=$(PLATFORM_DEFINE) --display_error_number           \
               --diag_warning=225 --diag_wrap=off -z --reread_libs --warn_sections --ram_model --define=MMWAVE_L3RAM_SIZE=0xC0000   \
               -i$(C674_CODEGEN_INSTALL_PATH)/lib --emit_warnings_as_errors \
                -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
peakPruningDebug: buildDirectories dssRTSC $(lib_peakPruning_DEBUG_OBJECTS)
	$(C674_LD) $(C674_peakprunign_LDFLAGS) \
	-l$(MMWAVELIB_peakPruning_TEST_CONFIGPKG)/linker.cmd --map_file=$(lib_peakPruning_DEBUG_MAP) $(lib_peakPruning_DEBUG_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_peakPruning_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(lib_peakPruning_DEBUG_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Peak Group/Pruning DEBUG OUT'
	@echo '******************************************************************************'

       
###################################################################################
# Build Unit Test:
###################################################################################
peakPruningTest: BUILD_CONFIGPKG=$(MMWAVELIB_peakPruning_TEST_CONFIGPKG)
peakPruningTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
peakPruningTest: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
peakPruningTest: C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
		 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                 -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
                 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   
peakPruningTest: buildDirectories dssRTSC $(MMWAVELIB_peakPruning_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_peakPruning_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_peakPruning_TEST_MAP) $(MMWAVELIB_peakPruning_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_peakPruning_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_peakPruning_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Peak Group/Pruning Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup DEBUG Test:
###################################################################################
peakPruningDebugClean:
	@echo 'Cleaning the peakPruningDebug objects'
	@rm -f $(lib_peakPruning_DEBUG_OBJECTS) $(lib_peakPruning_DEBUG_OUT) $(lib_peakPruning_DEBUG_DEPENDS)
	@$(DEL) $(MMWAVELIB_peakPruning_TEST_CONFIGPKG)

###################################################################################
# Cleanup Unit Test:
###################################################################################
peakPruningClean:
	@echo 'Cleaning the peakPruningTest objects'
	@rm -f $(MMWAVELIB_peakPruning_TEST_OBJECTS) $(MMWAVELIB_peakPruning_TEST_OUT) $(MMWAVELIB_peakPruning_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_peakPruning_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_peakPruning_TEST_DEPENDS)
-include $(lib_peakPruning_DEBUG_DEPENDS)
