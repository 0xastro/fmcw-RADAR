###################################################################################
# staticclutterproc Library Makefile
###################################################################################
.PHONY: staticclutterprocLib staticclutterprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
STATICCLUTTERPROC_LIB_SOURCES = staticclutterproc.c

###################################################################################
# Library objects
#     Build for R4 and DSP
###################################################################################
STATICCLUTTERPROC_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
STATICCLUTTERPROC_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
STATICCLUTTERPROC_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
STATICCLUTTERPROC_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(STATICCLUTTERPROC_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
STATICCLUTTERPROC_R4F_DRV_LIB  = lib/libstaticclutterproc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
STATICCLUTTERPROC_C674_DRV_LIB = lib/libstaticclutterproc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the R4 & DSP Library
###################################################################################
staticclutterprocLib: buildDirectories $(STATICCLUTTERPROC_R4F_DRV_LIB_OBJECTS) $(STATICCLUTTERPROC_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(STATICCLUTTERPROC_R4F_DRV_LIB) $(STATICCLUTTERPROC_R4F_DRV_LIB_OBJECTS)
	$(C674_AR) $(C674_AR_OPTS) $(STATICCLUTTERPROC_C674_DRV_LIB) $(STATICCLUTTERPROC_C674_DRV_LIB_OBJECTS)

###################################################################################
# Clean the Libraries
###################################################################################
staticclutterprocLibClean:
	@echo 'Cleaning the staticclutterproc Library Objects'
	@$(DEL) $(STATICCLUTTERPROC_R4F_DRV_LIB_OBJECTS) $(STATICCLUTTERPROC_R4F_DRV_LIB)
	@$(DEL) $(STATICCLUTTERPROC_C674_DRV_LIB_OBJECTS) $(STATICCLUTTERPROC_C674_DRV_LIB)
	@$(DEL) $(STATICCLUTTERPROC_R4F_DRV_DEPENDS) $(STATICCLUTTERPROC_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(STATICCLUTTERPROC_R4F_DRV_DEPENDS)
-include $(STATICCLUTTERPROC_C674_DRV_DEPENDS)

