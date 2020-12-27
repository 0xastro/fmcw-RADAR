###################################################################################
# STATIC CLUTTER PROC Test
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/$(MMWAVE_SDK_DEVICE_TYPE)
vpath %.c test/common
vpath %.c ../../../../common
vpath %.c ../../../dpedma/src
vpath %.c src

###################################################################################
# The STATIC CLUTTER PROC Test requires additional libraries
###################################################################################
STATICCLUTTERPROC_MSS_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)								\
           				-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 		\
           				-llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)       \
                        -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
STATICCLUTTERPROC_MSS_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)								\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib 	\
           				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib    \
                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
STATICCLUTTERPROC_MSS_TEST_CFG	 	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
STATICCLUTTERPROC_MSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
STATICCLUTTERPROC_MSS_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
STATICCLUTTERPROC_MSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_staticclutterproc_mss.map
STATICCLUTTERPROC_MSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_staticclutterproc_mss.$(R4F_EXE_EXT)
STATICCLUTTERPROC_MSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_staticclutterproc_mss.bin
STATICCLUTTERPROC_MSS_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/mss_staticclutterproc_linker.cmd
STATICCLUTTERPROC_MSS_TEST_SOURCES   = main_mss.c \
                                       dpedma.c \
                                       dpedmahwa.c \
                                       test_common.c \
                                       staticclutterproc.c
                                   

STATICCLUTTERPROC_MSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_MSS_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
STATICCLUTTERPROC_MSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_MSS_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(STATICCLUTTERPROC_MSS_TEST_CONFIGPKG) $(STATICCLUTTERPROC_MSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(STATICCLUTTERPROC_MSS_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories mssRTSC $(STATICCLUTTERPROC_MSS_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(STATICCLUTTERPROC_MSS_TEST_LOC_LIBS) $(STATICCLUTTERPROC_MSS_TEST_STD_LIBS) -l$(STATICCLUTTERPROC_MSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(STATICCLUTTERPROC_MSS_TEST_MAP) $(STATICCLUTTERPROC_MSS_TEST_OBJECTS) \
	$(PLATFORM_R4F_LINK_CMD) $(STATICCLUTTERPROC_MSS_TEST_APP_CMD) $(R4F_LD_RTS_FLAGS) -o $(STATICCLUTTERPROC_MSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the staticClutterProc R4 Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the STATIC CLUTTER PROC R4 Unit Test objects'
	@$(DEL) $(STATICCLUTTERPROC_MSS_TEST_OBJECTS) $(STATICCLUTTERPROC_MSS_TEST_OUT) $(STATICCLUTTERPROC_MSS_TEST_BIN)
	@$(DEL) $(STATICCLUTTERPROC_MSS_TEST_MAP) $(STATICCLUTTERPROC_MSS_TEST_DEPENDS)
	@echo 'Cleaning the STATIC CLUTTER PROC R4 Unit RTSC package'
	@$(DEL) $(STATICCLUTTERPROC_MSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(STATICCLUTTERPROC_MSS_TEST_DEPENDS)

