###################################################################################
# Memory Capture: DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/test/common
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/test/mem_capture
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/test/mem_capture/$(MMWAVE_SDK_DEVICE_TYPE)

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
DSS_MEMORY_CAPTURE_STD_LIBS = $(C674_COMMON_STD_LIB)					\
   			-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)			\
   			-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)		\
   			-llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

DSS_MEMORY_CAPTURE_LOC_LIBS = $(C674_COMMON_LOC_LIB)					\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib			\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	    	\
	        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib

###################################################################################
# Stream Wave Demo
###################################################################################
DSS_MEMORY_CAPTURE_CFG       = $(MMWAVE_SDK_DEVICE_TYPE)/dss.cfg
DSS_MEMORY_CAPTURE_CONFIGPKG = $(MMWAVE_SDK_DEVICE_TYPE)/dss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
DSS_MEMORY_CAPTURE_MAP       = $(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mem_capture_dss.map
DSS_MEMORY_CAPTURE_OUT       = $(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mem_capture_dss.$(C674_EXE_EXT)
DSS_MEMORY_CAPTURE_CMD       = $(MMWAVE_SDK_DEVICE_TYPE)/dss_linker.cmd
DSS_MEMORY_CAPTURE_SOURCES   = dss.c					\
						       framework_core.c 		\
							   framework_ipc.c 			\
							   framework_ipc_local.c 	\
							   framework_listlib.c 		\
							   framework_$(MMWAVE_SDK_DEVICE_TYPE).c		\
							   capture.c
DSS_MEMORY_CAPTURE_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MEMORY_CAPTURE_SOURCES:.c=.$(C674_DEP_EXT)))
DSS_MEMORY_CAPTURE_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MEMORY_CAPTURE_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
dssStreamDemoRTSC: $(DSP_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(DSS_MEMORY_CAPTURE_CONFIGPKG) $(DSS_MEMORY_CAPTURE_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the Stream Wave Demo
###################################################################################
dssTest: BUILD_CONFIGPKG=$(DSS_MEMORY_CAPTURE_CONFIGPKG)
dssTest: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssTest: buildDirectories dssStreamDemoRTSC $(DSS_MEMORY_CAPTURE_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(DSS_MEMORY_CAPTURE_LOC_LIBS) $(DSS_MEMORY_CAPTURE_STD_LIBS) 					\
	-l$(DSS_MEMORY_CAPTURE_CONFIGPKG)/linker.cmd --map_file=$(DSS_MEMORY_CAPTURE_MAP) $(DSS_MEMORY_CAPTURE_OBJECTS) 	\
	$(PLATFORM_C674X_LINK_CMD) $(DSS_MEMORY_CAPTURE_CMD) $(C674_LD_RTS_FLAGS) -o $(DSS_MEMORY_CAPTURE_OUT)
	@echo '******************************************************************************'
	@echo 'Built the DSS Memory Capture OUT & BIN Formats'
	@echo '******************************************************************************'

###################################################################################
# Cleanup the Stream Wave Demo
###################################################################################
dssTestClean:
	@echo 'Cleaning the DSS Memory Capture Objects'
	@rm -f $(DSS_MEMORY_CAPTURE_OBJECTS) $(DSS_MEMORY_CAPTURE_MAP) $(DSS_MEMORY_CAPTURE_OUT) \
			$(DSS_MEMORY_CAPTURE_DEPENDS) $(DSS_MEMORY_CAPTURE_ROV_XS)
	@echo 'Cleaning the DSS Memory Capture RTSC package'
	@$(DEL) $(DSS_MEMORY_CAPTURE_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_MEMORY_CAPTURE_DEPENDS)

