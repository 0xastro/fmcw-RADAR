###################################################################################
# DPM Control Makefile
###################################################################################
.PHONY: dpmLib dpmLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Module Source Files:
###################################################################################
DPM_LIB_SOURCES = dpm_core.c 		\
				  dpm_msg.c 		\
				  dpm_pipe.c 		\
				  dpm_listlib.c

###################################################################################
# Device Specific Module Source Files:
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
DPM_LIB_SOURCES          += dpm_$(MMWAVE_SDK_DEVICE_TYPE).c dpm_mailbox.c
DPM_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(DPM_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
DPM_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(DPM_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
DPM_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DPM_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
DPM_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DPM_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
DPM_R4F_DRV_LIB  = lib/libdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
DPM_C674_DRV_LIB = lib/libdpm_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# DPM Driver Build:
#  Build the R4 & DSP Library
###################################################################################
dpmLib: buildDirectories $(DPM_R4F_DRV_LIB_OBJECTS) $(DPM_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(DPM_R4F_DRV_LIB) $(DPM_R4F_DRV_LIB_OBJECTS)
ifneq ($(DPM_C674_DRV_LIB_OBJECTS),"")
	$(C674_AR) $(C674_AR_OPTS) $(DPM_C674_DRV_LIB) $(DPM_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the DPM Driver Libraries
###################################################################################
dpmLibClean:
	@echo 'Cleaning the DPM Driver Library Objects'
	@$(DEL) $(DPM_R4F_DRV_LIB_OBJECTS) $(DPM_R4F_DRV_LIB)
	@$(DEL) $(DPM_C674_DRV_LIB_OBJECTS) $(DPM_C674_DRV_LIB)
	@$(DEL) $(DPM_R4F_DRV_DEPENDS)
	@$(DEL) $(DPM_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DPM_R4F_DRV_DEPENDS)
-include $(DPM_C674_DRV_DEPENDS)

