###################################################################################
# EDMA Unit Test on DSS
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The EDMA unit test does not require any additional libraries
###################################################################################
EDMA_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB) \
                         -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
EDMA_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB) \
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
EDMA_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
EDMA_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
EDMA_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
EDMA_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_edma_dss.map
EDMA_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_edma_dss.$(C674_EXE_EXT)
EDMA_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_edma_dss.bin
EDMA_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_edma_linker.cmd
EDMA_DSS_TEST_SOURCES   = $(EDMA_DRV_SOURCES) ../main.c
EDMA_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(EDMA_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
EDMA_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(EDMA_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(EDMA_DSS_TEST_CONFIGPKG) $(EDMA_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(EDMA_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(EDMA_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(EDMA_DSS_TEST_LOC_LIBS) $(EDMA_DSS_TEST_STD_LIBS) 						\
	-l$(EDMA_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(EDMA_DSS_TEST_MAP) $(EDMA_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(EDMA_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(EDMA_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the EDMA DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the EDMA DSS Unit Test objects'
	@$(DEL) $(EDMA_DSS_TEST_OBJECTS) $(EDMA_DSS_TEST_OUT)
	@$(DEL) $(EDMA_DSS_TEST_BIN) $(EDMA_DSS_TEST_DEPENDS)
	@$(DEL) $(EDMA_DSS_TEST_MAP)
	@echo 'Cleaning the EDMA DSS Unit RTSC package'
	@$(DEL) $(EDMA_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(EDMA_DSS_TEST_DEPENDS)

