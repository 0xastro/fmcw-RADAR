###################################################################################
# QSPI Driver Makefile
###################################################################################
.PHONY: qspiDrv qspiDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
QSPI_DRV_SOURCES = qspi.c
###################################################################################
# Driver Source Files:
# - XWR14xx/XWR16xx/XWR18xx/XWR68xx:
#   QSPI driver is available for R4
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr14xx xwr16xx xwr18xx xwr68xx), )
QSPI_DRV_SOURCES        += qspi_$(MMWAVE_SDK_DEVICE_TYPE).c
QSPI_R4F_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(QSPI_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
QSPI_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(QSPI_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
QSPI_R4F_DRV_LIB  = lib/libqspi_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

###################################################################################
# QSPI Driver Build:
#   Build the R4 Library
###################################################################################
qspiDrv: buildDirectories $(QSPI_R4F_DRV_LIB_OBJECTS)
ifndef QSPI_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(QSPI_R4F_DRV_LIB)  $(QSPI_R4F_DRV_LIB_OBJECTS)
endif
###################################################################################
# Clean the QSPI Driver Libraries
###################################################################################
qspiDrvClean:
	@echo 'Cleaning the QSPI Driver Library Objects'
	@$(DEL) $(QSPI_R4F_DRV_LIB_OBJECTS) $(QSPI_R4F_DRV_LIB)
	@$(DEL) $(QSPI_R4F_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(QSPI_R4F_DRV_DEPENDS)

