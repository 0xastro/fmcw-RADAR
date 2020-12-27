###################################################################################
# CRC Driver Makefile
###################################################################################
.PHONY: crcDrv crcDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
CRC_DRV_SOURCES = crc.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   CRC driver is available
# - XWR16xx/XWR18xx/XWR68xx:
#   CRC driver is available for both the DSP and R4
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
CRC_DRV_SOURCES         += crc_xwr14xx.c
CRC_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
CRC_C674_DRV_LIB_OBJECTS =
endif
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx), )
CRC_DRV_SOURCES         += crc_$(MMWAVE_SDK_DEVICE_TYPE).c
CRC_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
CRC_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
CRC_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
CRC_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(CRC_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
CRC_R4F_DRV_LIB  = lib/libcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
CRC_C674_DRV_LIB = lib/libcrc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# CRC Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
crcDrv: buildDirectories $(CRC_R4F_DRV_LIB_OBJECTS) $(CRC_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(CRC_R4F_DRV_LIB) $(CRC_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(CRC_C674_DRV_LIB) $(CRC_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the CRC Driver Libraries
###################################################################################
crcDrvClean:
	@echo 'Cleaning the CRC Driver Library Objects'
	@$(DEL) $(CRC_R4F_DRV_LIB_OBJECTS) $(CRC_R4F_DRV_LIB)
	@$(DEL) $(CRC_C674_DRV_LIB_OBJECTS) $(CRC_C674_DRV_LIB)
	@$(DEL) $(CRC_R4F_DRV_DEPENDS) $(CRC_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(CRC_R4F_DRV_DEPENDS)
-include $(CRC_C674_DRV_DEPENDS)

