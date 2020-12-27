###################################################################################
#	mmWave Link Unit Test on MSS Makefile
###################################################################################
.PHONY: mssMinimalTest mssMinimalTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal

###################################################################################
# The mmWave Unit Test requires additional libraries
###################################################################################
MMWAVE_MINIMAL_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)							\
		   					-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   					-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   					-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   					-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   					-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

MMWAVE_MINIMAL_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)							\
						   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib		\
						   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
						   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib		\
						   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib	\
						   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_MINIMAL_UNIT_TEST_CFG		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/mss.cfg
MMWAVE_MINIMAL_UNIT_TEST_CMD		 = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_MINIMAL_UNIT_TEST_CONFIGPKG	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_MINIMAL_UNIT_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_minimal_mss.map
MMWAVE_MINIMAL_UNIT_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_minimal_mss.$(R4F_EXE_EXT)
MMWAVE_MINIMAL_UNIT_TEST_BIN		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_minimal_mss.bin
MMWAVE_MINIMAL_UNIT_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/minimal/mss_mmwave_linker.cmd
MMWAVE_MINIMAL_UNIT_TEST_SOURCES	 = $(MMWAVE_SOURCES) main_min.c
MMWAVE_MINIMAL_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MINIMAL_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
MMWAVE_MINIMAL_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MINIMAL_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mmwaveMinimalRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MMWAVE_MINIMAL_UNIT_TEST_CONFIGPKG) $(MMWAVE_MINIMAL_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssMinimalTest: BUILD_CONFIGPKG=$(MMWAVE_MINIMAL_UNIT_TEST_CONFIGPKG)
mssMinimalTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssMinimalTest: buildDirectories mmwaveMinimalRTSC $(MMWAVE_MINIMAL_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MMWAVE_MINIMAL_UNIT_TEST_LOC_LIBS) $(MMWAVE_MINIMAL_UNIT_TEST_STD_LIBS) \
	-l$(MMWAVE_MINIMAL_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVE_MINIMAL_UNIT_TEST_MAP) $(MMWAVE_MINIMAL_UNIT_TEST_OBJECTS) \
	$(PLATFORM_R4F_LINK_CMD) $(MMWAVE_MINIMAL_UNIT_TEST_APP_CMD) $(R4F_LD_RTS_FLAGS) -o $(MMWAVE_MINIMAL_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmWave Control Minimal MSS Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssMinimalTestClean:
	@echo 'Cleaning the mmWave Control Minimal MSS Unit Test objects'
	@$(DEL) $(MMWAVE_MINIMAL_UNIT_TEST_OBJECTS) $(MMWAVE_MINIMAL_UNIT_TEST_OUT) $(MMWAVE_MINIMAL_UNIT_TEST_BIN)
	@$(DEL) $(MMWAVE_MINIMAL_UNIT_TEST_MAP) $(MMWAVE_MINIMAL_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the mmWave Control Minimal MSS Unit RTSC package'
	@$(DEL) $(MMWAVE_MINIMAL_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_MINIMAL_UNIT_TEST_DEPENDS)

