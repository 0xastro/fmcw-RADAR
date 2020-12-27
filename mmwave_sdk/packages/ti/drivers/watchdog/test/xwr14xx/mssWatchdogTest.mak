###################################################################################
# Watchdog Unit Test on MSS Makefile
###################################################################################
.PHONY: mssWatchdogTest mssWatchdogTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test

###################################################################################
# The Watchdog Unit Test uses the common libraries + Test Logger Library
###################################################################################
WATCHDOG_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
WATCHDOG_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
WATCHDOG_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
WATCHDOG_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
WATCHDOG_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_watchdog_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
WATCHDOG_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_watchdog_mss.map
WATCHDOG_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_watchdog_mss.$(R4F_EXE_EXT)
WATCHDOG_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_watchdog_mss.bin
WATCHDOG_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_watchdog_linker.cmd
WATCHDOG_UNIT_TEST_SOURCES   = $(WATCHDOG_DRV_SOURCES) main.c
WATCHDOG_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(WATCHDOG_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
WATCHDOG_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(WATCHDOG_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
watchdogRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(WATCHDOG_UNIT_TEST_CONFIGPKG) $(WATCHDOG_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssWatchdogTest: BUILD_CONFIGPKG=$(WATCHDOG_UNIT_TEST_CONFIGPKG)
mssWatchdogTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssWatchdogTest: buildDirectories watchdogRTSC $(WATCHDOG_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(WATCHDOG_UNIT_TEST_LOC_LIBS) $(WATCHDOG_UNIT_TEST_STD_LIBS)	\
	-l$(WATCHDOG_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(WATCHDOG_UNIT_TEST_MAP) 		\
	$(WATCHDOG_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(WATCHDOG_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(WATCHDOG_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Watchdog R4 Unit Test OUT file'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssWatchdogTestClean:
	@echo 'Cleaning the Watchdog R4 Unit Test objects'
	@$(DEL) $(WATCHDOG_UNIT_TEST_OBJECTS) $(WATCHDOG_UNIT_TEST_OUT) $(WATCHDOG_UNIT_TEST_BIN)
	@$(DEL) $(WATCHDOG_UNIT_TEST_MAP) $(WATCHDOG_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the Watchdog R4 Unit RTSC package'
	@$(DEL) $(WATCHDOG_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(WATCHDOG_UNIT_TEST_DEPENDS)

