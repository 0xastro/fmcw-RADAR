###################################################################################
# CBUFF Unit Test on DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
CBUFF_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)									\
					     -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
		   				 -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
		   				 -llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

CBUFF_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)								\
		   				 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
		   				 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib		\
		   				 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib

###################################################################################
# Enabling Debug Support
###################################################################################
C674_CFLAGS  += --define=DebugP_LOG_ENABLED

###################################################################################
# Unit Test Files
###################################################################################
CBUFF_DSS_TEST_CFG	     = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
CBUFF_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
CBUFF_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CBUFF_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_cbuff_dss.map
CBUFF_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_cbuff_dss.$(C674_EXE_EXT)
CBUFF_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_cbuff_dss.bin
CBUFF_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_cbuff_linker.cmd
CBUFF_DSS_TEST_SOURCES   = $(CBUFF_DRV_SOURCES) main_dss.c test_common.c
CBUFF_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CBUFF_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
CBUFF_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CBUFF_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(CBUFF_DSS_TEST_CONFIGPKG) $(CBUFF_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(CBUFF_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(CBUFF_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(CBUFF_DSS_TEST_LOC_LIBS) $(CBUFF_DSS_TEST_STD_LIBS) 						\
	-l$(CBUFF_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(CBUFF_DSS_TEST_MAP) $(CBUFF_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(CBUFF_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(CBUFF_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the CBUFF DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the CBUFF DSS Unit Test objects'
	@$(DEL) $(CBUFF_DSS_TEST_OBJECTS) $(CBUFF_DSS_TEST_OUT)
	@$(DEL) $(CBUFF_DSS_TEST_BIN) $(CBUFF_DSS_TEST_DEPENDS)
	@$(DEL) $(CBUFF_DSS_TEST_MAP)
	@echo 'Cleaning the CBUFF DSS Unit RTSC package'
	@$(DEL) $(CBUFF_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CBUFF_DSS_TEST_DEPENDS)

