###################################################################################
# Mailbox Unit Test on DSS
###################################################################################
.PHONY: dssStandaloneTest dssStandaloneTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone

###################################################################################
# The Mailbox unit test does not require any additional libraries
###################################################################################
MAILBOX_DSS_STANDALONE_TEST_STD_LIBS = $(C674_COMMON_STD_LIB) \
					    -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
MAILBOX_DSS_STANDALONE_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB) \
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
MAILBOX_DSS_STANDALONE_TEST_CFG	      = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/dss.cfg
MAILBOX_DSS_STANDALONE_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/dss_standalone_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MAILBOX_DSS_STANDALONE_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss_standalone.map
MAILBOX_DSS_STANDALONE_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss_standalone.$(C674_EXE_EXT)
MAILBOX_DSS_STANDALONE_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss_standalone.bin
MAILBOX_DSS_STANDALONE_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_dss_standalone/dss_mailbox_linker.cmd
MAILBOX_DSS_STANDALONE_TEST_SOURCES   = $(MAILBOX_DRV_SOURCES) main_dss_standalone.c
MAILBOX_DSS_STANDALONE_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_DSS_STANDALONE_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MAILBOX_DSS_STANDALONE_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_DSS_STANDALONE_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssStandaloneRTSC: 
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MAILBOX_DSS_STANDALONE_TEST_CONFIGPKG) $(MAILBOX_DSS_STANDALONE_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssStandaloneTest: BUILD_CONFIGPKG=$(MAILBOX_DSS_STANDALONE_TEST_CONFIGPKG)
dssStandaloneTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssStandaloneTest: buildDirectories dssStandaloneRTSC $(MAILBOX_DSS_STANDALONE_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(MAILBOX_DSS_STANDALONE_TEST_LOC_LIBS) $(MAILBOX_DSS_STANDALONE_TEST_STD_LIBS) \
	-l$(MAILBOX_DSS_STANDALONE_TEST_CONFIGPKG)/linker.cmd --map_file=$(MAILBOX_DSS_STANDALONE_TEST_MAP) $(MAILBOX_DSS_STANDALONE_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MAILBOX_DSS_STANDALONE_TEST_APP_CMD) $(C674_LD_RTS_FLAGS)  -lrts6740_elf.lib -o $(MAILBOX_DSS_STANDALONE_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Mailbox DSS Standalone Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssStandaloneTestClean:
	@echo 'Cleaning the Mailbox DSS Standalone Unit Test objects'
	@$(DEL) $(MAILBOX_DSS_STANDALONE_TEST_OBJECTS) $(MAILBOX_DSS_STANDALONE_TEST_OUT)
	@$(DEL) $(MAILBOX_DSS_STANDALONE_TEST_BIN) $(MAILBOX_DSS_STANDALONE_TEST_DEPENDS)
	@$(DEL) $(MAILBOX_DSS_STANDALONE_TEST_MAP)
	@echo 'Cleaning the Mailbox DSS Standalone Unit RTSC package'
	@$(DEL) $(MAILBOX_DSS_STANDALONE_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MAILBOX_DSS_STANDALONE_TEST_DEPENDS)

