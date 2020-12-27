###################################################################################
# UART Unit Test on DSS
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
UART_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)										\
					     -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
					     -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
UART_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)									\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib		\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
UART_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
UART_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
UART_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
UART_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_uart_dss.map
UART_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_uart_dss.$(C674_EXE_EXT)
UART_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_uart_dss.bin
UART_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_uart_linker.cmd
UART_DSS_TEST_SOURCES   = $(UART_DRV_SOURCES) 		\
						  main_dss.c				\
						  uart_test.c
UART_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(UART_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
UART_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(UART_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(UART_DSS_TEST_CONFIGPKG) $(UART_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(UART_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(UART_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(UART_DSS_TEST_LOC_LIBS) $(UART_DSS_TEST_STD_LIBS) 						\
	-l$(UART_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(UART_DSS_TEST_MAP) $(UART_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(UART_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(UART_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the UART DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the UART DSS Unit Test objects'
	@$(DEL) $(UART_DSS_TEST_OBJECTS) $(UART_DSS_TEST_OUT)
	@$(DEL) $(UART_DSS_TEST_BIN) $(UART_DSS_TEST_DEPENDS)
	@$(DEL) $(UART_DSS_TEST_MAP)
	@echo 'Cleaning the UART DSS Unit RTSC package'
	@$(DEL) $(UART_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(UART_DSS_TEST_DEPENDS)

