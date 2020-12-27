###################################################################################
# DOPPLER PROC Test
###################################################################################
.PHONY: hwaDssTest hwaDssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

###################################################################################
# The DOPPLER PROC Test requires additional libraries
###################################################################################
HWADOPPLERPROC_DSS_TEST_STD_LIBS = $(C674_COMMON_STD_LIB)								\
           				-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) 		\
           				-llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)       \
                        -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
						-llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
HWADOPPLERPROC_DSS_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB)								\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib 	\
           				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib    \
                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWADOPPLERPROC_DSS_TEST_CFG	 	 = test/dss.cfg
HWADOPPLERPROC_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_DSS_TEST_CONFIGPKG = test/c674_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_dss.map
HWADOPPLERPROC_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_dss.$(C674_EXE_EXT)
HWADOPPLERPROC_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_dss.bin
HWADOPPLERPROC_DSS_TEST_APP_CMD   = test/dss_dopplerproc_linker.cmd
HWADOPPLERPROC_DSS_TEST_SOURCES   = doppleprochwa_test_main.c \
								     mathutils.c 
								

HWADOPPLERPROC_DSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
HWADOPPLERPROC_DSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3


###################################################################################
# RTSC Configuration:
###################################################################################
hwaDssRTSC: $(HWADOPPLERPROC_DSS_TEST_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(HWADOPPLERPROC_DSS_TEST_CONFIGPKG) $(HWADOPPLERPROC_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
hwaDssTest: BUILD_CONFIGPKG=$(HWADOPPLERPROC_DSS_TEST_CONFIGPKG)
hwaDssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
hwaDssTest: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
hwaDssTest: buildDirectories hwaDssRTSC $(HWADOPPLERPROC_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(HWADOPPLERPROC_DSS_TEST_LOC_LIBS) $(HWADOPPLERPROC_DSS_TEST_STD_LIBS) \
   -l$(HWADOPPLERPROC_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(HWADOPPLERPROC_DSS_TEST_MAP)   \
    $(HWADOPPLERPROC_DSS_TEST_OBJECTS) $(PLATFORM_C674X_LINK_CMD) $(HWADOPPLERPROC_DSS_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(HWADOPPLERPROC_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the doppler proc DSP Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaDssTestClean:
	@echo 'Cleaning the DOPPLER PROC DSP Unit Test objects'
	@$(DEL) $(HWADOPPLERPROC_DSS_TEST_OBJECTS) $(HWADOPPLERPROC_DSS_TEST_OUT) $(HWADOPPLERPROC_DSS_TEST_BIN)
	@$(DEL) $(HWADOPPLERPROC_DSS_TEST_MAP) $(HWADOPPLERPROC_DSS_TEST_DEPENDS)
	@echo 'Cleaning the DOPPLER PROC DSP Unit RTSC package'
	@$(DEL) $(HWADOPPLERPROC_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWADOPPLERPROC_DSS_TEST_DEPENDS)

