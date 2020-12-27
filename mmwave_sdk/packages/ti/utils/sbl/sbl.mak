###################################################################################
# SBL Makefile
###################################################################################
.PHONY: sbl sblClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# The SBL uses the common libraries + Test Logger Library
###################################################################################
SBL_STD_LIBS = $(R4F_COMMON_STD_LIB)									\
						  -llibpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibesm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibuart_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibqspiflash_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibqspi_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
						  -llibcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)		\
					      -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

SBL_LOC_LIBS = $(R4F_COMMON_LOC_LIB)									\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/pinmux/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/esm/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/dma/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/uart/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/qspi/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/qspiflash/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/crc/lib	\
						  -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Source Files:
# SBL is avialable for XWR16xx/XWR18xx/XWR68xx
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
SBL_SOURCES = transport.c buffer_mgmt.c sbl.c image_parser.c image_validity.c \
			  metaheader_parser.c  multicoreimage_parser.c main.c crc16.c
SBL_SOURCES   += sbl_$(MMWAVE_SDK_DEVICE_TYPE).c
endif

###################################################################################
# Driver Dependency:
###################################################################################
SBL_CFG       		= sbl.cfg
SBL_CONFIGPKG 		= mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
SBL_MAP       		= $(MMWAVE_SDK_DEVICE_TYPE)_sbl_mss.map
SBL_OUT       		= $(MMWAVE_SDK_DEVICE_TYPE)_sbl_mss.$(R4F_EXE_EXT)
SBL_BIN       		= $(MMWAVE_SDK_DEVICE_TYPE)_sbl.bin
SBL_PLATFORM_CMD	= xwr_r4f.cmd
SBL_APP_CMD  		= sbl_linker.cmd
SBL_DEPENDS   		= $(addprefix $(PLATFORM_OBJDIR)/, $(SBL_SOURCES:.c=.$(R4F_DEP_EXT)))
SBL_OBJECTS   		= $(addprefix $(PLATFORM_OBJDIR)/, $(SBL_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# Enabling SBL module flags
###################################################################################
R4F_LDFLAGS  += --diag_suppress=10278

###################################################################################
# RTSC Configuration:
###################################################################################
sblRTSC: $(R4_CFG)
ifndef SBL_SOURCES
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE_TYPE)"
else
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(SBL_CONFIGPKG) $(SBL_CFG)
	@echo 'Finished configuring packages'
	@echo ' '
endif

###################################################################################
# SBL Build:
#   Build the R4 out file
###################################################################################
sbl: BUILD_CONFIGPKG=$(SBL_CONFIGPKG)
sbl: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
sbl: buildDirectories sblRTSC $(SBL_OBJECTS)
ifndef SBL_SOURCES
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE_TYPE)"
else
	$(R4F_LD) $(R4F_LDFLAGS) $(SBL_LOC_LIBS) $(SBL_STD_LIBS)	\
	-l$(SBL_CONFIGPKG)/linker.cmd --map_file=$(SBL_MAP) 		\
	$(SBL_OBJECTS) $(SBL_PLATFORM_CMD) $(SBL_APP_CMD) 		\
	$(R4F_LD_RTS_FLAGS) -o $(SBL_OUT)
	@echo '******************************************************************************'
	@echo 'Built the SBL OUT file'
	@echo '******************************************************************************'
endif

###################################################################################
# Clean the SBL objects
###################################################################################
sblClean:
ifndef SBL_SOURCES
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE_TYPE)"
else
	@echo 'Cleaning the SBL Objects'
	@$(DEL) $(SBL_OBJECTS) $(SBL_OUT) $(SBL_BIN)
	@$(DEL) $(SBL_MAP) $(SBL_DEPENDS)
	@echo 'Cleaning the SBL R4 Unit RTSC package'
	@$(DEL) $(SBL_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)
endif

###################################################################################
# Dependency handling
###################################################################################
-include $(SBL_DEPENDS)
