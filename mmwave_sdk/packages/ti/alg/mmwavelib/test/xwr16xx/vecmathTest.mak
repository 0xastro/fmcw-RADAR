###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: vecmathTest vecmathClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
#MMWAVELIB_DSS_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_VECMATH_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)

MMWAVELIB_VECMATH_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_vecmath.map
MMWAVELIB_VECMATH_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_vecmath.$(C674_EXE_EXT)
MMWAVELIB_VECMATH_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd

MMWAVELIB_VECMATH_TEST_SOURCES  = 	lib_vecmath_test.c	\
									lib_matutil_test.c	\
									lib_dcremovalflt_test.c

# Compiler Flags for DSP Unit Test Builds:
OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

MMWAVELIB_VECMATH_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_VECMATH_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_VECMATH_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_VECMATH_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
#dssRTSC:
#	@echo 'Configuring RTSC packages...'
#	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_DSS_TEST_CONFIGPKG) $(MMWAVELIB_DSS_TEST_CFG)
#	@echo 'Finished configuring packages'
#	@echo ' '


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
			     -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                 -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
				 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   
                 
###################################################################################
# Build Unit Test:
###################################################################################
vecmathTest: BUILD_CONFIGPKG=$(MMWAVELIB_VECMATH_TEST_CONFIGPKG)
vecmathTest: C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C674_CFLAGS))
vecmathTest: buildDirectories dssRTSC $(MMWAVELIB_VECMATH_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_VECMATH_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_VECMATH_TEST_MAP) $(MMWAVELIB_VECMATH_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_VECMATH_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_VECMATH_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib fixed-point Vector Math Utility Unit Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
vecmathClean:
	@echo 'Cleaning the vecmathTest objects'
	@rm -f $(MMWAVELIB_VECMATH_TEST_OBJECTS) $(MMWAVELIB_CFAROS_TEST_OUT) $(MMWAVELIB_CFAROS_TEST_DEPENDS)
#	@$(DEL) $(MMWAVELIB_DSS_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_CFAROS_TEST_DEPENDS)

