###################################################################################
# SOC Driver Makefile
###################################################################################
.PHONY: socDrv socDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c 	src
vpath %.asm     src
vpath %.c 	platform

###################################################################################
# Driver Source Files:
###################################################################################
SOC_DRV_SOURCES = soc.c soc_mpu.asm

###################################################################################
# Driver Source Files:
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
SOC_DRV_SOURCES += soc_xwr14xx.c
SOC_DRV_R4_ADDL_SOURCES =
SOC_DRV_C674_ADDL_SOURCES =
endif

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx), )
SOC_DRV_SOURCES += soc_$(MMWAVE_SDK_DEVICE_TYPE).c
SOC_DRV_R4_ADDL_SOURCES = soc_$(MMWAVE_SDK_DEVICE_TYPE)_mss.c
SOC_DRV_C674_ADDL_SOURCES = soc_$(MMWAVE_SDK_DEVICE_TYPE)_dss.c
endif

# Get the list of C and assembly files
SOC_DRV_C_SOURCES   = $(filter %.c,   $(SOC_DRV_SOURCES))
SOC_DRV_ASM_SOURCES = $(filter %.asm, $(SOC_DRV_SOURCES))

###################################################################################
# Driver Objects:
####################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
SOC_R4_DRV_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_C_SOURCES:.c=.$(R4F_OBJ_EXT)))
SOC_R4_DRV_OBJECTS  += $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_ASM_SOURCES:.asm=.o))
SOC_C674_DRV_OBJECTS =
else
SOC_R4_DRV_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_C_SOURCES:.c=.$(R4F_OBJ_EXT)))
SOC_R4_DRV_OBJECTS   += $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_R4_ADDL_SOURCES:.c=.$(R4F_OBJ_EXT)))
SOC_R4_DRV_OBJECTS   += $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_ASM_SOURCES:.asm=.o))
SOC_C674_DRV_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_C_SOURCES:.c=.$(C674_OBJ_EXT)))
SOC_C674_DRV_OBJECTS += $(addprefix $(PLATFORM_OBJDIR)/, $(SOC_DRV_C674_ADDL_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
SOC_C674_DRV_DEPENDS = $(SOC_DRV_C_SOURCES:.c=.$(C674_DEP_EXT))
SOC_C674_DRV_DEPENDS += $(SOC_DRV_C674_ADDL_SOURCES:.c=.$(C674_DEP_EXT))
SOC_R4F_DRV_DEPENDS  = $(SOC_DRV_C_SOURCES:.c=.$(R4F_DEP_EXT))
SOC_R4F_DRV_DEPENDS  += $(SOC_DRV_R4_ADDL_SOURCES:.c=.$(R4F_DEP_EXT))
SOC_R4F_DRV_DEPENDS  += $(SOC_DRV_ASM_SOURCES:.asm=.asm.d)

###################################################################################
# Driver Library Names:
###################################################################################
SOC_R4_DRV_LIB   = lib/libsoc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
SOC_C674_DRV_LIB = lib/libsoc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# SOC Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
socDrv: buildDirectories $(SOC_R4_DRV_OBJECTS) $(SOC_C674_DRV_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(SOC_R4_DRV_LIB)  $(SOC_R4_DRV_OBJECTS)
ifneq ($(SOC_C674_DRV_OBJECTS),"")
	$(C674_AR) $(C674_AR_OPTS) $(SOC_C674_DRV_LIB) $(SOC_C674_DRV_OBJECTS)
endif

###################################################################################
# Clean the SOC Driver Libraries
###################################################################################
socDrvClean:
	@echo 'Cleaning the SOC Driver Library Objects'
	@$(DEL) $(SOC_R4_DRV_OBJECTS) $(SOC_R4_DRV_LIB)
	@$(DEL) $(SOC_R4F_DRV_DEPENDS)
	@$(DEL) $(SOC_C674_DRV_OBJECTS) $(SOC_C674_DRV_LIB)
	@$(DEL) $(SOC_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(SOC_R4F_DRV_DEPENDS)
-include $(SOC_C674_DRV_DEPENDS)

