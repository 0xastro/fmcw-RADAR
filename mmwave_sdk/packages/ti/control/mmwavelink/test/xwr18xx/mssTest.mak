###################################################################################
# mmWave Link Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The mmWave Link Unit Test requires additional libraries
###################################################################################
MMWAVE_MSS_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
		   						-llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 		\
		   						-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   						-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   						-llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)    \
                                -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)    \
		   						-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MMWAVE_MSS_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB) 									\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib 		\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib	   \
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib		\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib		\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_MSS_UNIT_TEST_CFG		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
MMWAVE_MSS_UNIT_TEST_CMD		 = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_MSS_UNIT_TEST_CONFIGPKG	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_MSS_UNIT_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_mss.map
MMWAVE_MSS_UNIT_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_mss.$(R4F_EXE_EXT)
MMWAVE_MSS_UNIT_TEST_BIN		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_mss.bin
MMWAVE_MSS_UNIT_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_link_linker.cmd
MMWAVE_MSS_UNIT_TEST_SOURCES	 = main_mss.c link_test.c osi_tirtos.c link_testcase_config.c
MMWAVE_MSS_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MSS_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
MMWAVE_MSS_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MSS_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
linkRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MMWAVE_MSS_UNIT_TEST_CONFIGPKG) $(MMWAVE_MSS_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(MMWAVE_MSS_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories linkRTSC $(MMWAVE_MSS_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MMWAVE_MSS_UNIT_TEST_LOC_LIBS) $(MMWAVE_MSS_UNIT_TEST_STD_LIBS) 	\
	-l$(MMWAVE_MSS_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVE_MSS_UNIT_TEST_MAP) 		\
	$(MMWAVE_MSS_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(MMWAVE_MSS_UNIT_TEST_APP_CMD) 	\
	$(R4F_LD_RTS_FLAGS) -o $(MMWAVE_MSS_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Link MSS Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the Link MSS Unit Test objects'
	@$(DEL) $(MMWAVE_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_MSS_UNIT_TEST_OUT) $(MMWAVE_MSS_UNIT_TEST_BIN)
	@$(DEL) $(MMWAVE_MSS_UNIT_TEST_MAP) $(MMWAVE_MSS_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the Link MSS Unit RTSC package'
	@$(DEL) $(MMWAVE_MSS_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_MSS_UNIT_TEST_DEPENDS)

