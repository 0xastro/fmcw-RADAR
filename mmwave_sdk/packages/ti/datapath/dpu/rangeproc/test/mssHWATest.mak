###################################################################################
# Range Proc HWA DPU Unit Test
###################################################################################
.PHONY: mssHWATest mssHWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/test

###################################################################################
# The HWA EDMA Test requires additional libraries
###################################################################################
RANGEPROC_UNIT_MSS_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)					\
                                    -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 	\
                                    -llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)       \
                                    -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) \
                                    -llibmathutils.$(R4F_LIB_EXT)
RANGEPROC_UNIT_MSS_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)					\
                                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib 	\
                                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib    	\
                                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
                                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib

###################################################################################
# Unit Test Files
###################################################################################
RANGEPROC_UNIT_MSS_TEST_CFG       = test/mss.cfg
RANGEPROC_UNIT_MSS_MSS_TEST_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_MSS_TEST_CONFIGPKG = test/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_MSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_mss.map
RANGEPROC_UNIT_MSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_mss.$(R4F_EXE_EXT)
RANGEPROC_UNIT_MSS_TEST_APP_CMD   = test/mss_linker.cmd
RANGEPROC_UNIT_MSS_TEST_SOURCES   = $(RANGEPROC_HWA_LIB_SOURCES) \
                                    hwa_main.c  \
                                    dpedmahwa.c \
                                    dpedma.c                                             

RANGEPROC_UNIT_MSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_MSS_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
RANGEPROC_UNIT_MSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_MSS_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# RTSC Configuration:
###################################################################################
mssRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(RANGEPROC_UNIT_MSS_TEST_CONFIGPKG) $(RANGEPROC_UNIT_MSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssHWATest: BUILD_CONFIGPKG=$(RANGEPROC_UNIT_MSS_TEST_CONFIGPKG)
mssHWATest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt  
mssHWATest: R4F_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(R4F_CFLAGS))
mssHWATest: buildDirectories mssRTSC $(RANGEPROC_UNIT_MSS_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(RANGEPROC_UNIT_MSS_TEST_LOC_LIBS) $(RANGEPROC_UNIT_MSS_TEST_STD_LIBS) \
	-l$(RANGEPROC_UNIT_MSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(RANGEPROC_UNIT_MSS_TEST_MAP)      \
	$(RANGEPROC_UNIT_MSS_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(RANGEPROC_UNIT_MSS_TEST_APP_CMD)   \
	$(R4F_LD_RTS_FLAGS) -o $(RANGEPROC_UNIT_MSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Range Proc HWA DPU R4 Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssHWATestClean:
	@echo 'Cleaning the Range Proc DPU R4 Unit Test objects'
	@$(DEL) $(RANGEPROC_UNIT_MSS_TEST_OBJECTS) $(RANGEPROC_UNIT_MSS_TEST_OUT)
	@$(DEL) $(RANGEPROC_UNIT_MSS_TEST_MAP) $(RANGEPROC_UNIT_MSS_TEST_DEPENDS)
	@echo 'Cleaning the Range Proc HWA DPU R4 Unit Test RTSC package'
	@$(DEL) $(RANGEPROC_UNIT_MSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_UNIT_MSS_TEST_DEPENDS)

