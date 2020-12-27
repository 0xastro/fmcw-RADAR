###################################################################################
#   MSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: mssLocal mssLocalClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/local/mss
vpath %.c test/local/dpc/ll

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_LOCAL_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
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
MSS_LOCAL_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
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
MSS_LOCAL_CFG       = test/local/mss/mss.cfg
MSS_LOCAL_CONFIGPKG = test/local/mss/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MSS_LOCAL_MAP       = test/local/mss/$(MMWAVE_SDK_DEVICE_TYPE)_local_mss.map
MSS_LOCAL_OUT       = test/local/mss/$(MMWAVE_SDK_DEVICE_TYPE)_local_mss.$(R4F_EXE_EXT)
MSS_LOCAL_CMD       = test/local/mss/mss_linker.cmd
MSS_LOCAL_SOURCES   = mss_local.c			\
					  ll_profile_local.c
MSS_LOCAL_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_LOCAL_SOURCES:.c=.$(R4F_DEP_EXT)))
MSS_LOCAL_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_LOCAL_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssLocalRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MSS_LOCAL_CONFIGPKG) $(MSS_LOCAL_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the MSS
###################################################################################
mssLocal: BUILD_CONFIGPKG=$(MSS_LOCAL_CONFIGPKG)
mssLocal: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssLocal: buildDirectories mssLocalRTSC $(MSS_LOCAL_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MSS_LOCAL_LOC_LIBS) $(MSS_LOCAL_STD_LIBS) 					\
	-l$(MSS_LOCAL_CONFIGPKG)/linker.cmd --map_file=$(MSS_LOCAL_MAP) $(MSS_LOCAL_OBJECTS) 	\
	$(PLATFORM_R4F_LINK_CMD) $(MSS_LOCAL_CMD) $(R4F_LD_RTS_FLAGS) -o $(MSS_LOCAL_OUT)
	@echo '*****************************'
	@echo 'Built the MSS Local Unit Test'
	@echo '*****************************'

###################################################################################
# Clean the MSS
###################################################################################
mssLocalClean:
	@echo 'Cleaning the MSS Local Objects'
	@$(DEL) $(MSS_LOCAL_OBJECTS) $(MSS_LOCAL_MAP) $(MSS_LOCAL_OUT)
	@$(DEL) $(MSS_LOCAL_DEPENDS) $(MSS_LOCAL_ROV_XS)
	@echo 'Cleaning the MSS Local RTSC package'
	@$(DEL) $(MSS_LOCAL_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_LOCAL_DEPENDS)


