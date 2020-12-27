###################################################################################
#   DSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: dssDistributed dssDistributedClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/distributed/dpc/ll
vpath %.c test/distributed/dss

###################################################################################
# Additional libraries which are required to build the Unit Test
###################################################################################
DSS_DISTRIBUTED_STD_LIBS =  $(C674_COMMON_STD_LIB)	      							\
   			   				-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
   			   				-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
   			   				-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
DSS_DISTRIBUTED_LOC_LIBS = 	$(C674_COMMON_LOC_LIB)									\
   			   				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	  	\
			   				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib			\
			   				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# DSS Unit Test Definitions
###################################################################################
DSS_DISTRIBUTED_CFG       	= test/distributed/dss/dss.cfg
DSS_DISTRIBUTED_CONFIGPKG 	= test/distributed/dss/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
DSS_DISTRIBUTED_MAP       	= test/distributed/dss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_dss.map
DSS_DISTRIBUTED_OUT       	= test/distributed/dss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_dss.$(C674_EXE_EXT)
DSS_DISTRIBUTED_CMD       	= test/distributed/dss/dss_linker.cmd
DSS_DISTRIBUTED_SOURCES   	= dss_distributed.c			\
			      			  ml_profile_distributed.c
DSS_DISTRIBUTED_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_DISTRIBUTED_SOURCES:.c=.$(C674_DEP_EXT)))
DSS_DISTRIBUTED_OBJECTS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_DISTRIBUTED_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssDistributedRTSC: $(DSS_DISTRIBUTED_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(DSS_DISTRIBUTED_CONFIGPKG) $(DSS_DISTRIBUTED_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the DSS
###################################################################################
dssDistributed: BUILD_CONFIGPKG=$(DSS_DISTRIBUTED_CONFIGPKG)
dssDistributed: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssDistributed: buildDirectories dssDistributedRTSC $(DSS_DISTRIBUTED_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(DSS_DISTRIBUTED_LOC_LIBS) $(DSS_DISTRIBUTED_STD_LIBS) 		\
	-l$(DSS_DISTRIBUTED_CONFIGPKG)/linker.cmd --map_file=$(DSS_DISTRIBUTED_MAP)				\
	$(DSS_DISTRIBUTED_OBJECTS) 	$(PLATFORM_C674X_LINK_CMD) $(DSS_DISTRIBUTED_CMD) 			\
	$(C674_LD_RTS_FLAGS) -o $(DSS_DISTRIBUTED_OUT)
	@echo '***********************************'
	@echo 'Built the DSS Distributed Unit Test'
	@echo '***********************************'

###################################################################################
# Clean the DSS
###################################################################################
dssDistributedClean:
	@echo 'Cleaning the DSS Distributed Objects'
	@$(DEL) $(DSS_DISTRIBUTED_OBJECTS) $(DSS_DISTRIBUTED_MAP) $(DSS_DISTRIBUTED_OUT)
	@$(DEL) $(DSS_DISTRIBUTED_BIN) $(DSS_DISTRIBUTED_DEPENDS)
	@echo 'Cleaning the DSS Distributed RTSC package'
	@$(DEL) $(DSS_DISTRIBUTED_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_DISTRIBUTED_DEPENDS)

