###################################################################################
# PINMUX Driver Makefile
###################################################################################
.PHONY: pinmuxDrv pinmuxDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
PINMUX_DRV_SOURCES = pinmux.c

###################################################################################
# Driver Source Files:
# - XWR14xx/XWR16xx/XWR18xx/XWR68xx:
#   PINMUX driver is available for only the R4
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr14xx xwr16xx xwr18xx xwr68xx), )
PINMUX_DRV_SOURCES         += pinmux_xwr1xxx.c
PINMUX_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(PINMUX_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
else
PINMUX_DRV_SOURCES         +=
PINMUX_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(PINMUX_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
PINMUX_R4F_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(PINMUX_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
PINMUX_R4F_DRV_LIB = lib/libpinmux_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

###################################################################################
# Pinmux Driver Build:
#   Build the R4 Library
###################################################################################
pinmuxDrv: buildDirectories $(PINMUX_R4F_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(PINMUX_R4F_DRV_LIB)  $(PINMUX_R4F_DRV_LIB_OBJECTS)

###################################################################################
# Clean the Pinmux Driver Libraries
###################################################################################
pinmuxDrvClean:
	@echo 'Cleaning the PINMUX Driver Library Objects'
	@$(DEL) $(PINMUX_R4F_DRV_DEPENDS)
	@$(DEL) $(PINMUX_R4F_DRV_LIB_OBJECTS) $(PINMUX_R4F_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(PINMUX_R4F_DRV_DEPENDS)



