###################################################################################
# EDMA Unit Test on R4 Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# EDMA Unit Test does not require any additional libraries
###################################################################################
EDMA_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB) \
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
EDMA_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB) \
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)

###################################################################################
# Unit Test Files
###################################################################################
EDMA_UNIT_TEST_CFG	     = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
EDMA_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
EDMA_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/r4f_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
EDMA_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_edma_mss.map
EDMA_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_edma_mss.$(R4F_EXE_EXT)
EDMA_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_edma_mss.bin
EDMA_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_edma_linker.cmd
EDMA_UNIT_TEST_SOURCES   = $(EDMA_DRV_SOURCES) ../main.c
EDMA_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(EDMA_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
EDMA_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(EDMA_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
edmaRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(EDMA_UNIT_TEST_CONFIGPKG) $(EDMA_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(EDMA_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories edmaRTSC $(EDMA_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(EDMA_UNIT_TEST_LOC_LIBS) $(EDMA_UNIT_TEST_STD_LIBS) 		\
	-l$(EDMA_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(EDMA_UNIT_TEST_MAP) 			\
	$(EDMA_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(EDMA_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(EDMA_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the EDMA R4 Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the edma R4 Unit Test objects'
	@$(DEL) $(EDMA_UNIT_TEST_OBJECTS) $(EDMA_UNIT_TEST_OUT) $(EDMA_UNIT_TEST_BIN)
	@$(DEL) $(EDMA_UNIT_TEST_MAP) $(EDMA_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the edma R4 Unit RTSC package'
	@$(DEL) $(EDMA_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(EDMA_UNIT_TEST_DEPENDS)

