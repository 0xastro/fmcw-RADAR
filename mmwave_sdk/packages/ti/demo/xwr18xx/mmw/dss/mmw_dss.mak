###################################################################################
#   Millimeter wave Demo
#
#  NOTE:
#      (C) Copyright 2016 Texas Instruments, Inc.
###################################################################################

###################################################################################
# Millimeter Wave Demo
###################################################################################
.PHONY: dssDemo dssDemoClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/demo/utils \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwa/src \
	  ./dss

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
DSS_MMW_DEMO_STD_LIBS = $(C674_COMMON_STD_LIB)						\
		   	-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
		   	-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
			-llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)  	\
   			-lmathlib.$(C674_LIB_EXT) 					\
		   	-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
		   	-llibhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)           	\
        		-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) 		\
                    	-llibmathutils.$(C674_LIB_EXT) 					\
                    	-llibosal_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
	                -llibrangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)     \
                    	-llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   \
                    	-llibcfarcaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)    \
                    	-llibaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)       \
                    	-llibstaticclutterproc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT) \
                    	-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)        \

DSS_MMW_DEMO_LOC_LIBS = $(C674_COMMON_LOC_LIB)						\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib			\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	    	\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib	    	\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib		\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/hwa/lib         	\
                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib 		\
	                -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib         	\
        	        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib     	\
			-i$(C674x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib 	\
                    	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/lib          \
                    	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/dopplerproc/lib    \
                    	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/cfarcaproc/lib     \
                    	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/aoaproc/lib        \
                    	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/dpu/staticclutterproc/lib  \
                    	-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib                 \

###################################################################################
# Millimeter Wave Demo
###################################################################################
DSS_MMW_CFG_PREFIX       = mmw_dss
DSS_MMW_DEMO_CFG         = $(DSS_MMW_CFG_PREFIX).cfg
DSS_MMW_DEMO_ROV_XS      = $(DSS_MMW_CFG_PREFIX)_$(C674_XS_SUFFIX).rov.xs
DSS_MMW_DEMO_CONFIGPKG   = mmw_configPkg_dss_$(MMWAVE_SDK_DEVICE_TYPE)
DSS_MMW_DEMO_MAP         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss.map
DSS_MMW_DEMO_OUT         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss.$(C674_EXE_EXT)
DSS_MMW_DEMO_METAIMG_BIN = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo.bin
DSS_MMW_DEMO_CMD         = dss/mmw_dss_linker.cmd
DSS_MMW_DEMO_SOURCES     =  objectdetection.c \
                       	mmwdemo_rfparser.c  \
                       	mmwdemo_adcconfig.c \
                       	dss_main.c \
                       	data_path.c

DSS_MMW_DEMO_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MMW_DEMO_SOURCES:.c=.$(C674_DEP_EXT)))
DSS_MMW_DEMO_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MMW_DEMO_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mmwDssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(DSS_MMW_DEMO_CONFIGPKG) dss/$(DSS_MMW_DEMO_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the Millimeter Wave Demo
###################################################################################
dssDemo: BUILD_CONFIGPKG=$(DSS_MMW_DEMO_CONFIGPKG)
dssDemo: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt \
                       --define=APP_RESOURCE_FILE="<ti/demo/xwr18xx/mmw/mmw_res.h>" \
                        -i$(C674x_MATHLIB_INSTALL_PATH)/packages \
			--define=DebugP_LOG_ENABLED

dssDemo: buildDirectories mmwDssRTSC $(DSS_MMW_DEMO_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(DSS_MMW_DEMO_LOC_LIBS) $(DSS_MMW_DEMO_STD_LIBS) 					\
	-l$(DSS_MMW_DEMO_CONFIGPKG)/linker.cmd --map_file=$(DSS_MMW_DEMO_MAP) $(DSS_MMW_DEMO_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(DSS_MMW_DEMO_CMD) $(C674_LD_RTS_FLAGS) -o $(DSS_MMW_DEMO_OUT)
	$(COPY_CMD) $(DSS_MMW_DEMO_CONFIGPKG)/package/cfg/$(DSS_MMW_DEMO_ROV_XS) $(DSS_MMW_DEMO_ROV_XS)
	@echo '******************************************************************************'
	@echo 'Built the DSS for Millimeter Wave Demo'
	@echo '******************************************************************************'

###################################################################################
# Cleanup the Millimeter Wave Demo
###################################################################################
dssDemoClean:
	@echo 'Cleaning the Millimeter Wave Demo DSS Objects'
	@rm -f $(DSS_MMW_DEMO_OBJECTS) $(DSS_MMW_DEMO_MAP) $(DSS_MMW_DEMO_OUT) $(DSS_MMW_DEMO_METAIMG_BIN) $(DSS_MMW_DEMO_DEPENDS) $(DSS_MMW_DEMO_ROV_XS)
	@echo 'Cleaning the Millimeter Wave Demo DSS RTSC package'
	@$(DEL) $(DSS_MMW_DEMO_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_MMW_DEMO_DEPENDS)

