###################################################################################
# DMA Driver Makefile
###################################################################################
.PHONY: dmaDrv dmaDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
DMA_DRV_SOURCES = dma.c

###################################################################################
# Driver Source Files:
# - XWR14xx/XWR16xx/XWR18xx/XWR68xx:
#   DMA driver is available on R4F
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr14xx xwr16xx xwr18xx xwr68xx), )
DMA_DRV_SOURCES        += dma_$(MMWAVE_SDK_DEVICE_TYPE).c
DMA_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(DMA_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif
###################################################################################
# Driver Dependency:
###################################################################################
DMA_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DMA_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
DMA_R4F_DRV_LIB = lib/libdma_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

###################################################################################
# DMA Driver Build:
#   Build the R4 Library
###################################################################################
dmaDrv: buildDirectories $(DMA_R4F_DRV_LIB_OBJECTS)
ifndef DMA_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(DMA_R4F_DRV_LIB) $(DMA_R4F_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the DMA Driver Libraries
###################################################################################
dmaDrvClean:
	@echo 'Cleaning the DMA Driver Library Objects'
ifdef DMA_R4F_DRV_LIB_OBJECTS
	@$(DEL) $(DMA_R4F_DRV_DEPENDS)
	@$(DEL) $(DMA_R4F_DRV_LIB_OBJECTS) $(DMA_R4F_DRV_LIB)
endif
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DMA_R4F_DRV_DEPENDS)

