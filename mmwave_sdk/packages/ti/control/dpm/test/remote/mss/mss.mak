###################################################################################
#   MSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: mssRemote mssRemoteClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/remote/mss
vpath %.c test/remote/dpc/hl

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_REMOTE_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
		   	-llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 			\
		   	-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)				\
		   	-llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)				\
		   	-llibuart_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)				\
		   	-llibgpio_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  			\
		   	-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   	-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  			\
			-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
			-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MSS_REMOTE_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib 	   			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/uart/lib				\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib					\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib					\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/gpio/lib				\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	   			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib      		\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib      			\
            -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# MSS Definitions:
###################################################################################
MSS_REMOTE_CFG       = test/remote/mss/mss.cfg
MSS_REMOTE_CONFIGPKG = test/remote/mss/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MSS_REMOTE_MAP       = test/remote/mss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_mss.map
MSS_REMOTE_OUT       = test/remote/mss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_mss.$(R4F_EXE_EXT)
MSS_REMOTE_CMD       = test/remote/mss/mss_linker.cmd
MSS_REMOTE_SOURCES   = mss_remote.c					\
					   hl_profile_remote.c
MSS_REMOTE_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_REMOTE_SOURCES:.c=.$(R4F_DEP_EXT)))
MSS_REMOTE_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_REMOTE_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssRemoteRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MSS_REMOTE_CONFIGPKG) $(MSS_REMOTE_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the MSS
###################################################################################
mssRemote: BUILD_CONFIGPKG=$(MSS_REMOTE_CONFIGPKG)
mssRemote: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssRemote: buildDirectories mssRemoteRTSC $(MSS_REMOTE_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MSS_REMOTE_LOC_LIBS) $(MSS_REMOTE_STD_LIBS) 						\
	-l$(MSS_REMOTE_CONFIGPKG)/linker.cmd --map_file=$(MSS_REMOTE_MAP) $(MSS_REMOTE_OBJECTS) 	\
	$(PLATFORM_R4F_LINK_CMD) $(MSS_REMOTE_CMD) $(R4F_LD_RTS_FLAGS) -o $(MSS_REMOTE_OUT)
	@echo '******************************'
	@echo 'Built the MSS Remote Unit Test'
	@echo '******************************'

###################################################################################
# Clean the MSS
###################################################################################
mssRemoteClean:
	@echo 'Cleaning the MSS Remote Objects'
	@$(DEL) $(MSS_REMOTE_OBJECTS) $(MSS_REMOTE_MAP) $(MSS_REMOTE_OUT) $(MSS_REMOTE_DEPENDS)
	@echo 'Cleaning the MSS Remote RTSC package'
	@$(DEL) $(MSS_REMOTE_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_REMOTE_DEPENDS)


