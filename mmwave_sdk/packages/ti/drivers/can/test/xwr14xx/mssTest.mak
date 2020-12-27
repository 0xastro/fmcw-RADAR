###################################################################################
# CAN Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The CAN Unit Test uses the common libraries + Test Logger Library
###################################################################################
CAN_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
CAN_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
CAN_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
CAN_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
CAN_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CAN_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_can_mss.map
CAN_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_can_mss.$(R4F_EXE_EXT)
CAN_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_can_mss.bin
CAN_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_can_linker.cmd
CAN_UNIT_TEST_SOURCES   = $(CAN_DRV_SOURCES) main.c
CAN_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CAN_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
CAN_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CAN_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
canRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(CAN_UNIT_TEST_CONFIGPKG) $(CAN_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(CAN_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories canRTSC $(CAN_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(CAN_UNIT_TEST_LOC_LIBS) $(CAN_UNIT_TEST_STD_LIBS)	\
	-l$(CAN_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(CAN_UNIT_TEST_MAP) 		\
	$(CAN_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(CAN_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(CAN_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the CAN R4 Unit Test OUT file'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the CAN R4 Unit Test objects'
	@$(DEL) $(CAN_UNIT_TEST_OBJECTS) $(CAN_UNIT_TEST_OUT) $(CAN_UNIT_TEST_BIN)
	@$(DEL) $(CAN_UNIT_TEST_MAP) $(CAN_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the CAN R4 Unit RTSC package'
	@$(DEL) $(CAN_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CAN_UNIT_TEST_DEPENDS)

