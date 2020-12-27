###################################################################################
# OSAL Library Makefile
###################################################################################
.PHONY: osal osalClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src/ti_rtos

###################################################################################
# Driver Source Files:
###################################################################################
OSAL_DRV_SOURCES = DebugP_tirtos.c 		\
				   HwiP_tirtos.c 		\
				   SemaphoreP_tirtos.c 	\
				   MemoryP_tirtos.c \
                   CycleprofilerP_tirtos.c

###################################################################################
# Driver Objects:
# - XWR14xx:
#   OSAL driver is available only for the R4
# - XWR16xx/XWR18xx/XWR68xx:
#   OSAL Driver is available for both the R4 and DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
OSAL_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(OSAL_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
OSAL_C674_DRV_LIB_OBJECTS =
else
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),tms320c6748)
OSAL_R4F_DRV_LIB_OBJECTS  = 
OSAL_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(OSAL_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
else
OSAL_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(OSAL_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
OSAL_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(OSAL_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif
endif

###################################################################################
# Driver Dependency:
###################################################################################
OSAL_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(OSAL_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
OSAL_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(OSAL_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
OSAL_R4F_DRV_LIB   = lib/libosal_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
OSAL_C674_DRV_LIB  = lib/libosal_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# OSAL Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
osal: buildDirectories $(OSAL_R4F_DRV_LIB_OBJECTS) $(OSAL_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr68xx xwr18xx xwr16xx xwr14xx),)
	$(R4F_AR) $(R4F_AR_OPTS) $(OSAL_R4F_DRV_LIB)  $(OSAL_R4F_DRV_LIB_OBJECTS)
endif
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx),)
	$(C674_AR) $(C674_AR_OPTS) $(OSAL_C674_DRV_LIB) $(OSAL_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the OSAL Driver Libraries
###################################################################################
osalClean:
	@echo 'Cleaning the OSAL Driver Library Objects'
	@$(DEL) $(OSAL_R4F_DRV_DEPENDS) $(OSAL_C674_DRV_DEPENDS)
	@$(DEL) $(OSAL_R4F_DRV_LIB_OBJECTS) $(OSAL_R4F_DRV_LIB)
	@$(DEL) $(OSAL_C674_DRV_LIB_OBJECTS) $(OSAL_C674_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(OSAL_DRV_DEPENDS)

