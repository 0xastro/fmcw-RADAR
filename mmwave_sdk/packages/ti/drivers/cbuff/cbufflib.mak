###################################################################################
# CBUFF Driver Makefile
###################################################################################
.PHONY: cbuffDrv cbuffDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
CBUFF_DRV_SOURCES = cbuff.c cbuff_edma.c cbuff_transfer.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   CBUFF Driver with support for the CSI High speed interface.
# - XWR16xx/XWR18xx/XWR68xx:
#   CBUFF Driver with support for the LVDS High speed interface. Only on R4F
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
CBUFF_DRV_SOURCES          += cbuff_csi.c cbuff_lvds.c cbuff_xwr14xx.c
CBUFF_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(CBUFF_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
CBUFF_C674_DRV_LIB_OBJECTS =
endif
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
CBUFF_DRV_SOURCES          += cbuff_lvds.c cbuff_$(MMWAVE_SDK_DEVICE_TYPE).c
CBUFF_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(CBUFF_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
CBUFF_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(CBUFF_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
CBUFF_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CBUFF_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
CBUFF_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/,  $(CBUFF_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
CBUFF_R4F_DRV_LIB  = lib/libcbuff_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
CBUFF_C674_DRV_LIB = lib/libcbuff_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# CBUFF Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
cbuffDrv: buildDirectories $(CBUFF_R4F_DRV_LIB_OBJECTS) $(CBUFF_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(CBUFF_R4F_DRV_LIB) $(CBUFF_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(CBUFF_C674_DRV_LIB) $(CBUFF_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the CBUFF Driver Libraries
###################################################################################
cbuffDrvClean:
	@echo 'Cleaning the CBUFF Driver Library Objects'
	@$(DEL) $(CBUFF_R4F_DRV_LIB_OBJECTS) $(CBUFF_R4F_DRV_LIB)
	@$(DEL) $(CBUFF_C674_DRV_LIB_OBJECTS) $(CBUFF_C674_DRV_LIB)
	@$(DEL) $(CBUFF_R4F_DRV_DEPENDS)
	@$(DEL) $(CBUFF_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CBUFF_R4F_DRV_DEPENDS)
-include $(CBUFF_C674_DRV_DEPENDS)

