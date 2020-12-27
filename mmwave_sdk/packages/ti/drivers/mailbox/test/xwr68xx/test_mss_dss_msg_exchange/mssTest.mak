###################################################################################
# Mailbox Unit Test on MSS
###################################################################################
.PHONY: mssMsgExchangeTest mssMsgExchangeTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/

###################################################################################
# The Mailbox Unit Test does not require any additional libraries
###################################################################################
MAILBOX_MSS_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB) \
						 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MAILBOX_MSS_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB) \
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
MAILBOX_MSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/mss.cfg
MAILBOX_MSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MAILBOX_MSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.map
MAILBOX_MSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.$(R4F_EXE_EXT)
MAILBOX_MSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.bin
MAILBOX_MSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/test_mss_dss_msg_exchange/mss_mailbox_linker.cmd
MAILBOX_MSS_TEST_SOURCES   = $(MAILBOX_DRV_SOURCES) main_mss.c
MAILBOX_MSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_MSS_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
MAILBOX_MSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_MSS_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssRTSC: 
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MAILBOX_MSS_TEST_CONFIGPKG) $(MAILBOX_MSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssMsgExchangeTest: BUILD_CONFIGPKG=$(MAILBOX_MSS_TEST_CONFIGPKG)
mssMsgExchangeTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssMsgExchangeTest: buildDirectories mssRTSC $(MAILBOX_MSS_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MAILBOX_MSS_TEST_LOC_LIBS) $(MAILBOX_MSS_TEST_STD_LIBS) 	\
	-l$(MAILBOX_MSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MAILBOX_MSS_TEST_MAP) 		\
	$(MAILBOX_MSS_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(MAILBOX_MSS_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(MAILBOX_MSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Mailbox MSS Msg Exchange Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssMsgExchangeTestClean:
	@echo 'Cleaning the Mailbox MSS Msg Exchange Test objects'
	@$(DEL) $(MAILBOX_MSS_TEST_OBJECTS) $(MAILBOX_MSS_TEST_OUT) $(MAILBOX_MSS_TEST_BIN)
	@$(DEL) $(MAILBOX_MSS_TEST_MAP) $(MAILBOX_MSS_TEST_DEPENDS)
	@echo 'Cleaning the Mailbox MSS Msg Exchange RTSC package'
	@$(DEL) $(MAILBOX_MSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MAILBOX_MSS_TEST_DEPENDS)

