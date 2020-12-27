###################################################################################
# SPI Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# SPI Unit Test: This needs additional libraries.
###################################################################################
SPI_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB) 					\
                         -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 	\
                         -llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)      \
                         -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
SPI_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)		                        \
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib    \
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
SPI_UNIT_TEST_CFG       = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
SPI_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
SPI_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
SPI_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_spi_mss.map
SPI_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_spi_mss.$(R4F_EXE_EXT)
SPI_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_spi_mss.bin
SPI_UNIT_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_spi_linker.cmd
SPI_UNIT_TEST_SOURCES   = $(SPI_DRV_SOURCES) main.c test_common.c
SPI_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(SPI_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
SPI_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(SPI_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
spiRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(SPI_UNIT_TEST_CONFIGPKG) $(SPI_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(SPI_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories spiRTSC $(SPI_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(SPI_UNIT_TEST_LOC_LIBS) $(SPI_UNIT_TEST_STD_LIBS) 	\
	-l$(SPI_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(SPI_UNIT_TEST_MAP) 		\
	$(SPI_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(SPI_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(SPI_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the SPI MSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the SPI MSS Unit Test objects'
	@$(DEL) $(SPI_UNIT_TEST_OBJECTS) $(SPI_UNIT_TEST_OUT) $(SPI_UNIT_TEST_BIN)
	@$(DEL) $(SPI_UNIT_TEST_MAP) $(SPI_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the SPI MSS Unit RTSC package'
	@$(DEL) $(SPI_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(SPI_UNIT_TEST_DEPENDS)

