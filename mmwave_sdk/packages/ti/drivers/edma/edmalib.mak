###################################################################################
#   EDMA Driver Makefile
###################################################################################
.PHONY: edmaDrv edmaDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
EDMA_DRV_SOURCES = edma.c edma_low_level.c edma_xwr1xxx.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   EDMA driver is available
# - XWR16xx/XWR18xx/XWR68xx:
#   EDMA driver is available for both the DSP and R4
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
EDMA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(EDMA_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
EDMA_C674_DRV_LIB_OBJECTS =
else
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),tms320c6748)
EDMA_R4F_DRV_LIB_OBJECTS  =
EDMA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(EDMA_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
else
EDMA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(EDMA_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
EDMA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(EDMA_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif
endif

###################################################################################
# Driver Dependency:
###################################################################################
EDMA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(EDMA_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
EDMA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(EDMA_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
EDMA_R4F_DRV_LIB  = lib/libedma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
EDMA_C674_DRV_LIB = lib/libedma_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# EDMA Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
edmaDrv: buildDirectories $(EDMA_R4F_DRV_LIB_OBJECTS) $(EDMA_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr14xx xwr18xx xwr68xx),)
	$(R4F_AR) $(R4F_AR_OPTS) $(EDMA_R4F_DRV_LIB)  $(EDMA_R4F_DRV_LIB_OBJECTS)
endif
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx),)
	$(C674_AR) $(C674_AR_OPTS) $(EDMA_C674_DRV_LIB) $(EDMA_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the EDMA Driver Libraries
###################################################################################
edmaDrvClean:
	@echo 'Cleaning the EDMA Driver Library Objects'
	@$(DEL) $(EDMA_R4F_DRV_LIB_OBJECTS) $(EDMA_R4F_DRV_LIB) $(EDMA_R4F_DRV_DEPENDS)
	@$(DEL) $(EDMA_C674_DRV_LIB_OBJECTS) $(EDMA_C674_DRV_LIB) $(EDMA_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(EDMA_R4F_DRV_DEPENDS)
-include $(EDMA_C674_DRV_DEPENDS)

