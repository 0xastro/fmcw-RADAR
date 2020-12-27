###################################################################################
# ADCBUF Unit Test on DSS
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The ADCBUF unit test does not require any additional libraries
###################################################################################
ADCBUF_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)    \
  	    	           -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)  \
			   -llibsoc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
			   -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
ADCBUF_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)    \
			    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib   \
			    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/soc/lib    \
			    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
ADCBUF_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
ADCBUF_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
ADCBUF_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
ADCBUF_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_adcbuf_dss.map
ADCBUF_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_adcbuf_dss.$(C674_EXE_EXT)
ADCBUF_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_adcbuf_dss.bin
ADCBUF_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_adcbuf_linker.cmd
ADCBUF_DSS_TEST_SOURCES   = $(ADCBUF_DRV_SOURCES) main_dss.c test_adcbuf.c
ADCBUF_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
ADCBUF_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC: $(DSP_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(ADCBUF_DSS_TEST_CONFIGPKG) $(ADCBUF_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(ADCBUF_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(ADCBUF_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(ADCBUF_DSS_TEST_LOC_LIBS) $(ADCBUF_DSS_TEST_STD_LIBS) \
	-l$(ADCBUF_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(ADCBUF_DSS_TEST_MAP) $(ADCBUF_DSS_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(ADCBUF_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS)  -lrts6740_elf.lib -o $(ADCBUF_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the ADCBUF DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the ADCBUF DSS Unit Test objects'
	@$(DEL) $(ADCBUF_DSS_TEST_OBJECTS) $(ADCBUF_DSS_TEST_OUT)
	@$(DEL) $(ADCBUF_DSS_TEST_BIN) $(ADCBUF_DSS_TEST_DEPENDS)
	@$(DEL) $(ADCBUF_DSS_TEST_MAP)
	@echo 'Cleaning the ADCBUF DSS Unit RTSC package'
	@$(DEL) $(ADCBUF_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(ADCBUF_DSS_TEST_DEPENDS)

