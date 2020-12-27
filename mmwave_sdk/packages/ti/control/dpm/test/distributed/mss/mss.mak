###################################################################################
#   MSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: mssDistributed mssDistributedClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/distributed/mss
vpath %.c test/distributed/dpc/ll
vpath %.c test/distributed/dpc/hl

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_DISTRIBUTED_STD_LIBS = $(R4F_COMMON_STD_LIB)						\
		   	-llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 		\
		   	-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibuart_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibgpio_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  		\
		   	-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   	-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)	\
		   	-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  		\
			-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)	\
			-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MSS_DISTRIBUTED_LOC_LIBS = $(R4F_COMMON_LOC_LIB)						\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib 	   		\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/uart/lib			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib				\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib				\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/gpio/lib			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	   		\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib		\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib      	\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib      		\
            -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# MSS Definitions:
###################################################################################
MSS_DISTRIBUTED_CFG       = test/distributed/mss/mss.cfg
MSS_DISTRIBUTED_CONFIGPKG = test/distributed/mss/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MSS_DISTRIBUTED_MAP       = test/distributed/mss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_mss.map
MSS_DISTRIBUTED_OUT       = test/distributed/mss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_mss.$(R4F_EXE_EXT)
MSS_DISTRIBUTED_CMD       = test/distributed/mss/mss_linker.cmd
MSS_DISTRIBUTED_SOURCES   = mss_distributed.c				\
							hl_profile_distributed.c		\
							ll_profile_distributed.c
MSS_DISTRIBUTED_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_DISTRIBUTED_SOURCES:.c=.$(R4F_DEP_EXT)))
MSS_DISTRIBUTED_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_DISTRIBUTED_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssDistributedRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MSS_DISTRIBUTED_CONFIGPKG) $(MSS_DISTRIBUTED_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the MSS
###################################################################################
mssDistributed: BUILD_CONFIGPKG=$(MSS_DISTRIBUTED_CONFIGPKG)
mssDistributed: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssDistributed: buildDirectories mssDistributedRTSC $(MSS_DISTRIBUTED_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MSS_DISTRIBUTED_LOC_LIBS) $(MSS_DISTRIBUTED_STD_LIBS)	\
	-l$(MSS_DISTRIBUTED_CONFIGPKG)/linker.cmd --map_file=$(MSS_DISTRIBUTED_MAP) 		\
	$(MSS_DISTRIBUTED_OBJECTS) 	$(PLATFORM_R4F_LINK_CMD) $(MSS_DISTRIBUTED_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(MSS_DISTRIBUTED_OUT)
	@echo '***********************************'
	@echo 'Built the MSS Distributed Unit Test'
	@echo '***********************************'

###################################################################################
# Clean the MSS
###################################################################################
mssDistributedClean:
	@echo 'Cleaning the MSS Distributed Objects'
	@$(DEL) $(MSS_DISTRIBUTED_OBJECTS) $(MSS_DISTRIBUTED_MAP) $(MSS_DISTRIBUTED_OUT)
	@$(DEL) $(MSS_DISTRIBUTED_BIN) $(MSS_DISTRIBUTED_DEPENDS)
	@echo 'Cleaning the MSS RTSC package'
	@$(DEL) $(MSS_DISTRIBUTED_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_DISTRIBUTED_DEPENDS)

