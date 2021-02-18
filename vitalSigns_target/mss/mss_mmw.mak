###################################################################################
#   Vital Signs Demo
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################

###################################################################################
# Millimeter Wave Demo
###################################################################################
.PHONY: mssDemo mssDemoClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c mss
vpath %.c common

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_MMW_DEMO_STD_LIBS = $(R4F_COMMON_STD_LIB)							\
		   	-llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 		\
		   	-llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibuart_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
		   	-llibgpio_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  		\
		   	-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
		   	-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)	\
		   	-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)  		\
			-llibcli_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MSS_MMW_DEMO_LOC_LIBS = $(R4F_COMMON_LOC_LIB)						\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib 	   	\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/uart/lib		\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/gpio/lib		\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	   	\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib	\
		   	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib      \
            -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/cli/lib

###################################################################################
# Millimeter Wave Demo
###################################################################################
MSS_MMW_CFG_PREFIX     = mss_mmw
MSS_MMW_DEMO_CFG       = mss/$(MSS_MMW_CFG_PREFIX).cfg
MSS_MMW_DEMO_ROV_XS    = $(MSS_MMW_CFG_PREFIX)_$(R4F_XS_SUFFIX).rov.xs
MSS_MMW_DEMO_CONFIGPKG = mss/mmw_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MSS_MMW_DEMO_MAP       = $(MMWAVE_SDK_DEVICE_TYPE)_vitalSigns_demo_mss.map
MSS_MMW_DEMO_OUT       = $(MMWAVE_SDK_DEVICE_TYPE)_vitalSigns_demo_mss.$(R4F_EXE_EXT)
MSS_MMW_DEMO_BIN       = $(MMWAVE_SDK_DEVICE_TYPE)_vitalSigns_demo_mss.bin
MSS_MMW_DEMO_CMD       = mss/mss_mmw_linker.cmd
MSS_MMW_DEMO_SOURCES   = mss_main.c 				\
                         cli.c
MSS_MMW_DEMO_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_MMW_DEMO_SOURCES:.c=.$(R4F_DEP_EXT)))
MSS_MMW_DEMO_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_MMW_DEMO_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mssDemoRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MSS_MMW_DEMO_CONFIGPKG) $(MSS_MMW_DEMO_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the Millimeter Wave Demo
###################################################################################
mssDemo: BUILD_CONFIGPKG=$(MSS_MMW_DEMO_CONFIGPKG)
mssDemo: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssDemo: buildDirectories mssDemoRTSC $(MSS_MMW_DEMO_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MSS_MMW_DEMO_LOC_LIBS) $(MSS_MMW_DEMO_STD_LIBS) 					\
	-l$(MSS_MMW_DEMO_CONFIGPKG)/linker.cmd --map_file=$(MSS_MMW_DEMO_MAP) $(MSS_MMW_DEMO_OBJECTS) 	\
	$(PLATFORM_R4F_LINK_CMD) $(MSS_MMW_DEMO_CMD) $(R4F_LD_RTS_FLAGS) -o $(MSS_MMW_DEMO_OUT)
	$(COPY_CMD) $(MSS_MMW_DEMO_CONFIGPKG)/package/cfg/$(MSS_MMW_DEMO_ROV_XS) $(MSS_MMW_DEMO_ROV_XS)
	@echo 'Built the MSS Vital Signs Monitoring Demo [Preparing the BIN Format]'
	@$(GENERATE_BIN) $(MSS_MMW_DEMO_OUT) $(MSS_MMW_DEMO_BIN)
	@echo '******************************************************************************'
	@echo 'Built the MSS Vital Signs Monitoring OUT and BIN Formats'
	@echo '******************************************************************************'

###################################################################################
# Cleanup the Millimeter Wave Demo
###################################################################################
mssDemoClean:
	@echo 'Cleaning the MSS Vital Signs Monitoring Demo Objects'
	@$(DEL) $(MSS_MMW_DEMO_OBJECTS) $(MSS_MMW_DEMO_MAP) $(MSS_MMW_DEMO_OUT) $(MSS_MMW_DEMO_BIN) $(MSS_MMW_DEMO_DEPENDS) $(MSS_MMW_DEMO_ROV_XS)
	@echo 'Cleaning the MSS Millimeter Wave Demo RTSC package'
	@$(DEL) $(MSS_MMW_DEMO_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_MMW_DEMO_DEPENDS)

