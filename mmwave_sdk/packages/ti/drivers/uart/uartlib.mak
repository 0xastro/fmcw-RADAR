###################################################################################
# UART Driver Makefile
###################################################################################
.PHONY: uartDrv uartDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
UART_DRV_SOURCES = UART.c uartsci.c uartsci_dma.c uartsci_edma.c uartsci_nodma.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   UART driver is available
# - XWR16xx/XWR18XX/XWR68XX:
#   UART driver is available for both the DSP and R4
###################################################################################

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
UART_DRV_SOURCES += uart_xwr14xx.c
UART_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(UART_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
UART_C674_DRV_LIB_OBJECTS =
endif
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
UART_DRV_SOURCES += uart_$(MMWAVE_SDK_DEVICE_TYPE).c
UART_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(UART_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
UART_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(UART_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
endif
###################################################################################
# Driver Dependency:
###################################################################################
UART_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(UART_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
UART_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(UART_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
UART_R4F_DRV_LIB  = lib/libuart_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
UART_C674_DRV_LIB = lib/libuart_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# UART Driver Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
uartDrv: buildDirectories $(UART_R4F_DRV_LIB_OBJECTS) $(UART_C674_DRV_LIB_OBJECTS)
ifeq ($(UART_R4F_DRV_LIB_OBJECTS), "")
     echo "Unsupported device"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(UART_R4F_DRV_LIB)  $(UART_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(UART_C674_DRV_LIB) $(UART_C674_DRV_LIB_OBJECTS)
endif
endif

###################################################################################
# Clean the UART Driver Libraries
###################################################################################
uartDrvClean:
	@echo 'Cleaning the UART Driver Library Objects'
	@$(DEL) $(UART_R4F_DRV_LIB_OBJECTS) $(UART_R4F_DRV_LIB)
	@$(DEL) $(UART_C674_DRV_LIB_OBJECTS) $(UART_C674_DRV_LIB)
	@$(DEL) $(UART_R4F_DRV_DEPENDS) $(UART_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(UART_R4F_DRV_DEPENDS)
-include $(UART_C674_DRV_DEPENDS)

