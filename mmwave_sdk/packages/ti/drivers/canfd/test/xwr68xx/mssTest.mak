###################################################################################
# CANFD Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)

###################################################################################
# The CANFD Unit Test uses the common libraries + Test Logger Library
###################################################################################
CANFD_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
CANFD_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
CANFD_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
CANFD_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
CANFD_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CANFD_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_canfd_mss.map
CANFD_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_canfd_mss.$(R4F_EXE_EXT)
CANFD_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_canfd_mss.bin
CANFD_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_canfd_linker.cmd
CANFD_UNIT_TEST_SOURCES   = $(CANFD_DRV_SOURCES) main.c
CANFD_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CANFD_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
CANFD_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CANFD_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
canfdRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(CANFD_UNIT_TEST_CONFIGPKG) $(CANFD_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(CANFD_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories canfdRTSC $(CANFD_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(CANFD_UNIT_TEST_LOC_LIBS) $(CANFD_UNIT_TEST_STD_LIBS)	\
	-l$(CANFD_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(CANFD_UNIT_TEST_MAP) 		\
	$(CANFD_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(CANFD_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(CANFD_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the CANFD R4 Unit Test OUT file'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the CANFD R4 Unit Test objects'
	@$(DEL) $(CANFD_UNIT_TEST_OBJECTS) $(CANFD_UNIT_TEST_OUT) $(CANFD_UNIT_TEST_BIN)
	@$(DEL) $(CANFD_UNIT_TEST_MAP) $(CANFD_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the CANFD R4 Unit RTSC package'
	@$(DEL) $(CANFD_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CANFD_UNIT_TEST_DEPENDS)

