###################################################################################
# Memory Capture: MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/test/common
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/test/mem_capture
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/test/mem_capture/xwr14xx

###################################################################################
# The mmWave Unit Test requires additional libraries
###################################################################################
MSS_MEMORY_CAPTURE_STD_LIBS = $(R4F_COMMON_STD_LIB)						\
   			-llibesm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT) 			\
   			-llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
   			-llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)			\
   			-llibmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
   			-llibadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
   			-llibedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		    \
   			-llibmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)    \
   			-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)        \

MSS_MEMORY_CAPTURE_LOC_LIBS = $(R4F_COMMON_LOC_LIB)						\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/esm/lib				\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib			\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib				\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib		        \
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/mailbox/lib	        \
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwavelink/lib      \
			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/adcbuf/lib  		\
	        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/edma/lib    		\
   			-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib

###################################################################################
# Unit Test Files
###################################################################################
MSS_MEMORY_CAPTURE_UNIT_TEST_CFG		 = $(MMWAVE_SDK_DEVICE_TYPE)/mss.cfg
MSS_MEMORY_CAPTURE_UNIT_TEST_CMD		 = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MSS_MEMORY_CAPTURE_UNIT_TEST_CONFIGPKG	 = $(MMWAVE_SDK_DEVICE_TYPE)/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
MSS_MEMORY_CAPTURE_UNIT_TEST_MAP		 = $(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mem_capture_mss.map
MSS_MEMORY_CAPTURE_UNIT_TEST_OUT		 = $(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mem_capture_mss.$(R4F_EXE_EXT)
MSS_MEMORY_CAPTURE_UNIT_TEST_BIN		 = $(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mem_capture_mss.bin
MSS_MEMORY_CAPTURE_UNIT_TEST_APP_CMD	 = $(MMWAVE_SDK_DEVICE_TYPE)/mss_linker.cmd
MSS_MEMORY_CAPTURE_UNIT_TEST_SOURCES	 = mss.c 					\
										   framework_core.c 		\
										   framework_ipc.c 			\
										   framework_ipc_local.c 	\
										   framework_listlib.c 		\
										   framework_xwr14xx.c		\
										   capture.c
MSS_MEMORY_CAPTURE_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_MEMORY_CAPTURE_UNIT_TEST_SOURCES:.c=.$(R4F_DEP_EXT)))
MSS_MEMORY_CAPTURE_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_MEMORY_CAPTURE_UNIT_TEST_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
mmWaveFullRTSC: $(R4_CFG)
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(MSS_MEMORY_CAPTURE_UNIT_TEST_CONFIGPKG) $(MSS_MEMORY_CAPTURE_UNIT_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: BUILD_CONFIGPKG=$(MSS_MEMORY_CAPTURE_UNIT_TEST_CONFIGPKG)
mssTest: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssTest: buildDirectories mmWaveFullRTSC $(MSS_MEMORY_CAPTURE_UNIT_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(MSS_MEMORY_CAPTURE_LOC_LIBS) $(MSS_MEMORY_CAPTURE_STD_LIBS) 					\
	-l$(MSS_MEMORY_CAPTURE_UNIT_TEST_CONFIGPKG)/linker.cmd --map_file=$(MSS_MEMORY_CAPTURE_UNIT_TEST_MAP) 	\
	$(MSS_MEMORY_CAPTURE_UNIT_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(MSS_MEMORY_CAPTURE_UNIT_TEST_APP_CMD) 	\
	$(R4F_LD_RTS_FLAGS) -o $(MSS_MEMORY_CAPTURE_UNIT_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the XWR14xx Memory Capture '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the XWR14xx Memory Capture objects'
	@$(DEL) $(MSS_MEMORY_CAPTURE_UNIT_TEST_OBJECTS) $(MSS_MEMORY_CAPTURE_UNIT_TEST_OUT) $(MSS_MEMORY_CAPTURE_UNIT_TEST_BIN)
	@$(DEL) $(MSS_MEMORY_CAPTURE_UNIT_TEST_MAP) $(MSS_MEMORY_CAPTURE_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the XWR14xx Memory Capture RTSC package'
	@$(DEL) $(MSS_MEMORY_CAPTURE_UNIT_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_MEMORY_CAPTURE_UNIT_TEST_DEPENDS)

