###################################################################################
# Watchdog Unit Test on DSS
###################################################################################
.PHONY: dssWatchdogTest dssWatchdogTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test

###################################################################################
# The Watchdog Unit test requires additional libraries
###################################################################################
WATCHDOG_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)								\
					     -llibesm_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
					     -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
WATCHDOG_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)								\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/esm/lib		\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
WATCHDOG_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
WATCHDOG_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
WATCHDOG_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
WATCHDOG_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_watchdog_dss.map
WATCHDOG_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_watchdog_dss.$(C674_EXE_EXT)
WATCHDOG_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_watchdog_dss.bin
WATCHDOG_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_watchdog_linker.cmd
WATCHDOG_DSS_TEST_SOURCES   = $(WATCHDOG_DRV_SOURCES) main_dss.c
WATCHDOG_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(WATCHDOG_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
WATCHDOG_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(WATCHDOG_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(WATCHDOG_DSS_TEST_CONFIGPKG) $(WATCHDOG_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssWatchdogTest: BUILD_CONFIGPKG=$(WATCHDOG_DSS_TEST_CONFIGPKG)
dssWatchdogTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssWatchdogTest: buildDirectories dssRTSC $(WATCHDOG_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(WATCHDOG_DSS_TEST_LOC_LIBS) $(WATCHDOG_DSS_TEST_STD_LIBS) 						\
	-l$(WATCHDOG_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(WATCHDOG_DSS_TEST_MAP) $(WATCHDOG_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(WATCHDOG_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(WATCHDOG_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Watchdog DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssWatchdogTestClean:
	@echo 'Cleaning the Watchdog DSS Unit Test objects'
	@$(DEL) $(WATCHDOG_DSS_TEST_OBJECTS) $(WATCHDOG_DSS_TEST_OUT)
	@$(DEL) $(WATCHDOG_DSS_TEST_BIN) $(WATCHDOG_DSS_TEST_DEPENDS)
	@$(DEL) $(WATCHDOG_DSS_TEST_MAP)
	@echo 'Cleaning the Watchdog DSS Unit RTSC package'
	@$(DEL) $(WATCHDOG_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(WATCHDOG_DSS_TEST_DEPENDS)

