###################################################################################
# rangeproc Library Makefile
###################################################################################
.PHONY: rangeprocLib rangeprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Library Source Files:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
RANGEPROC_HWA_LIB_SOURCES = rangeprochwa.c 			
endif
RANGEPROC_DSP_LIB_SOURCES = rangeprocdsp.c 			

###################################################################################
# Library objects
#     Build for R4 and DSP
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
RANGEPROC_HWA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
RANGEPROC_HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

RANGEPROC_DSP_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_DSP_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
RANGEPROC_HWA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
RANGEPROC_HWA_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(C674_DEP_EXT)))
endif

RANGEPROC_DSP_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_DSP_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
RANGEPROC_HWA_R4F_DRV_LIB  = lib/librangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
RANGEPROC_HWA_C674_DRV_LIB = lib/librangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
endif

RANGEPROC_DSP_C674_DRV_LIB = lib/librangeproc_dsp_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the R4 & DSP Library
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
rangeprocHWALib: buildDirectories $(RANGEPROC_HWA_R4F_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(RANGEPROC_HWA_R4F_DRV_LIB) $(RANGEPROC_HWA_R4F_DRV_LIB_OBJECTS)
	$(C674_AR) $(C674_AR_OPTS) $(RANGEPROC_HWA_C674_DRV_LIB) $(RANGEPROC_HWA_C674_DRV_LIB_OBJECTS)
endif

rangeprocDSPLib: C674_CFLAGS += -i$(C674x_MATHLIB_INSTALL_PATH)/packages \
								-i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16_imre/c64P
rangeprocDSPLib: buildDirectories $(RANGEPROC_DSP_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(C674_AR) $(C674_AR_OPTS) $(RANGEPROC_DSP_C674_DRV_LIB) $(RANGEPROC_DSP_C674_DRV_LIB_OBJECTS)

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
rangeprocLib: rangeprocHWALib rangeprocDSPLib
else
rangeprocLib: rangeprocDSPLib
endif

###################################################################################
# Clean the Libraries
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
rangeprocHWALibClean:
	@echo 'Cleaning the rangeproc HWA Library Objects'
	@$(DEL) $(RANGEPROC_HWA_R4F_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_R4F_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_C674_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_C674_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_R4F_DRV_DEPENDS) $(RANGEPROC_HWA_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)
endif

rangeprocDSPLibClean:
	@echo 'Cleaning the rangeproc DSP Library Objects'
	@$(DEL) $(RANGEPROC_DSP_C674_DRV_LIB_OBJECTS) $(RANGEPROC_DSP_C674_DRV_LIB)
	@$(DEL) $(RANGEPROC_DSP_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
rangeprocLibClean: rangeprocHWALibClean rangeprocDSPLibClean
else
rangeprocLibClean: rangeprocDSPLibClean
endif

###################################################################################
# Dependency handling
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr18xx xwr68xx), )
-include $(RANGEPROC_HWA_R4F_DRV_DEPENDS)
-include $(RANGEPROC_HWA_C674_DRV_DEPENDS)
endif
-include $(RANGEPROC_DSP_C674_DRV_DEPENDS)

