###################################################################################
# ADCBUF Unit Test on MSS
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The ADCBUF Unit Test does not require any additional libraries
###################################################################################
ADCBUF_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)    \
			     -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  \
                             -llibsoc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)   \
                             -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

ADCBUF_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)    \
			    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib   \
                            -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/soc/lib    \
			    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
ADCBUF_UNIT_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
ADCBUF_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
ADCBUF_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
ADCBUF_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_adcbuf_mss.map
ADCBUF_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_adcbuf_mss.$(R4F_EXE_EXT)
ADCBUF_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_adcbuf_mss.bin
ADCBUF_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_adcbuf_linker.cmd
ADCBUF_UNIT_TEST_SOURCES   = $(ADCBUF_DRV_SOURCES) main_mss.c test_adcbuf.c
ADCBUF_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
ADCBUF_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(ADCBUF_UNIT_TEST_CONFIGPKG) $(ADCBUF_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(ADCBUF_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories mssRTSC $(ADCBUF_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(ADCBUF_UNIT_TEST_LOC_LIBS) $(ADCBUF_UNIT_TEST_STD_LIBS) 		\
	-l$(ADCBUF_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(ADCBUF_UNIT_TEST_MAP) 			\
	$(ADCBUF_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(ADCBUF_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(ADCBUF_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the ADCBUF MSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the ADCBUF MSS Unit Test objects'
	@$(DEL) $(ADCBUF_UNIT_TEST_OBJECTS)
	@$(DEL) $(ADCBUF_UNIT_TEST_DEPENDS)
	@$(DEL) $(ADCBUF_UNIT_TEST_OUT) $(ADCBUF_UNIT_TEST_BIN) $(ADCBUF_UNIT_TEST_MAP) $(ADCBUF_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the ADCBUF MSS Unit RTSC package'
	@$(DEL) $(ADCBUF_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(ADCBUF_UNIT_TEST_DEPENDS)

