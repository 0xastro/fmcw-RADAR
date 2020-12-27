###################################################################################
# aoaproc Library Makefile
###################################################################################
.PHONY: aoaprocLib aoaprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
AOAPROC_HWA_LIB_SOURCES = aoaprochwa.c

###################################################################################
# Library objects
#     Build for R4 and DSP
###################################################################################
AOAPROC_HWA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
AOAPROC_HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
AOAPROC_HWA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
AOAPROC_HWA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
AOAPROC_HWA_R4F_DRV_LIB  = lib/libaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
AOAPROC_HWA_C674_DRV_LIB = lib/libaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the R4 & DSP Library
###################################################################################
aoaprocLib: buildDirectories $(AOAPROC_HWA_R4F_DRV_LIB_OBJECTS) $(AOAPROC_HWA_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(AOAPROC_HWA_R4F_DRV_LIB) $(AOAPROC_HWA_R4F_DRV_LIB_OBJECTS)
	$(C674_AR) $(C674_AR_OPTS) $(AOAPROC_HWA_C674_DRV_LIB) $(AOAPROC_HWA_C674_DRV_LIB_OBJECTS)

###################################################################################
# Clean the Libraries
###################################################################################
aoaprocLibClean:
	@echo 'Cleaning the aoaproc Library Objects'
	@$(DEL) $(AOAPROC_HWA_R4F_DRV_LIB_OBJECTS) $(AOAPROC_HWA_R4F_DRV_LIB)
	@$(DEL) $(AOAPROC_HWA_C674_DRV_LIB_OBJECTS) $(AOAPROC_HWA_C674_DRV_LIB)
	@$(DEL) $(AOAPROC_HWA_R4F_DRV_DEPENDS) $(AOAPROC_HWA_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(AOAPROC_HWA_R4F_DRV_DEPENDS)
-include $(AOAPROC_HWA_C674_DRV_DEPENDS)

