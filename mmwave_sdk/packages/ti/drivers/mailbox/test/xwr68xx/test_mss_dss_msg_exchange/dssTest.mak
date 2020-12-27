###################################################################################
# Mailbox Unit Test on DSS
###################################################################################
.PHONY: dssMsgExchangeTest dssMsgExchangeTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange

###################################################################################
# The Mailbox unit test does not require any additional libraries
###################################################################################
MAILBOX_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB) \
					    -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

MAILBOX_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB) \
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
MAILBOX_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/dss.cfg
MAILBOX_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MAILBOX_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.map
MAILBOX_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.$(C674_EXE_EXT)
MAILBOX_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.bin
MAILBOX_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/dss_mailbox_linker.cmd
MAILBOX_DSS_TEST_SOURCES   = $(MAILBOX_DRV_SOURCES) main_dss.c
MAILBOX_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MAILBOX_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC: 
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MAILBOX_DSS_TEST_CONFIGPKG) $(MAILBOX_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssMsgExchangeTest: BUILD_CONFIGPKG=$(MAILBOX_DSS_TEST_CONFIGPKG)
dssMsgExchangeTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssMsgExchangeTest: buildDirectories dssRTSC $(MAILBOX_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(MAILBOX_DSS_TEST_LOC_LIBS) $(MAILBOX_DSS_TEST_STD_LIBS) \
	-l$(MAILBOX_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MAILBOX_DSS_TEST_MAP) $(MAILBOX_DSS_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MAILBOX_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS)  -lrts6740_elf.lib -o $(MAILBOX_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Mailbox DSS Message Exchange Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssMsgExchangeTestClean:
	@echo 'Cleaning the Mailbox DSS Msg Exchange Test objects'
	@$(DEL) $(MAILBOX_DSS_TEST_OBJECTS) $(MAILBOX_DSS_TEST_OUT)
	@$(DEL) $(MAILBOX_DSS_TEST_BIN) $(MAILBOX_DSS_TEST_DEPENDS)
	@$(DEL) $(MAILBOX_DSS_TEST_MAP)
	@echo 'Cleaning the Mailbox DSS Msg Exchange RTSC package'
	@$(DEL) $(MAILBOX_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MAILBOX_DSS_TEST_DEPENDS)

