###################################################################################
# ADCBUF Driver Makefile
###################################################################################
.PHONY: adcbufDrv adcbufDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
ADCBUF_DRV_SOURCES = ADCBuf.c adcbuf_mmwave.c

###################################################################################
# Driver Objects:
# - XWR14xx: Build only for R4
# - XWR16xx/XWR18xx/XWR68xx: Build R4 and DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
ADCBUF_DRV_SOURCES         += adcbuf_xwr1xxx.c
ADCBUF_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
ADCBUF_C674_DRV_LIB_OBJECTS =
else
ADCBUF_DRV_SOURCES         += adcbuf_xwr1xxx.c
ADCBUF_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
ADCBUF_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
ADCBUF_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
ADCBUF_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(ADCBUF_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
ADCBUF_R4F_DRV_LIB  = lib/libadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
ADCBUF_C674_DRV_LIB = lib/libadcbuf_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# ADCBUF Driver Build:
###################################################################################
adcbufDrv: buildDirectories $(ADCBUF_R4F_DRV_LIB_OBJECTS) $(ADCBUF_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(ADCBUF_R4F_DRV_LIB) $(ADCBUF_R4F_DRV_LIB_OBJECTS)
ifneq ($(ADCBUF_C674_DRV_LIB_OBJECTS),"")
	$(C674_AR) $(C674_AR_OPTS) $(ADCBUF_C674_DRV_LIB) $(ADCBUF_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the ADCBUF Driver Libraries
###################################################################################
adcbufDrvClean:
	@echo 'Cleaning the ADCBUF Driver Library Objects'
	@$(DEL) $(ADCBUF_R4F_DRV_LIB_OBJECTS) $(ADCBUF_R4F_DRV_LIB)
	@$(DEL) $(ADCBUF_C674_DRV_LIB_OBJECTS) $(ADCBUF_C674_DRV_LIB)
	@$(DEL) $(ADCBUF_R4F_DRV_DEPENDS) $(ADCBUF_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(ADCBUF_R4F_DRV_DEPENDS)
-include $(ADCBUF_C674_DRV_DEPENDS)

