###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: dbscanClusteringTest dbscanClusteringClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/


###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_dbscanClustering_TEST_CFG      = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
MMWAVELIB_dbscanClustering_TEST_CONFIGPKG = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)

MMWAVELIB_dbscanClustering_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dbscanClustering.map
MMWAVELIB_dbscanClustering_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dbscanClustering.$(C674_EXE_EXT)
MMWAVELIB_dbscanClustering_TEST_APP_CMD	 = test/$(MMWAVE_SDK_DEVICE_TYPE)/dss_mmwavelib_linker.cmd

MMWAVELIB_dbscanClustering_TEST_SOURCES  = 	lib_dbscanClustering_test.c
								
# Compiler Flags for DSP Builds:
		    
MMWAVELIB_dbscanClustering_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_dbscanClustering_TEST_SOURCES:.c=.$(C674_DEP_EXT)))
MMWAVELIB_dbscanClustering_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_dbscanClustering_TEST_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dbscanClusteringRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(MMWAVELIB_dbscanClustering_TEST_CONFIGPKG) $(MMWAVELIB_dbscanClustering_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
#STD_LIBS = -llibmmwavelib_$(MMWAVE_SDK_DEVICE_TYPE).$(DSP_LIB_EXT)    \
#           -ldsplib.lib    \
#LOC_LIBS = -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
#           -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib

C674_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
			     -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                 -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
				 -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   
                 
###################################################################################
# Build Unit Test:
###################################################################################
dbscanClusteringTest: BUILD_CONFIGPKG=$(MMWAVELIB_dbscanClustering_TEST_CONFIGPKG)
dbscanClusteringTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dbscanClusteringTest: buildDirectories dssRTSC $(MMWAVELIB_dbscanClustering_TEST_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) \
	-l$(MMWAVELIB_dbscanClustering_TEST_CONFIGPKG)/linker.cmd --map_file=$(MMWAVELIB_dbscanClustering_TEST_MAP) $(MMWAVELIB_dbscanClustering_TEST_OBJECTS) \
	$(PLATFORM_C674X_LINK_CMD) $(MMWAVELIB_dbscanClustering_TEST_APP_CMD) \
	$(C674_LD_RTS_FLAGS) -o $(MMWAVELIB_dbscanClustering_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the mmwavelib Clustering point DBSCAN Clustering Test OUT'
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
dbscanClusteringClean:
	@echo 'Cleaning the dbscanClusteringTest objects'
	@rm -f $(MMWAVELIB_dbscanClustering_TEST_OBJECTS) $(MMWAVELIB_dbscanClustering_TEST_OUT) $(MMWAVELIB_dbscanClustering_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_dbscanClustering_TEST_CONFIGPKG)

# Dependency handling
-include $(MMWAVELIB_dbscanClustering_TEST_DEPENDS)

