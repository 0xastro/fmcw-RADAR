###################################################################################
# dpedma Library Makefile
###################################################################################
.PHONY: dpedmaLib dpedmaLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
DPEDMA_HWA_LIB_SOURCES = dpedma.c \
                         dpedmahwa.c
else
DPEDMA_DSP_LIB_SOURCES = dpedma.c
endif

###################################################################################
# Library objects
#     Build for R4 and DSP
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
DPEDMA_HWA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_HWA_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
DPEDMA_HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_HWA_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))
else
DPEDMA_DSP_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_DSP_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Library Dependency:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
DPEDMA_HWA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_HWA_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
DPEDMA_HWA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_HWA_LIB_SOURCES:.c=.$(C674_DEP_EXT)))
else
DPEDMA_DSP_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_DSP_LIB_SOURCES:.c=.$(C674_DEP_EXT)))
endif

###################################################################################
# Library Names:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
DPEDMA_HWA_R4F_DRV_LIB  = lib/libdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
DPEDMA_HWA_C674_DRV_LIB = lib/libdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
else
DPEDMA_DSP_C674_DRV_LIB = lib/libdpedma_dsp_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
endif

###################################################################################
# Library Build:
#     - Build the R4 & DSP Library
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
dpedmaLib: buildDirectories $(DPEDMA_HWA_R4F_DRV_LIB_OBJECTS) $(DPEDMA_HWA_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(DPEDMA_HWA_R4F_DRV_LIB) $(DPEDMA_HWA_R4F_DRV_LIB_OBJECTS)
	$(C674_AR) $(C674_AR_OPTS) $(DPEDMA_HWA_C674_DRV_LIB) $(DPEDMA_HWA_C674_DRV_LIB_OBJECTS)
else
dpedmaLib: buildDirectories $(DPEDMA_DSP_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(C674_AR) $(C674_AR_OPTS) $(DPEDMA_DSP_C674_DRV_LIB) $(DPEDMA_DSP_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the Libraries
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
dpedmaLibClean:
	@echo 'Cleaning the dpedma Library Objects'
	@$(DEL) $(DPEDMA_HWA_R4F_DRV_LIB_OBJECTS) $(DPEDMA_HWA_R4F_DRV_LIB)
	@$(DEL) $(DPEDMA_HWA_C674_DRV_LIB_OBJECTS) $(DPEDMA_HWA_C674_DRV_LIB)
	@$(DEL) $(DPEDMA_HWA_R4F_DRV_DEPENDS) $(DPEDMA_HWA_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)
else
dpedmaLibClean:
	@echo 'Cleaning the dpedma Library Objects'
	@$(DEL) $(DPEDMA_DSP_C674_DRV_LIB_OBJECTS) $(DPEDMA_DSP_C674_DRV_LIB)
	@$(DEL) $(DPEDMA_DSP_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)
endif

###################################################################################
# Dependency handling
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
-include $(DPEDMA_HWA_R4F_DRV_DEPENDS)
-include $(DPEDMA_HWA_C674_DRV_DEPENDS)
else
-include $(DPEDMA_DSP_C674_DRV_DEPENDS)
endif

