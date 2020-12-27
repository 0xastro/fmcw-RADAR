###################################################################################
# QSPI Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# QSPI Unit Test: This needs additional libraries.
###################################################################################
QSPIFLASH_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB) 				\
			 -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 	\
			 -llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)           \
		   	 -llibqspi_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)          \
			 -llibqspiflash_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)     \
                         -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
QSPIFLASH_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)		                \
	  		 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
			 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib        \
			 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/qspi/lib       \
			 -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/qspiflash/lib  \
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
QSPIFLASH_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
QSPIFLASH_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
QSPIFLASH_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
QSPIFLASH_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_qspiflash_mss.map
QSPIFLASH_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_qspiflash_mss.$(R4F_EXE_EXT)
QSPIFLASH_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_qspiflash_mss.bin
QSPIFLASH_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_qspiflash_linker.cmd
QSPIFLASH_UNIT_TEST_SOURCES   = $(QSPIFLASH_DRV_SOURCES) main.c test_common.c
QSPIFLASH_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(QSPIFLASH_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
QSPIFLASH_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(QSPIFLASH_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
qspiRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(QSPIFLASH_UNIT_TEST_CONFIGPKG) $(QSPIFLASH_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(QSPIFLASH_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories qspiRTSC $(QSPIFLASH_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(QSPIFLASH_UNIT_TEST_LOC_LIBS) $(QSPIFLASH_UNIT_TEST_STD_LIBS) 	\
	-l$(QSPIFLASH_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(QSPIFLASH_UNIT_TEST_MAP) 		\
	$(QSPIFLASH_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(QSPIFLASH_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(QSPIFLASH_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the QSPI MSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the QSPI MSS Unit Test objects'
	@$(DEL) $(QSPIFLASH_UNIT_TEST_OBJECTS) $(QSPIFLASH_UNIT_TEST_OUT) $(QSPIFLASH_UNIT_TEST_BIN)
	@$(DEL) $(QSPIFLASH_UNIT_TEST_MAP) $(QSPIFLASH_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the QSPI MSS Unit RTSC package'
	@$(DEL) $(QSPIFLASH_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(QSPIFLASH_UNIT_TEST_DEPENDS)

