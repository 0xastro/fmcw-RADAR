###################################################################################
# CRC Unit Test on DSS
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common

###################################################################################
# The CRC Unit Test uses the common libraries + Test Logger Library
###################################################################################
CRC_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)									\
					    -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
CRC_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)									\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
CRC_DSS_TEST_CFG	   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
CRC_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
CRC_DSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CRC_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_crc_dss.map
CRC_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_crc_dss.$(C674_EXE_EXT)
CRC_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_crc_dss.bin
CRC_DSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_crc_linker.cmd
CRC_DSS_TEST_SOURCES   = $(CRC_DRV_SOURCES) 		\
						  main_dss.c				\
						  crc_test.c 				\
						  crcmodel.c
CRC_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
CRC_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(CRC_DSS_TEST_CONFIGPKG) $(CRC_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(CRC_DSS_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssRTSC $(CRC_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(CRC_DSS_TEST_LOC_LIBS) $(CRC_DSS_TEST_STD_LIBS) 						\
	-l$(CRC_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(CRC_DSS_TEST_MAP) $(CRC_DSS_TEST_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(CRC_DSS_TEST_APP_CMD) $(C674_LD_RTS_FLAGS) -lrts6740_elf.lib -o $(CRC_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the CRC DSS Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the CRC DSS Unit Test objects'
	@$(DEL) $(CRC_DSS_TEST_OBJECTS) $(CRC_DSS_TEST_OUT)
	@$(DEL) $(CRC_DSS_TEST_BIN) $(CRC_DSS_TEST_DEPENDS)
	@$(DEL) $(CRC_DSS_TEST_MAP)
	@echo 'Cleaning the CRC DSS Unit RTSC package'
	@$(DEL) $(CRC_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CRC_DSS_TEST_DEPENDS)

