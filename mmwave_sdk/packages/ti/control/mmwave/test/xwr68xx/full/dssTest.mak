###################################################################################
# mmWave Full Unit Test on DSS Makefile
###################################################################################
.PHONY: dssFullTest dssFullTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)/full

###################################################################################
# The mmWave Unit test requires additional libraries
###################################################################################
MMWAVE_FULL_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)									\
		   						-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)			\
		   						-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
		   						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
		   						-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
								-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
MMWAVE_FULL_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)									\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib		\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib	\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_FULL_DSS_TEST_CFG	      = test/$(MMWAVE_SDK_DEVICE_TYPE)/full/dss.cfg
MMWAVE_FULL_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_FULL_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/full/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_FULL_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_dss.map
MMWAVE_FULL_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_dss.$(C674_EXE_EXT)
MMWAVE_FULL_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_dss.bin
MMWAVE_FULL_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/full/dss_linker.cmd
MMWAVE_FULL_DSS_TEST_SOURCES   = main_full_dss.c common_full.c
MMWAVE_FULL_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_FULL_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVE_FULL_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_FULL_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssFullRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVE_FULL_DSS_TEST_CONFIGPKG) $(MMWAVE_FULL_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssFullTest: BUILD_CONFIGPKG=$(MMWAVE_FULL_DSS_TEST_CONFIGPKG)
dssFullTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssFullTest: buildDirectories dssFullRTSC $(MMWAVE_FULL_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(MMWAVE_FULL_DSS_TEST_LOC_LIBS) $(MMWAVE_FULL_DSS_TEST_STD_LIBS) 						\
	-l$(MMWAVE_FULL_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVE_FULL_DSS_TEST_MAP) $(MMWAVE_FULL_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVE_FULL_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(MMWAVE_FULL_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmWave Full DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssFullTestClean:
	@echo 'Cleaning the mmWave Full DSS Unit Test objects'
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_OBJECTS) $(MMWAVE_FULL_DSS_TEST_OUT)
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_BIN) $(MMWAVE_FULL_DSS_TEST_DEPENDS)
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_MAP)
	@echo 'Cleaning the mmWave Full DSS Unit RTSC package'
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_FULL_DSS_TEST_DEPENDS)

