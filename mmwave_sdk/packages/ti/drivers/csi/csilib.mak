###################################################################################
# CSI Driver Makefile
###################################################################################
.PHONY: csiDrv csiDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
CSI_DRV_SOURCES = csi.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   CSI driver is available
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
CSI_DRV_SOURCES         += csi_xwr14xx.c
CSI_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(CSI_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
CSI_R4F_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(CSI_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
CSI_R4F_DRV_LIB  = lib/libcsi_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

###################################################################################
# CSI Driver Build:
# - XWR14xx: Build the R4 Library
###################################################################################
csiDrv: buildDirectories $(CSI_R4F_DRV_LIB_OBJECTS)
ifndef CSI_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(CSI_R4F_DRV_LIB) $(CSI_R4F_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the CSI Driver Libraries
###################################################################################
csiDrvClean:
	@echo 'Cleaning the CSI Driver Library Objects'
	@$(DEL) $(CSI_R4F_DRV_LIB_OBJECTS) $(CSI_R4F_DRV_LIB)
	@$(DEL) $(CSI_R4F_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CSI_R4F_DRV_DEPENDS)

