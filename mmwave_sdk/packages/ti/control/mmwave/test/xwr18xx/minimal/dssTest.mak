###################################################################################
# mmWave Minimal Unit Test on DSS Makefile
###################################################################################
.PHONY: dssMinimalTest dssMinimalTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal

###################################################################################
# The mmWave Unit test requires additional libraries
###################################################################################
MMWAVE_MINIMAL_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)								\
		   						-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)			\
		   						-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
		   						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
		   						-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
								-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
MMWAVE_MINIMAL_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)								\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib		\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib	\
		   						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_MINIMAL_DSS_TEST_CFG	      = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/dss.cfg
MMWAVE_MINIMAL_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_MINIMAL_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_MINIMAL_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_minimal_dss.map
MMWAVE_MINIMAL_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_minimal_dss.$(C674_EXE_EXT)
MMWAVE_MINIMAL_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_minimal_dss.bin
MMWAVE_MINIMAL_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/dss_linker.cmd
MMWAVE_MINIMAL_DSS_TEST_SOURCES   = main_min_dss.c common_min.c
MMWAVE_MINIMAL_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MINIMAL_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVE_MINIMAL_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MINIMAL_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssMinimalRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVE_MINIMAL_DSS_TEST_CONFIGPKG) $(MMWAVE_MINIMAL_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssMinimalTest: BUILD_CONFIGPKG=$(MMWAVE_MINIMAL_DSS_TEST_CONFIGPKG)
dssMinimalTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssMinimalTest: buildDirectories dssMinimalRTSC $(MMWAVE_MINIMAL_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(MMWAVE_MINIMAL_DSS_TEST_LOC_LIBS) $(MMWAVE_MINIMAL_DSS_TEST_STD_LIBS) 						\
	-l$(MMWAVE_MINIMAL_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVE_MINIMAL_DSS_TEST_MAP) $(MMWAVE_MINIMAL_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVE_MINIMAL_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(MMWAVE_MINIMAL_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmWave Minimal DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssMinimalTestClean:
	@echo 'Cleaning the mmWave Minimal DSS Unit Test objects'
	@$(DEL) $(MMWAVE_MINIMAL_DSS_TEST_OBJECTS) $(MMWAVE_MINIMAL_DSS_TEST_OUT)
	@$(DEL) $(MMWAVE_MINIMAL_DSS_TEST_BIN) $(MMWAVE_MINIMAL_DSS_TEST_DEPENDS)
	@$(DEL) $(MMWAVE_MINIMAL_DSS_TEST_MAP)
	@echo 'Cleaning the mmWave Minimal DSS Unit RTSC package'
	@$(DEL) $(MMWAVE_MINIMAL_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_MINIMAL_DSS_TEST_DEPENDS)

