###################################################################################
# DOPPLER PROC Test
###################################################################################
.PHONY: hwaMssTest hwaMssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/src

###################################################################################
# The DOPPLER PROC Test requires additional libraries
###################################################################################
HWADOPPLERPROC_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)								\
           				-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 		\
           				-llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)       \
                        -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) \
						-llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
HWADOPPLERPROC_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)								\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib 	\
           				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib    \
                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWADOPPLERPROC_TEST_CFG	 	 = test/mss.cfg
HWADOPPLERPROC_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_TEST_CONFIGPKG = test/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_mss.map
HWADOPPLERPROC_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_mss.$(R4F_EXE_EXT)
HWADOPPLERPROC_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_mss.bin
HWADOPPLERPROC_TEST_APP_CMD   = test/mss_dopplerproc_linker.cmd
HWADOPPLERPROC_TEST_SOURCES   = doppleprochwa_test_main.c \
								 mathutils.c 
								
                                   
OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

HWADOPPLERPROC_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
HWADOPPLERPROC_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
hwaMssRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(HWADOPPLERPROC_TEST_CONFIGPKG) $(HWADOPPLERPROC_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
hwaMssTest: BUILD_CONFIGPKG=$(HWADOPPLERPROC_TEST_CONFIGPKG)
hwaMssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
hwaMssTest: R4F_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(R4F_CFLAGS))
hwaMssTest: buildDirectories hwaMssRTSC $(HWADOPPLERPROC_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(HWADOPPLERPROC_TEST_LOC_LIBS) $(HWADOPPLERPROC_TEST_STD_LIBS) -l$(HWADOPPLERPROC_TEST_CONFIGPKG)/linker.cmd --map_file=$(HWADOPPLERPROC_TEST_MAP) $(HWADOPPLERPROC_TEST_OBJECTS) \
	$(PLATFORM_R4F_LINK_CMD) $(HWADOPPLERPROC_TEST_APP_CMD) $(R4F_LD_RTS_FLAGS) -o $(HWADOPPLERPROC_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the doppler proc R4 Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaMssTestClean:
	@echo 'Cleaning the DOPPLER PROC R4 Unit Test objects'
	@$(DEL) $(HWADOPPLERPROC_TEST_OBJECTS) $(HWADOPPLERPROC_TEST_OUT) $(HWADOPPLERPROC_TEST_BIN)
	@$(DEL) $(HWADOPPLERPROC_TEST_MAP) $(HWADOPPLERPROC_TEST_DEPENDS)
	@echo 'Cleaning the DOPPLER PROC R4 Unit RTSC package'
	@$(DEL) $(HWADOPPLERPROC_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWADOPPLERPROC_TEST_DEPENDS)

