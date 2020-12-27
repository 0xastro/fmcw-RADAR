###################################################################################
# HSI Header Library Makefile
###################################################################################
.PHONY: hsiHeaderLib hsiHeaderLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Source Files:
###################################################################################
HSI_HEADER_UTIL_SOURCES = hsiheader.c

###################################################################################
# HSI Header Library:
# - XWR14xx:
#   HSI Header Library is available
# - XWR16xx/XWR18xx/XWR68xx:
#   HSI Header Library is available for both the DSP and R4
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
HSI_HEADER_UTIL_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R4F_OBJ_EXT)))
else
HSI_HEADER_UTIL_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R4F_OBJ_EXT)))
HSI_HEADER_UTIL_C674_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Library Dependency:
###################################################################################
HSI_HEADER_UTIL_R4F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R4F_DEP_EXT)))
HSI_HEADER_UTIL_C674_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
HSI_HEADER_UTIL_R4F_DRV_LIB  = lib/libhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
HSI_HEADER_UTIL_C674_DRV_LIB = lib/libhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# HSI Header Library:
###################################################################################
hsiHeaderLib: buildDirectories $(HSI_HEADER_UTIL_R4F_LIB_OBJECTS) $(HSI_HEADER_UTIL_C674_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(HSI_HEADER_UTIL_R4F_DRV_LIB) $(HSI_HEADER_UTIL_R4F_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx),)
	$(C674_AR) $(C674_AR_OPTS) $(HSI_HEADER_UTIL_C674_DRV_LIB) $(HSI_HEADER_UTIL_C674_LIB_OBJECTS)
endif

###################################################################################
# Clean the HSI Header Library
###################################################################################
hsiHeaderLibClean:
	@echo 'Cleaning the HSI Header Library Objects'
	@$(DEL) $(HSI_HEADER_UTIL_R4F_LIB_OBJECTS) $(HSI_HEADER_UTIL_R4F_DRV_LIB)
	@$(DEL) $(HSI_HEADER_UTIL_C674_LIB_OBJECTS) $(HSI_HEADER_UTIL_C674_DRV_LIB)
	@$(DEL) $(HSI_HEADER_UTIL_R4F_DEPENDS)
	@$(DEL) $(HSI_HEADER_UTIL_C674_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(HSI_HEADER_UTIL_R4F_DEPENDS)
-include $(HSI_HEADER_UTIL_C674_DEPENDS)

