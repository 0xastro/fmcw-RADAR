###################################################################################
# Unit Test on xxx Makefile
###################################################################################
.PHONY: dssDSPTest dssDSPTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16_imre/c64P

###################################################################################
# The Test requires additional libraries
###################################################################################
RANGEPROCDSP_UNIT_DSS_TEST_STD_LIBS =   $(C674_COMMON_STD_LIB) \
                                        -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) 	\
                                        -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
                            			-llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)  \
                            			-ldsplib.ae64P	\
                                        -llibmathutils.$(C674_LIB_EXT) \
                            			-lmathlib.$(C674_LIB_EXT)
RANGEPROCDSP_UNIT_DSS_TEST_LOC_LIBS =   $(C674_COMMON_LOC_LIB) \
                                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib 	\
                                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
                                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib	\
                            			-i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib \
                            			-i$(C674x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib		\
                                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib

###################################################################################
# Unit Test Files - rangeProcDSP
###################################################################################
RANGEPROCDSP_UNIT_DSS_TEST_CFG	     = test/dss.cfg
RANGEPROCDSP_UNIT_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROCDSP_UNIT_DSS_TEST_CONFIGPKG = test/c674_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROCDSP_UNIT_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprocdsp_dss.map
RANGEPROCDSP_UNIT_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprocdsp_dss.$(C674_EXE_EXT)
RANGEPROCDSP_UNIT_DSS_TEST_APP_CMD   = test/dss_linker.cmd
RANGEPROCDSP_UNIT_DSS_TEST_SOURCES   = $(RANGEPROC_DSP_LIB_SOURCES)  \
                                       dpedma.c \
                                       rangeprocdsp_test.c \
                                       gen_twiddle_fft16x16_imre.c

RANGEPROCDSP_UNIT_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROCDSP_UNIT_DSS_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
RANGEPROCDSP_UNIT_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROCDSP_UNIT_DSS_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
rangeProcDspRTSC: $(RANGEPROCDSP_UNIT_DSS_TEST_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(RANGEPROCDSP_UNIT_DSS_TEST_CONFIGPKG) $(RANGEPROCDSP_UNIT_DSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test: rangeProcDSP
###################################################################################
dssDSPTest: BUILD_CONFIGPKG=$(RANGEPROCDSP_UNIT_DSS_TEST_CONFIGPKG)
dssDSPTest: C674_CFLAGS += --define=_LITTLE_ENDIAN  --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt	\
	-i$(C674x_MATHLIB_INSTALL_PATH)/packages								\
	-i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16_imre/c64P	\

dssDSPTest: buildDirectories rangeProcDspRTSC $(RANGEPROCDSP_UNIT_DSS_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(RANGEPROCDSP_UNIT_DSS_TEST_LOC_LIBS) $(RANGEPROCDSP_UNIT_DSS_TEST_STD_LIBS) 	\
	-l$(RANGEPROCDSP_UNIT_DSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(RANGEPROCDSP_UNIT_DSS_TEST_MAP) 			\
	$(RANGEPROCDSP_UNIT_DSS_TEST_OBJECTS) $(PLATFORM_C674X_LINK_CMD) $(RANGEPROCDSP_UNIT_DSS_TEST_APP_CMD) 		\
	$(C674_LD_RTS_FLAGS) -o $(RANGEPROCDSP_UNIT_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the Range Proc DSP DPU Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssDSPTestClean:
	@echo 'Cleaning the Range Proc DSP DPU Unit Test objects'
	@$(DEL) $(RANGEPROCDSP_UNIT_DSS_TEST_OBJECTS) $(RANGEPROCDSP_UNIT_DSS_TEST_OUT)
	@$(DEL) $(RANGEPROCDSP_UNIT_DSS_TEST_MAP) $(RANGEPROCDSP_UNIT_DSS_TEST_DEPENDS)
	@echo 'Cleaning the Range Proc DPU DSP Uint Test RTSC package'
	@$(DEL) $(RANGEPROCDSP_UNIT_DSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROCDSP_UNIT_DSS_TEST_DEPENDS)

