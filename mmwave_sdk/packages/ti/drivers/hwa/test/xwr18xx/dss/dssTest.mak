###################################################################################
# HWA Unit Test on DSP Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/common

###################################################################################
# The HWA Unit Test uses the common libraries + Test Logger Library
###################################################################################
HWA_DSS_UNIT_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)									\
						 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
HWA_DSS_UNIT_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)									\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
HWA_DSS_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss/dss.cfg
HWA_DSS_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWA_DSS_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
HWA_DSS_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss/$(MMWAVE_SDK_DEVICE_TYPE)_hwa_dss.map
HWA_DSS_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss/$(MMWAVE_SDK_DEVICE_TYPE)_hwa_dss.$(C674_EXE_EXT)
HWA_DSS_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss/$(MMWAVE_SDK_DEVICE_TYPE)_hwa_dss.bin
HWA_DSS_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss/dss_hwa_linker.cmd
HWA_DSS_UNIT_TEST_SOURCES   = $(HWA_DRV_SOURCES) main.c
HWA_DSS_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DSS_UNIT_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
HWA_DSS_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DSS_UNIT_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(HWA_DSS_UNIT_TEST_CONFIGPKG) $(HWA_DSS_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(HWA_DSS_UNIT_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(HWA_DSS_UNIT_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(HWA_DSS_UNIT_TEST_LOC_LIBS) $(HWA_DSS_UNIT_TEST_STD_LIBS) 	\
	-l$(HWA_DSS_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(HWA_DSS_UNIT_TEST_MAP) 		\
	$(HWA_DSS_UNIT_TEST_OBJECTS) $(PLATFORM_C674X_LINK_CMD) $(HWA_DSS_UNIT_TEST_APP_CMD) 		\
	$(C674_LD_RTS_FLAGS)  -lrts6740_elf.lib -o $(HWA_DSS_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the HWA DSS Unit Test OUT & BIN Formats'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the HWA DSS Unit Test objects'
	@$(DEL) $(HWA_DSS_UNIT_TEST_OBJECTS) $(HWA_DSS_UNIT_TEST_OUT) $(HWA_DSS_UNIT_TEST_BIN)
	@$(DEL) $(HWA_DSS_UNIT_TEST_MAP) $(C674_DEPENDS)
	@echo 'Cleaning the HWA DSS Unit RTSC package'
	@$(DEL) $(HWA_DSS_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWA_DSS_UNIT_TEST_DEPENDS)

