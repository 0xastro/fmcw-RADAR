###################################################################################
# CBUFF Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
CBUFF_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						   -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)	\
		   				   -llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   				   -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

CBUFF_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)								\
		   				   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
		   				   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib		\
		   				   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib

###################################################################################
# Enabling Debug Support
###################################################################################
R4F_CFLAGS  += --define=DebugP_LOG_ENABLED

###################################################################################
# Unit Test Files
###################################################################################
CBUFF_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
CBUFF_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
CBUFF_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CBUFF_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_cbuff_mss.map
CBUFF_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_cbuff_mss.$(R4F_EXE_EXT)
CBUFF_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_cbuff_mss.bin
CBUFF_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_cbuff_linker.cmd
CBUFF_UNIT_TEST_SOURCES   = $(CBUFF_DRV_SOURCES) main.c test_common.c
CBUFF_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CBUFF_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
CBUFF_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CBUFF_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(CBUFF_UNIT_TEST_CONFIGPKG) $(CBUFF_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(CBUFF_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories mssRTSC $(CBUFF_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(CBUFF_UNIT_TEST_LOC_LIBS) $(CBUFF_UNIT_TEST_STD_LIBS) 		\
	-l$(CBUFF_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(CBUFF_UNIT_TEST_MAP) 			\
	$(CBUFF_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(CBUFF_UNIT_TEST_APP_CMD) 			\
	$(R4F_LD_RTS_FLAGS) -o $(CBUFF_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the CBUFF MSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the CBUFF MSS Unit Test objects'
	@$(DEL) $(CBUFF_UNIT_TEST_OBJECTS) $(CBUFF_UNIT_TEST_OUT) $(CBUFF_UNIT_TEST_BIN)
	@$(DEL) $(CBUFF_UNIT_TEST_MAP) $(CBUFF_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the CBUFF MSS Unit RTSC package'
	@$(DEL) $(CBUFF_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CBUFF_UNIT_TEST_DEPENDS)

