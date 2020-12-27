###################################################################################
# Unit Test on Range Proc HWA DPU Makefile
###################################################################################
.PHONY: dssHWATest dssHWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/test

###################################################################################
# Libraries
###################################################################################
RANGEPROC_UNIT_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB) \
									-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) 	\
									-llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)     \
									-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
									-llibmathutils.$(C674_LIB_EXT)
RANGEPROC_UNIT_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB) \
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib 	\
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib    	\
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib 	\
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib

###################################################################################
# Unit Test Files
###################################################################################
RANGEPROC_UNIT_DSS_TEST_CFG	      = test/dss.cfg
RANGEPROC_UNIT_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_DSS_TEST_CONFIGPKG = test/c674_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_dss.map
RANGEPROC_UNIT_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_dss.$(C674_EXE_EXT)
RANGEPROC_UNIT_DSS_TEST_APP_CMD   = test/dss_linker.cmd
RANGEPROC_UNIT_DSS_TEST_SOURCES   = $(RANGEPROC_HWA_LIB_SOURCES) \
                                    hwa_main.c \
                                    dpedmahwa.c \
                                    dpedma.c

RANGEPROC_UNIT_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
RANGEPROC_UNIT_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3


###################################################################################
# RTSC Configuration:
###################################################################################
rangeProcRTSC: $(RANGEPROC_UNIT_DSS_TEST_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(RANGEPROC_UNIT_DSS_TEST_CONFIGPKG) $(RANGEPROC_UNIT_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssHWATest: BUILD_CONFIGPKG=$(RANGEPROC_UNIT_DSS_TEST_CONFIGPKG)
dssHWATest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt \
                        -i$(C674x_MATHLIB_INSTALL_PATH)/packages
dssHWATest: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))

dssHWATest: buildDirectories rangeProcRTSC $(RANGEPROC_UNIT_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(RANGEPROC_UNIT_DSS_TEST_LOC_LIBS) $(RANGEPROC_UNIT_DSS_TEST_STD_LIBS) 		\
	-l$(RANGEPROC_UNIT_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(RANGEPROC_UNIT_DSS_TEST_MAP) 			\
	$(RANGEPROC_UNIT_DSS_TEST_OBJECTS) $(PLATFORM_C674X_LINK_CMD) $(RANGEPROC_UNIT_DSS_TEST_APP_CMD) 		\
	$(C674_LD_RTS_FLAGS) -o $(RANGEPROC_UNIT_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Range Proc HWA DPU DSP Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssHWATestClean:
	@echo 'Cleaning the Range Proc DPU DSP Unit Test objects'
	@$(DEL) $(RANGEPROC_UNIT_DSS_TEST_OBJECTS) $(RANGEPROC_UNIT_DSS_TEST_OUT)
	@$(DEL) $(RANGEPROC_UNIT_DSS_TEST_MAP) $(RANGEPROC_UNIT_DSS_TEST_DEPENDS)
	@echo 'Cleaning the Range Proc HWA DPU DSP Uint Test RTSC package'
	@$(DEL) $(RANGEPROC_UNIT_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_UNIT_DSS_TEST_DEPENDS)

