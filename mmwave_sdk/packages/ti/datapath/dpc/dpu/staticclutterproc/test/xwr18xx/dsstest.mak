###################################################################################
# STATIC CLUTTER PROC Unit Test on C674 Makefile
###################################################################################
.PHONY: dssTest dssTestClean

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
# Libraries
###################################################################################
STATICCLUTTERPROC_TEST_STD_LIBS = $(C674_COMMON_STD_LIB) \
                               -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
                               -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
                               -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
STATICCLUTTERPROC_TEST_LOC_LIBS = $(C674_COMMON_LOC_LIB) \
                               -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib \
                               -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib \
                               -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib 
   


###################################################################################
# Unit Test Files
###################################################################################
STATICCLUTTERPROC_TEST_CFG	     = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
STATICCLUTTERPROC_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
STATICCLUTTERPROC_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/c674_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
STATICCLUTTERPROC_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_staticclutterproc_dss.map
STATICCLUTTERPROC_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_staticclutterproc_dss.$(C674_EXE_EXT)
STATICCLUTTERPROC_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_staticclutterproc_dss.bin
STATICCLUTTERPROC_TEST_APP_CMD   = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_staticclutterproc_linker.cmd
STATICCLUTTERPROC_TEST_SOURCES   = main_dss.c \
                                   dpedma.c \
                                   dpedmahwa.c \
                                   test_common.c \
                                   staticclutterproc.c
                                   

STATICCLUTTERPROC_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
STATICCLUTTERPROC_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRTSC: $(STATICCLUTTERPROC_TEST_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(STATICCLUTTERPROC_TEST_CONFIGPKG) $(STATICCLUTTERPROC_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: BUILD_CONFIGPKG=$(STATICCLUTTERPROC_TEST_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt 

dssTest: buildDirectories dssRTSC $(STATICCLUTTERPROC_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(STATICCLUTTERPROC_TEST_LOC_LIBS) $(STATICCLUTTERPROC_TEST_STD_LIBS) 		\
	-l$(STATICCLUTTERPROC_TEST_CONFIGPKG)/linker.cmd --map_file=$(STATICCLUTTERPROC_TEST_MAP) 			\
	$(STATICCLUTTERPROC_TEST_OBJECTS) $(PLATFORM_C674X_LINK_CMD) $(STATICCLUTTERPROC_TEST_APP_CMD) 		\
	$(C674_LD_RTS_FLAGS) -o $(STATICCLUTTERPROC_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the staticClutterProc C674 Unit Test'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the staticClutterProc C674 Unit Test objects'
	@$(DEL) $(STATICCLUTTERPROC_TEST_OBJECTS) $(STATICCLUTTERPROC_TEST_OUT) $(STATICCLUTTERPROC_TEST_BIN)
	@$(DEL) $(STATICCLUTTERPROC_TEST_MAP) $(STATICCLUTTERPROC_TEST_DEPENDS)
	@echo 'Cleaning the staticClutterProc C674 Unit RTSC package'
	@$(DEL) $(STATICCLUTTERPROC_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(STATICCLUTTERPROC_TEST_DEPENDS)

