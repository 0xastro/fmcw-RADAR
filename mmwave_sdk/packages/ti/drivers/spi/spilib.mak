###################################################################################
# SPI Driver Makefile
###################################################################################
.PHONY: spiDrv spiDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
SPI_DRV_SOURCES = SPI.c mibspi_dma.c
###################################################################################
# Driver Source Files:
# - XWR14xx/XWR16xx/XWR18xx/XWR68xx:
#   SPI driver is available for R4F
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr14xx xwr16xx xwr18xx xwr68xx), )
SPI_DRV_SOURCES += spi_$(MMWAVE_SDK_DEVICE_TYPE).c
SPI_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(SPI_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
SPI_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(SPI_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
SPI_R4F_DRV_LIB  = lib/libspi_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

###################################################################################
# SPI Driver Build:
#    Build the R4 Library
###################################################################################
spiDrv: buildDirectories $(SPI_R4F_DRV_LIB_OBJECTS)
ifndef SPI_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(SPI_R4F_DRV_LIB)  $(SPI_R4F_DRV_LIB_OBJECTS)
endif
###################################################################################
# Clean the SPI Driver Libraries
###################################################################################
spiDrvClean:
	@echo 'Cleaning the SPI Driver Library Objects'
	@$(DEL) $(SPI_R4F_DRV_LIB_OBJECTS) $(SPI_R4F_DRV_LIB)
	@$(DEL) $(SPI_R4F_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(SPI_R4F_DRV_DEPENDS)

