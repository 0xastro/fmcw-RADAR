###################################################################################
# mmWave Makefile
###################################################################################
.PHONY: mmwaveLib mmwaveLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
MMWAVE_SOURCES = mmwave.c			\
				 mmwave_link.c		\
				 mmwave_osal.c		\
				 mmwave_fullcfg.c	\
				 mmwave_listlib.c

###################################################################################
# Driver Objects:
# - XWR14xx: Build only for R4
# - XWR16xx/XWR18xx/XWR68xx: Build R4 and DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
MMWAVE_SOURCES         += mmwave_singlecore.c
MMWAVE_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R4F_OBJ_EXT)))
MMWAVE_C674_LIB_OBJECTS =
else ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
MMWAVE_SOURCES         += mmwave_dualcore.c
MMWAVE_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R4F_OBJ_EXT)))
MMWAVE_C674_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
MMWAVE_R4F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R4F_DEP_EXT)))
MMWAVE_C674_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
MMWAVE_R4F_DRV_LIB  = lib/libmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MMWAVE_C674_DRV_LIB = lib/libmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# mmWave Library:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
mmwaveCtrlLib: buildDirectories $(MMWAVE_R4F_LIB_OBJECTS) $(MMWAVE_C674_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(MMWAVE_R4F_DRV_LIB) $(MMWAVE_R4F_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx),)
	$(C674_AR) $(C674_AR_OPTS) $(MMWAVE_C674_DRV_LIB) $(MMWAVE_C674_LIB_OBJECTS)
endif

###################################################################################
# Clean the mmWave Library
###################################################################################
mmwaveCtrlLibClean:
	@echo 'Cleaning the mmWave Library Objects'
	@$(DEL) $(MMWAVE_R4F_LIB_OBJECTS) $(MMWAVE_R4F_DRV_LIB)
	@$(DEL) $(MMWAVE_C674_LIB_OBJECTS) $(MMWAVE_C674_DRV_LIB)
	@$(DEL) $(MMWAVE_R4F_DEPENDS) $(MMWAVE_C674_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_R4F_DEPENDS)
-include $(MMWAVE_C674_DEPENDS)

