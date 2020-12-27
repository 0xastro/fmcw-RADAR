###################################################################################
# DMA Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test

###################################################################################
# The DMA Unit Test uses the common libraries + Test Logger Library
###################################################################################
DMA_UNIT_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
                         -llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)	    \
                         -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
DMA_UNIT_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib        \
                         -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
DMA_UNIT_TEST_CFG       = test/mss.cfg
DMA_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
DMA_UNIT_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
DMA_UNIT_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_dma_mss.map
DMA_UNIT_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_dma_mss.$(R4F_EXE_EXT)
DMA_UNIT_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_dma_mss.bin
DMA_UNIT_TEST_APP_CMD   = test/mss_dma_linker.cmd
DMA_UNIT_TEST_SOURCES   = main.c
DMA_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DMA_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
DMA_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DMA_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dmaRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(DMA_UNIT_TEST_CONFIGPKG) $(DMA_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(DMA_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories dmaRTSC $(DMA_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(DMA_UNIT_TEST_LOC_LIBS) $(DMA_UNIT_TEST_STD_LIBS)	\
	-l$(DMA_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(DMA_UNIT_TEST_MAP) 		\
	$(DMA_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(DMA_UNIT_TEST_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(DMA_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the DMA R4 Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the DMA R4 Unit Test objects'
	@$(DEL) $(DMA_UNIT_TEST_OBJECTS) $(DMA_UNIT_TEST_OUT) $(DMA_UNIT_TEST_BIN)
	@$(DEL) $(DMA_UNIT_TEST_MAP) $(DMA_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the DMA R4 Unit RTSC package'
	@$(DEL) $(DMA_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DMA_UNIT_TEST_DEPENDS)

