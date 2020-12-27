###################################################################################
# MATH utils Library Makefile
###################################################################################
.PHONY: mathUtilsLib mathUtilsLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Source Files:
###################################################################################
MATH_UTIL_SOURCES = mathutils.c

###################################################################################
# MATH utils Library:
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
MATH_UTIL_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(R4F_OBJ_EXT)))
else
MATH_UTIL_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(R4F_OBJ_EXT)))
MATH_UTIL_C674_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(C674_OBJ_EXT)))
endif
###################################################################################
# Library Dependency:
###################################################################################
MATH_UTIL_R4F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(R4F_DEP_EXT)))
MATH_UTIL_C674_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
MATH_UTIL_R4F_DRV_LIB  = lib/libmathutils.$(R4F_LIB_EXT)
MATH_UTIL_C674_DRV_LIB  = lib/libmathutils.$(C674_LIB_EXT)

###################################################################################
# MATH utils Library:
###################################################################################
mathUtilsLib: buildDirectories $(MATH_UTIL_R4F_LIB_OBJECTS) $(MATH_UTIL_C674_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
ifdef MATH_UTIL_R4F_LIB_OBJECTS
	$(R4F_AR) $(R4F_AR_OPTS) $(MATH_UTIL_R4F_DRV_LIB) $(MATH_UTIL_R4F_LIB_OBJECTS)
endif
ifdef MATH_UTIL_C674_LIB_OBJECTS
	$(C674_AR) $(C674_AR_OPTS) $(MATH_UTIL_C674_DRV_LIB) $(MATH_UTIL_C674_LIB_OBJECTS)
endif

###################################################################################
# Clean the math utils Library
###################################################################################
mathUtilsLibClean:
	@echo 'Cleaning the MATH utils Library Objects'
	$(DEL) $(MATH_UTIL_R4F_LIB_OBJECTS) $(MATH_UTIL_R4F_DRV_LIB)
	$(DEL) $(MATH_UTIL_R4F_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(MATH_UTIL_R4F_DEPENDS)

