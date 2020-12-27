###################################################################################
# mmWave Link Unit Test on DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The UART Unit test requires additional libraries
###################################################################################
MMWAVE_LINK_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)									\
		   						-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)			\
		   						-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
		   						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
		   						-llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
                                -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
		   						-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
MMWAVE_LINK_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)									\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib		\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib	   \
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib	   \
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_LINK_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVE_LINK_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_LINK_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_LINK_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_dss.map
MMWAVE_LINK_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_dss.$(C674_EXE_EXT)
MMWAVE_LINK_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_dss.bin
MMWAVE_LINK_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_link_linker.cmd
MMWAVE_LINK_DSS_TEST_SOURCES   = main_dss.c link_test.c osi_tirtos.c link_testcase_config.c
MMWAVE_LINK_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVE_LINK_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVE_LINK_DSS_TEST_CONFIGPKG) $(MMWAVE_LINK_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(MMWAVE_LINK_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(MMWAVE_LINK_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(MMWAVE_LINK_DSS_TEST_LOC_LIBS) $(MMWAVE_LINK_DSS_TEST_STD_LIBS) 						\
	-l$(MMWAVE_LINK_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVE_LINK_DSS_TEST_MAP) $(MMWAVE_LINK_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVE_LINK_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(MMWAVE_LINK_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Link DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the Link DSS Unit Test objects'
	@$(DEL) $(MMWAVE_LINK_DSS_TEST_OBJECTS) $(MMWAVE_LINK_DSS_TEST_OUT)
	@$(DEL) $(MMWAVE_LINK_DSS_TEST_BIN) $(MMWAVE_LINK_DSS_TEST_DEPENDS)
	@$(DEL) $(MMWAVE_LINK_DSS_TEST_MAP)
	@echo 'Cleaning the Link DSS Unit RTSC package'
	@$(DEL) $(MMWAVE_LINK_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_LINK_DSS_TEST_DEPENDS)

