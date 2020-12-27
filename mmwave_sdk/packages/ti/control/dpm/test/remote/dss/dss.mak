###################################################################################
#   DSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: dssRemote dssRemoteClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/remote/dss
vpath %.c test/remote/dpc/ll

###################################################################################
# Additional libraries which are required to build the Unit Test
###################################################################################
DSS_STD_LIBS = $(C674_COMMON_STD_LIB)	      							\
   			   -llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)	\
   			   -llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
			   -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
DSS_LOC_LIBS = $(C674_COMMON_LOC_LIB)									\
   			   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	  	\
			   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib			\
			   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# DSS Unit Test Definitions
###################################################################################
DSS_CFG       	= test/remote/dss/dss.cfg
DSS_CONFIGPKG 	= test/remote/dss/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
DSS_MAP       	= test/remote/dss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_dss.map
DSS_OUT       	= test/remote/dss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_dss.$(C674_EXE_EXT)
DSS_CMD       	= test/remote/dss/dss_linker.cmd
DSS_SOURCES   	= dss_remote.c			\
			      ll_profile_remote.c
DSS_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_SOURCES:.c=.$(C674_DEP_EXT)))
DSS_OBJECTS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssRemoteRTSC: $(DSP_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(DSS_CONFIGPKG) $(DSS_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the DSS
###################################################################################
dssRemote: BUILD_CONFIGPKG=$(DSS_CONFIGPKG)
dssRemote: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssRemote: buildDirectories dssRemoteRTSC $(DSS_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(DSS_LOC_LIBS) $(DSS_STD_LIBS) 			\
	-l$(DSS_CONFIGPKG)/linker.cmd --map_file=$(DSS_MAP) $(DSS_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(DSS_CMD) $(C674_LD_RTS_FLAGS) -o $(DSS_OUT)
	@echo '******************************'
	@echo 'Built the DSS Remote Unit Test'
	@echo '******************************'

###################################################################################
# Clean the DSS
###################################################################################
dssRemoteClean:
	@echo 'Cleaning the DSS Remote Objects'
	@$(DEL) $(DSS_OBJECTS) $(DSS_MAP) $(DSS_OUT) $(DSS_DEPENDS)
	@echo 'Cleaning the DSS Remote RTSC package'
	@$(DEL) $(DSS_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_DEPENDS)


