###################################################################################
# CCS Debug makefile for DSS
###################################################################################
.PHONY: dssCCSDebug dssCCSDebugClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# The CCS Debug Utility application does not require any additional libraries
###################################################################################
CCS_DEBUG_DSS_STD_LIBS = $(C674_COMMON_STD_LIB)
CCS_DEBUG_DSS_LOC_LIBS = $(C674_COMMON_LOC_LIB)

###################################################################################
# CCS Debug Utility:
###################################################################################
CCS_DEBUG_DSS_SOURCES       = main.c
CCS_DEBUG_DSS_OUT           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_dss.$(C674_EXE_EXT)
CCS_DEBUG_DSS_MAP           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_dss.map
CCS_DEBUG_DSS_LINKER_CMD    =  dss_ccsdebug_linker.cmd
CCS_DEBUG_DSS_C674_CFG       = dss.cfg
CCS_DEBUG_DSS_C674_CONFIGPKG = c674_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CCS_DEBUG_DSS_DEPENDS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_DSS_SOURCES:.c=.$(C674_DEP_EXT)))
CCS_DEBUG_DSS_OBJECTS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_DSS_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
ccsdebug_dss.cfg:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(C674_XSFLAGS) -o $(CCS_DEBUG_DSS_C674_CONFIGPKG) $(CCS_DEBUG_DSS_C674_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
dssCCSDebug: BUILD_CONFIGPKG=$(CCS_DEBUG_DSS_C674_CONFIGPKG)
dssCCSDebug: C674_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
dssCCSDebug: buildDirectories ccsdebug_dss.cfg $(CCS_DEBUG_DSS_OBJECTS)
	$(C674_LD) $(C674_LDFLAGS) $(CCS_DEBUG_DSS_LOC_LIBS) $(CCS_DEBUG_DSS_STD_LIBS)            \
	-l$(CCS_DEBUG_DSS_C674_CONFIGPKG)/linker.cmd --map_file=$(CCS_DEBUG_DSS_MAP)             \
	$(CCS_DEBUG_DSS_OBJECTS) $(PLATFORM_C674X_LINK_CMD)                                    \
	$(CCS_DEBUG_DSS_LINKER_CMD) $(C674_LD_RTS_FLAGS) -o $(CCS_DEBUG_DSS_OUT)
	@echo 'Built the CCS Debug DSS Utility'


###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
dssCCSDebugClean:
	@echo 'Cleaning the CCS Debug Utility DSS objects'
	@$(DEL) $(CCS_DEBUG_DSS_OBJECTS) $(CCS_DEBUG_DSS_OUT)
	@$(DEL) $(CCS_DEBUG_DSS_MAP)
	@$(DEL) $(CCS_DEBUG_DSS_DEPENDS)
	@echo 'Cleaning the CCS Debug DSS RTSC package'
	@$(DEL) $(CCS_DEBUG_DSS_C674_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)


###################################################################################
# Dependency handling
###################################################################################
-include $(CCS_DEBUG_DSS_DEPENDS)

