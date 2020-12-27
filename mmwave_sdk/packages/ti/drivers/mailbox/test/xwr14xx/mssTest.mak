###################################################################################
# Mailbox Unit Test on R4 Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)

###################################################################################
# The Mailbox Unit Test does not require any additional libraries
###################################################################################
MAILBOX_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB) \
						 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MAILBOX_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB) \
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
MAILBOX_UNIT_TEST_CFG	    = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
MAILBOX_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MAILBOX_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MAILBOX_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.map
MAILBOX_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.$(R4F_EXE_EXT)
MAILBOX_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.bin
MAILBOX_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_mailbox_linker.cmd
MAILBOX_UNIT_TEST_SOURCES   = $(MAILBOX_DRV_SOURCES) main.c
MAILBOX_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
MAILBOX_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mailboxRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MAILBOX_UNIT_TEST_CONFIGPKG) $(MAILBOX_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(MAILBOX_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories mailboxRTSC $(MAILBOX_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MAILBOX_UNIT_TEST_LOC_LIBS) $(MAILBOX_UNIT_TEST_STD_LIBS) 	\
	-l$(MAILBOX_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(MAILBOX_UNIT_TEST_MAP) 		\
	$(MAILBOX_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(MAILBOX_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(MAILBOX_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Mailbox MSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the Mailbox MSS Unit Test objects'
	@$(DEL) $(MAILBOX_UNIT_TEST_OBJECTS) $(MAILBOX_UNIT_TEST_OUT) $(MAILBOX_UNIT_TEST_BIN)
	@$(DEL) $(MAILBOX_UNIT_TEST_MAP) $(MAILBOX_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the Mailbox MSS Unit RTSC package'
	@$(DEL) $(MAILBOX_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MAILBOX_UNIT_TEST_DEPENDS)

