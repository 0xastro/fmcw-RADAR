###################################################################################
# QSPI Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test

###################################################################################
# QSPI Unit Test: This needs additional libraries.
###################################################################################
QSPI_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB) 				\
			 -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 	\
                         -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
QSPI_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)		                        \
	  		 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
QSPI_UNIT_TEST_CFG       = test/mss.cfg
QSPI_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
QSPI_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
QSPI_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_qspi_mss.map
QSPI_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_qspi_mss.$(R4F_EXE_EXT)
QSPI_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_qspi_mss.bin
QSPI_UNIT_TEST_APP_CMD   = test/mss_qspi_linker.cmd
QSPI_UNIT_TEST_SOURCES   = $(QSPI_DRV_SOURCES) main.c
QSPI_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(QSPI_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
QSPI_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(QSPI_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
qspiRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(QSPI_UNIT_TEST_CONFIGPKG) $(QSPI_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(QSPI_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories qspiRTSC $(QSPI_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(QSPI_UNIT_TEST_LOC_LIBS) $(QSPI_UNIT_TEST_STD_LIBS) 	\
	-l$(QSPI_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(QSPI_UNIT_TEST_MAP) 		\
	$(QSPI_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(QSPI_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(QSPI_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the QSPI MSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the QSPI MSS Unit Test objects'
	@$(DEL) $(QSPI_UNIT_TEST_OBJECTS) $(QSPI_UNIT_TEST_OUT) $(QSPI_UNIT_TEST_BIN)
	@$(DEL) $(QSPI_UNIT_TEST_MAP) $(QSPI_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the QSPI MSS Unit RTSC package'
	@$(DEL) $(QSPI_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(QSPI_UNIT_TEST_DEPENDS)

