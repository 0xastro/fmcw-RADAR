###################################################################################
#   Millimeter wave Demo
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################

###################################################################################
# Millimeter Wave Demo
###################################################################################
.PHONY: mmwDemo mmwDemoClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/demo/utils \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwa/src \

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MMW_DEMO_STD_LIBS = $(R4F_COMMON_STD_LIB)                                           \
                    -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)            \
                    -llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)               \
                    -llibuart_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)              \
                    -llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)           \
                    -llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)        \
                    -llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)            \
                    -llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)            \
                    -llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)               \
                    -llibgpio_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)              \
                    -llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)              \
                    -llibcli_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)               \
                    -llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)               \
                    -llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)               \
                    -llibmathutils.$(R4F_LIB_EXT)                                   \
                    -llibosal_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)              \
                    -llibrangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)     \
                    -llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)   \
                    -llibcfarcaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)    \
                    -llibaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)       \
                    -llibstaticclutterproc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) \
                    -llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)        \

MMW_DEMO_LOC_LIBS = $(R4F_COMMON_LOC_LIB)                                               \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib                  \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/uart/lib                    \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib                     \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib                 \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib                  \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib                     \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/gpio/lib                    \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib                    \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib                     \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib              \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/cli/lib                       \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib                  \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib                     \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib                 \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/osal/lib                    \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/lib          \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/lib    \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/cfarcaproc/lib     \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/aoaproc/lib        \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/staticclutterproc/lib  \
                    -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib                 \


###################################################################################
# Millimeter Wave Demo
###################################################################################
MMW_CFG_PREFIX       = mmw
MMW_DEMO_CFG         = $(MMW_CFG_PREFIX).cfg
MMW_DEMO_ROV_XS      = $(MMW_CFG_PREFIX)_$(R4F_XS_SUFFIX).rov.xs
MMW_DEMO_CONFIGPKG   = mmw_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MMW_DEMO_MAP         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_mss.map
MMW_DEMO_OUT         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_mss.$(R4F_EXE_EXT)
MMW_DEMO_METAIMG_BIN = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo.bin
MMW_DEMO_CMD         = mss_mmw_linker.cmd
MMW_DEMO_SOURCES     = objectdetection.c \
                       mmwdemo_rfparser.c  \
                       mmwdemo_adcconfig.c \
                       main.c \
                       data_path.c \
                       mmw_cli.c 


MMW_DEMO_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMW_DEMO_SOURCES:.c=.$(R4F_DEP_EXT)))
MMW_DEMO_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMW_DEMO_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mmwRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MMW_DEMO_CONFIGPKG) $(MMW_DEMO_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the Millimeter Wave Demo
###################################################################################
mmwDemo: BUILD_CONFIGPKG=$(MMW_DEMO_CONFIGPKG)
mmwDemo: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt \
                       --define=APP_RESOURCE_FILE="<ti/demo/xwr68xx/mmw/mmw_res.h>" \
                       --define=DebugP_LOG_ENABLED
mmwDemo: buildDirectories mmwRTSC $(MMW_DEMO_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MMW_DEMO_LOC_LIBS) $(MMW_DEMO_STD_LIBS) 					\
	-l$(MMW_DEMO_CONFIGPKG)/linker.cmd --map_file=$(MMW_DEMO_MAP) $(MMW_DEMO_OBJECTS) 	\
	$(PLATFORM_R4F_LINK_CMD) $(MMW_DEMO_CMD) $(R4F_LD_RTS_FLAGS) -o $(MMW_DEMO_OUT)
	$(COPY_CMD) $(MMW_DEMO_CONFIGPKG)/package/cfg/$(MMW_DEMO_ROV_XS) $(MMW_DEMO_ROV_XS)
	@echo 'Built the Millimeter Wave Demo [Preparing the metaimage binary ..]'
	@$(GENERATE_METAIMAGE) $(MMW_DEMO_METAIMG_BIN) $(SHMEM_ALLOC) $(MMW_DEMO_OUT) $(XWR68XX_RADARSS_IMAGE_BIN) NULL

###################################################################################
# Cleanup the Millimeter Wave Demo
###################################################################################
mmwDemoClean:
	@echo 'Cleaning the Millimeter Wave Demo Objects'
	@rm -f $(MMW_DEMO_OBJECTS) $(MMW_DEMO_MAP) $(MMW_DEMO_OUT) $(MMW_DEMO_METAIMG_BIN) $(MMW_DEMO_DEPENDS) $(MMW_DEMO_ROV_XS)
	@echo 'Cleaning the Millimeter Wave Demo RTSC package'
	@$(DEL) $(MMW_DEMO_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMW_DEMO_DEPENDS)

