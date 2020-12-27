###################################################################################
# cfarcaproc Library Makefile
###################################################################################
.PHONY: cfarcaprocLib cfarcaprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
CFARCAPROC_HWA_LIB_SOURCES = cfarcaprochwa.c

###################################################################################
# Library objects
#     Build for R4 and DSP
###################################################################################
CFARCAPROC_HWA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARCAPROC_HWA_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
CFARCAPROC_HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARCAPROC_HWA_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
CFARCAPROC_HWA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARCAPROC_HWA_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
CFARCAPROC_HWA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARCAPROC_HWA_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
CFARCAPROC_HWA_R4F_DRV_LIB  = lib/libcfarcaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
CFARCAPROC_HWA_C674_DRV_LIB = lib/libcfarcaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the R4 & DSP Library
###################################################################################
cfarcaprocLib: buildDirectories $(CFARCAPROC_HWA_R4F_DRV_LIB_OBJECTS) $(CFARCAPROC_HWA_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(CFARCAPROC_HWA_R4F_DRV_LIB) $(CFARCAPROC_HWA_R4F_DRV_LIB_OBJECTS)
	$(C674_AR) $(C674_AR_OPTS) $(CFARCAPROC_HWA_C674_DRV_LIB) $(CFARCAPROC_HWA_C674_DRV_LIB_OBJECTS)

###################################################################################
# Clean the Libraries
###################################################################################
cfarcaprocLibClean:
	@echo 'Cleaning the cfarcaproc Library Objects'
	@$(DEL) $(CFARCAPROC_HWA_R4F_DRV_LIB_OBJECTS) $(CFARCAPROC_HWA_R4F_DRV_LIB)
	@$(DEL) $(CFARCAPROC_HWA_C674_DRV_LIB_OBJECTS) $(CFARCAPROC_HWA_C674_DRV_LIB)
	@$(DEL) $(CFARCAPROC_HWA_R4F_DRV_DEPENDS) $(CFARCAPROC_HWA_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CFARCAPROC_HWA_R4F_DRV_DEPENDS)
-include $(CFARCAPROC_HWA_C674_DRV_DEPENDS)

