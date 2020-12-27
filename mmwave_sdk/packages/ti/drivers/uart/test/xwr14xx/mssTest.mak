###################################################################################
# UART Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The UART Unit test requires additional libraries
###################################################################################
UART_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)										\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
UART_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)										\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib		\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib			\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
UART_UNIT_TEST_CFG	 	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
UART_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
UART_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
UART_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_uart_mss.map
UART_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_uart_mss.$(R4F_EXE_EXT)
UART_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_uart_mss.bin
UART_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_uart_linker.cmd
UART_UNIT_TEST_SOURCES   = $(UART_DRV_SOURCES) 		\
						   main.c 					\
						   uart_test.c       		\
						   uart_echo.c
UART_UNIT_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(UART_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
UART_UNIT_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(UART_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
uartRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(UART_UNIT_TEST_CONFIGPKG) $(UART_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(UART_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories uartRTSC $(UART_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(UART_UNIT_TEST_LOC_LIBS) $(UART_UNIT_TEST_STD_LIBS) 	\
	-l$(UART_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(UART_UNIT_TEST_MAP) 		\
	$(UART_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(UART_UNIT_TEST_APP_CMD) 	\
	$(R4F_LD_RTS_FLAGS) -o $(UART_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the UART MSS Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the UART MSS Unit Test objects'
	@$(DEL) $(UART_UNIT_TEST_OBJECTS) $(UART_UNIT_TEST_OUT) $(UART_UNIT_TEST_BIN)
	@$(DEL) $(UART_UNIT_TEST_MAP) $(UART_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the UART MSS Unit RTSC package'
	@$(DEL) $(UART_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(UART_UNIT_TEST_DEPENDS)

