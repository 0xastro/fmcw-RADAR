###################################################################################
# ESM Driver Makefile
###################################################################################
.PHONY: esmDrv esmDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
ESM_DRV_SOURCES = esm.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   ESM driver is available
# - XWR16xx/XWR18xx/XWR68xx:
#   ESM driver is available only on the R4
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
ESM_DRV_SOURCES         += esm_xwr1xxx.c
ESM_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(ESM_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
ESM_C674_DRV_LIB_OBJECTS =
else
ESM_DRV_SOURCES         += esm_xwr1xxx.c
ESM_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(ESM_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
ESM_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(ESM_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
ESM_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(ESM_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
ESM_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(ESM_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
ESM_R4F_DRV_LIB  = lib/libesm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
ESM_C674_DRV_LIB = lib/libesm_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# ESM Driver Build:
#   Build the R4 Library
###################################################################################
esmDrv: buildDirectories $(ESM_R4F_DRV_LIB_OBJECTS) $(ESM_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(ESM_R4F_DRV_LIB)  $(ESM_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(ESM_C674_DRV_LIB) $(ESM_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the ESM Driver Libraries
###################################################################################
esmDrvClean:
	@echo 'Cleaning the ESM Driver Library Objects'
	@$(DEL) $(ESM_R4F_DRV_DEPENDS)
	@$(DEL) $(ESM_R4F_DRV_LIB_OBJECTS) $(ESM_R4F_DRV_LIB)
	@$(DEL) $(ESM_C674_DRV_LIB_OBJECTS) $(ESM_C674_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(ESM_R4F_DRV_DEPENDS)
-include $(ESM_C674_DRV_DEPENDS)

