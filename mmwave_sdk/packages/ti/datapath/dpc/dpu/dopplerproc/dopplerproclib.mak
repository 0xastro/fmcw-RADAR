###################################################################################
# dopplerproc Library Makefile
###################################################################################
.PHONY: dopplerprocLib dopplerprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
DOPPLERPROC_HWA_LIB_SOURCES = dopplerprochwa.c

###################################################################################
# Library objects
#     Build for R4 and DSP
###################################################################################
DOPPLERPROC_HWA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
DOPPLERPROC_HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
DOPPLERPROC_HWA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
DOPPLERPROC_HWA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
DOPPLERPROC_HWA_R4F_DRV_LIB  = lib/libdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
DOPPLERPROC_HWA_C674_DRV_LIB = lib/libdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the R4 & DSP Library
###################################################################################
dopplerprocLib: buildDirectories $(DOPPLERPROC_HWA_R4F_DRV_LIB_OBJECTS) $(DOPPLERPROC_HWA_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(DOPPLERPROC_HWA_R4F_DRV_LIB) $(DOPPLERPROC_HWA_R4F_DRV_LIB_OBJECTS)
	$(C674_AR) $(C674_AR_OPTS) $(DOPPLERPROC_HWA_C674_DRV_LIB) $(DOPPLERPROC_HWA_C674_DRV_LIB_OBJECTS)

###################################################################################
# Clean the Libraries
###################################################################################
dopplerprocLibClean:
	@echo 'Cleaning the dopplerproc Library Objects'
	@$(DEL) $(DOPPLERPROC_HWA_R4F_DRV_LIB_OBJECTS) $(DOPPLERPROC_HWA_R4F_DRV_LIB)
	@$(DEL) $(DOPPLERPROC_HWA_C674_DRV_LIB_OBJECTS) $(DOPPLERPROC_HWA_C674_DRV_LIB)
	@$(DEL) $(DOPPLERPROC_HWA_R4F_DRV_DEPENDS) $(DOPPLERPROC_HWA_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DOPPLERPROC_HWA_R4F_DRV_DEPENDS)
-include $(DOPPLERPROC_HWA_C674_DRV_DEPENDS)

