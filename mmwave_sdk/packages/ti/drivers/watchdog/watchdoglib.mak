###################################################################################
# Watchdog Driver Makefile
###################################################################################
.PHONY: watchdogDrv watchdogDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
WATCHDOG_DRV_SOURCES = Watchdog.c watchdog_rti.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   Watchdog driver is available for R4
# - XWR16xx/XWR18xx/XWR68xx:
#   Watchdog driver is available for both R4 and DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
WATCHDOG_DRV_SOURCES += watchdog_$(MMWAVE_SDK_DEVICE_TYPE).c
WATCHDOG_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(WATCHDOG_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
WATCHDOG_C674_DRV_LIB_OBJECTS =
else
WATCHDOG_DRV_SOURCES += watchdog_$(MMWAVE_SDK_DEVICE_TYPE).c
WATCHDOG_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(WATCHDOG_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
WATCHDOG_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(WATCHDOG_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
WATCHDOG_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(WATCHDOG_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
WATCHDOG_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(WATCHDOG_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
WATCHDOG_R4F_DRV_LIB  = lib/libwatchdog_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
WATCHDOG_C674_DRV_LIB = lib/libwatchdog_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Watchdog Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 Library & DSP library
###################################################################################
watchdogDrv: buildDirectories $(WATCHDOG_R4F_DRV_LIB_OBJECTS) $(WATCHDOG_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(WATCHDOG_R4F_DRV_LIB)  $(WATCHDOG_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx),)
	$(C674_AR) $(C674_AR_OPTS) $(WATCHDOG_C674_DRV_LIB) $(WATCHDOG_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the WATCHDOG Driver Libraries
###################################################################################
watchdogDrvClean:
	@echo 'Cleaning the Watchdog Driver Library Objects'
	@$(DEL) $(WATCHDOG_R4F_DRV_LIB_OBJECTS) $(WATCHDOG_R4F_DRV_LIB)
	@$(DEL) $(WATCHDOG_C674_DRV_LIB_OBJECTS) $(WATCHDOG_C674_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(WATCHDOG_R4F_DRV_DEPENDS)
-include $(WATCHDOG_C674_DRV_DEPENDS)

