###################################################################################
# HWA Driver Makefile
###################################################################################
.PHONY: hwaDrv hwaDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
# - XWR14xx
#   HWA driver is available for R4
# - XWR16xx:
#   HWA driver is not available
# - XWR18xx XWR68xx
#   HWA driver is available for R4 & DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
HWA_DRV_SOURCES         = hwa.c hwa_xwr14xx.c
HWA_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(HWA_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr18xx)
HWA_DRV_SOURCES         = hwa.c hwa_xwr18xx.c
HWA_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(HWA_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(HWA_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr68xx)
HWA_DRV_SOURCES         = hwa.c hwa_xwr68xx.c
HWA_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(HWA_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(HWA_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif
###################################################################################
# Driver Dependency:
###################################################################################
HWA_R4F_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
HWA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
HWA_R4F_DRV_LIB  = lib/libhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
HWA_C674_DRV_LIB = lib/libhwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# HWA Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx: Build is not applicable
# - XWR18xx XWR68xx: Build the R4 & DSP Library
###################################################################################
hwaDrv: buildDirectories $(HWA_R4F_DRV_LIB_OBJECTS) $(HWA_C674_DRV_LIB_OBJECTS)
ifndef HWA_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(HWA_R4F_DRV_LIB) $(HWA_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(HWA_C674_DRV_LIB) $(HWA_C674_DRV_LIB_OBJECTS)
endif
endif

###################################################################################
# Clean the HWA Driver Libraries
###################################################################################
hwaDrvClean:
	@echo 'Cleaning the HWA Driver Library Objects'
ifdef HWA_R4F_DRV_LIB_OBJECTS
	@$(DEL) $(HWA_R4F_DRV_DEPENDS)
	@$(DEL) $(HWA_R4F_DRV_LIB_OBJECTS) $(HWA_R4F_DRV_LIB)
endif
	@$(DEL) $(HWA_C674_DRV_LIB_OBJECTS) $(HWA_C674_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWA_R4F_DRV_DEPENDS)
-include $(HWA_C674_DRV_DEPENDS)

