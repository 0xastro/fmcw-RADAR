###################################################################################
# CAN Driver Makefile
###################################################################################
.PHONY: canDrv canDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
CAN_DRV_SOURCES = can.c dcan.c

###################################################################################
# Driver Source Files:
#   CAN driver is available for the R4
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr14xx xwr16xx xwr18xx), )
CAN_DRV_SOURCES        += can_$(MMWAVE_SDK_DEVICE_TYPE).c
CAN_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(CAN_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
CAN_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CAN_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr14xx xwr16xx xwr18xx), )
CAN_R4F_DRV_LIB = lib/libcan_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
endif

###################################################################################
# CAN Driver Build:
###################################################################################
canDrv: buildDirectories $(CAN_R4F_DRV_LIB_OBJECTS)
ifndef CAN_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(CAN_R4F_DRV_LIB) $(CAN_R4F_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the CAN Driver Libraries
###################################################################################
canDrvClean:
	@echo 'Cleaning the CAN Driver Library Objects'
ifndef CAN_R4F_DRV_LIB_OBJECTS
	@$(DEL) $(CAN_R4F_DRV_DEPENDS)
	@$(DEL) $(CAN_R4F_DRV_LIB_OBJECTS) $(CAN_R4F_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)
endif

###################################################################################
# Dependency handling
###################################################################################
-include $(CAN_R4F_DRV_DEPENDS)


