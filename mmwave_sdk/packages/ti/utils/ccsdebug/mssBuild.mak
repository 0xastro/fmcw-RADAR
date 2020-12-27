###################################################################################
# CCS Debug makefile for MSS
###################################################################################
.PHONY: mssCCSDebug mssCCSDebugClean 

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# The CCS Debug Utility application does not require any additional libraries
###################################################################################
CCS_DEBUG_MSS_STD_LIBS = $(R4F_COMMON_STD_LIB)
CCS_DEBUG_MSS_LOC_LIBS = $(R4F_COMMON_LOC_LIB)

###################################################################################
# CCS Debug Utility:
###################################################################################
CCS_DEBUG_MSS_SOURCES       = main.c
CCS_DEBUG_MSS_OUT           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_mss.$(R4F_EXE_EXT)
CCS_DEBUG_MSS_MAP           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_mss.map
CCS_DEBUG_MSS_LINKER_CMD    = r4ccsdebug_linker.cmd
CCS_DEBUG_MSS_R4F_CFG       = r4.cfg
CCS_DEBUG_MSS_R4F_CONFIGPKG = r4_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
CCS_DEBUG_MSS_DEPENDS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_MSS_SOURCES:.c=.$(R4F_DEP_EXT)))
CCS_DEBUG_MSS_OBJECTS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_MSS_SOURCES:.c=.$(R4F_OBJ_EXT)))

###################################################################################
# RTSC Configuration:
###################################################################################
ccsdebug_mss.cfg:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(CCS_DEBUG_MSS_R4F_CONFIGPKG) $(CCS_DEBUG_MSS_R4F_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
mssCCSDebug: BUILD_CONFIGPKG=$(CCS_DEBUG_MSS_R4F_CONFIGPKG)
mssCCSDebug: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
mssCCSDebug: buildDirectories ccsdebug_mss.cfg $(CCS_DEBUG_MSS_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(CCS_DEBUG_MSS_LOC_LIBS) $(CCS_DEBUG_MSS_STD_LIBS)            \
	-l$(CCS_DEBUG_MSS_R4F_CONFIGPKG)/linker.cmd --map_file=$(CCS_DEBUG_MSS_MAP)             \
	$(CCS_DEBUG_MSS_OBJECTS) $(PLATFORM_R4F_LINK_CMD)                                    \
	$(CCS_DEBUG_MSS_LINKER_CMD) $(R4F_LD_RTS_FLAGS) -o $(CCS_DEBUG_MSS_OUT)
	@echo 'Built the CCS Debug MSS Utility'

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
mssCCSDebugClean:
	@echo 'Cleaning the CCS Debug Utility MSS objects'
	@$(DEL) $(CCS_DEBUG_MSS_OBJECTS) $(CCS_DEBUG_MSS_OUT)
	@$(DEL) $(CCS_DEBUG_MSS_MAP)
	@$(DEL) $(CCS_DEBUG_MSS_DEPENDS)
	@echo 'Cleaning the CCS Debug MSS RTSC package'
	@$(DEL) $(CCS_DEBUG_MSS_R4F_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)


###################################################################################
# Dependency handling
###################################################################################
-include $(CCS_DEBUG_MSS_DEPENDS)

