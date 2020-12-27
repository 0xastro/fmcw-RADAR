###################################################################################
#  mmWave Link Makefile
###################################################################################
.PHONY: link linkClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Driver Source Files:
###################################################################################
MMWAVE_LINK_SOURCES = rl_controller.c 		\
					  rl_device.c			\
					  rl_driver.c 			\
					  rl_monitoring.c 		\
					  rl_sensor.c

###################################################################################
# Library Objects:
# - XWR14xx:
#   mmWave Library is available for R4
# - XWR16xx/XWR18xx/XWR68xx:
#   mmWave Library is available for both the DSP and R4
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
MMWAVE_R4F_LINK_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_SOURCES:.c=.$(R4F_OBJ_EXT)))
MMWAVE_C674_LINK_LIB_OBJECTS =
else
MMWAVE_R4F_LINK_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_SOURCES:.c=.$(R4F_OBJ_EXT)))
MMWAVE_C674_LINK_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Lib Dependency:
###################################################################################
MMWAVE_R4F_LINK_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_SOURCES:.c=.$(R4F_DEP_EXT)))
MMWAVE_C674_LINK_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_LINK_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
MMWAVE_R4F_LINK_DRV_LIB  = lib/libmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MMWAVE_C674_LINK_DRV_LIB = lib/libmmwavelink_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# mmWave Link Build:
###################################################################################
link: buildDirectories $(MMWAVE_R4F_LINK_LIB_OBJECTS) $(MMWAVE_C674_LINK_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(MMWAVE_R4F_LINK_DRV_LIB)  $(MMWAVE_R4F_LINK_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(MMWAVE_C674_LINK_DRV_LIB) $(MMWAVE_C674_LINK_LIB_OBJECTS)
endif

###################################################################################
# Clean the mmWave Link Libraries
###################################################################################
linkClean:
	@echo 'Cleaning the mmWave Link Library Objects'
	@$(DEL) $(MMWAVE_R4F_LINK_LIB_OBJECTS) $(MMWAVE_R4F_LINK_DRV_LIB)
	@$(DEL) $(MMWAVE_R4F_LINK_DEPENDS)
	@$(DEL) $(MMWAVE_C674_LINK_LIB_OBJECTS) $(MMWAVE_C674_LINK_DRV_LIB)
	@$(DEL) $(MMWAVE_C674_LINK_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_R4F_LINK_DEPENDS)
-include $(MMWAVE_C674_LINK_DEPENDS)

