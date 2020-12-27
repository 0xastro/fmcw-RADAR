###################################################################################
# I2C Slave Unit Test on MSS Makefile
###################################################################################
.PHONY: mssSlaveTest mssSlaveTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test

###################################################################################
# The I2C Slave Unit Test uses the common libraries + Test Logger Library
###################################################################################
I2CSLAVE_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
I2CSLAVE_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
I2CSLAVE_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
I2CSLAVE_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
I2CSLAVE_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_slave_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
I2CSLAVE_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_i2c_mss_slave.map
I2CSLAVE_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_i2c_mss_slave.$(R4F_EXE_EXT)
I2CSLAVE_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_i2c_mss_slave.bin
I2CSLAVE_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_i2c_linker.cmd
I2CSLAVE_UNIT_TEST_SOURCES   = $(I2CSLAVE_DRV_SOURCES) slave.c
I2CSLAVE_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(I2CSLAVE_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
I2CSLAVE_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(I2CSLAVE_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
i2cSlaveRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(I2CSLAVE_UNIT_TEST_CONFIGPKG) $(I2CSLAVE_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssSlaveTest: BUILD_CONFIGPKG=$(I2CSLAVE_UNIT_TEST_CONFIGPKG)
mssSlaveTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssSlaveTest: buildDirectories i2cSlaveRTSC $(I2CSLAVE_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(I2CSLAVE_UNIT_TEST_LOC_LIBS) $(I2CSLAVE_UNIT_TEST_STD_LIBS)	\
	-l$(I2CSLAVE_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(I2CSLAVE_UNIT_TEST_MAP) 		\
	$(I2CSLAVE_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(I2CSLAVE_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(I2CSLAVE_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the I2C Slave R4 Unit Test OUT file'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssSlaveTestClean:
	@echo 'Cleaning the I2C Slave R4 Unit Test objects'
	@$(DEL) $(I2CSLAVE_UNIT_TEST_OBJECTS) $(I2CSLAVE_UNIT_TEST_OUT) $(I2CSLAVE_UNIT_TEST_BIN)
	@$(DEL) $(I2CSLAVE_UNIT_TEST_MAP) $(I2CSLAVE_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the I2C Slave R4 Unit RTSC package'
	@$(DEL) $(I2CSLAVE_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(I2CSLAVE_UNIT_TEST_DEPENDS)

