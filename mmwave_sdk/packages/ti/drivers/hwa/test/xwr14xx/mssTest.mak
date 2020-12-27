###################################################################################
# HWA Unit Test on R4 Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/common

###################################################################################
# The HWA Unit Test uses the common libraries + Test Logger Library
###################################################################################
HWA_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
HWA_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
HWA_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
HWA_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWA_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
HWA_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_hwa_mss.map
HWA_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_hwa_mss.$(R4F_EXE_EXT)
HWA_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_hwa_mss.bin
HWA_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_hwa_linker.cmd
HWA_UNIT_TEST_SOURCES   = $(HWA_DRV_SOURCES) main.c
HWA_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
HWA_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
r4.cfg: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(HWA_UNIT_TEST_CONFIGPKG) $(HWA_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(HWA_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories r4.cfg $(HWA_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(HWA_UNIT_TEST_LOC_LIBS) $(HWA_UNIT_TEST_STD_LIBS) 	\
	-l$(HWA_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(HWA_UNIT_TEST_MAP) 		\
	$(HWA_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(HWA_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(HWA_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the HWA R4 Unit Test OUT and BIN Formats'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the HWA R4 Unit Test objects'
	@$(DEL) $(HWA_UNIT_TEST_OBJECTS) $(HWA_UNIT_TEST_OUT) $(HWA_UNIT_TEST_BIN)
	@$(DEL) $(HWA_UNIT_TEST_MAP) $(R4_DEPENDS)
	@echo 'Cleaning the HWA R4 Unit RTSC package'
	@$(DEL) $(HWA_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWA_UNIT_TEST_DEPENDS)

