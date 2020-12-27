###################################################################################
# GPIO Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The GPIO Unit Test uses the common libraries + Test Logger Library
###################################################################################
GPIO_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibgpio_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
						  -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
GPIO_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib		\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/gpio/lib		\
						 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
GPIO_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
GPIO_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
GPIO_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
GPIO_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_gpio_mss.map
GPIO_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_gpio_mss.$(R4F_EXE_EXT)
GPIO_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_gpio_mss.bin
GPIO_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_gpio_linker.cmd
GPIO_UNIT_TEST_SOURCES   = $(GPIO_DRV_SOURCES) 		\
						   main.c 					\
						   gpio_test.c
GPIO_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(GPIO_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
GPIO_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(GPIO_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(GPIO_UNIT_TEST_CONFIGPKG) $(GPIO_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(GPIO_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories mssRTSC $(GPIO_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(GPIO_UNIT_TEST_LOC_LIBS) $(GPIO_UNIT_TEST_STD_LIBS) 	\
	-l$(GPIO_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(GPIO_UNIT_TEST_MAP) 		\
	$(GPIO_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(GPIO_UNIT_TEST_APP_CMD)	\
	$(R4F_LD_RTS_FLAGS) -o $(GPIO_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the GPIO MSS Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the GPIO MSS Unit Test objects'
	@$(DEL) $(GPIO_UNIT_TEST_OBJECTS) $(GPIO_UNIT_TEST_OUT) $(GPIO_UNIT_TEST_BIN)
	@$(DEL) $(GPIO_UNIT_TEST_MAP) $(GPIO_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the GPIO MSS Unit RTSC package'
	@$(DEL) $(GPIO_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(GPIO_UNIT_TEST_DEPENDS)

