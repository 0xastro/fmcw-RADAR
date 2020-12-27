###################################################################################
# I2C Driver Makefile
###################################################################################
.PHONY: i2cDrv i2cDrvClean i2cSlaveDrv i2cSlaveDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
I2C_DRV_SOURCES = I2C.c i2c_master.c i2c_reg.c
I2CSLAVE_DRV_SOURCES = I2CSlave.c i2c_slave.c i2c_reg.c

###################################################################################
# Driver Source Files:
# - XWR14xx/XWR16xx/XWR18xx/XWR68xx:
#   I2C driver is available for R4
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr14xx xwr16xx xwr18xx xwr68xx), )
I2C_DRV_SOURCES += i2c_$(MMWAVE_SDK_DEVICE_TYPE).c
I2CSLAVE_DRV_SOURCES += i2c_$(MMWAVE_SDK_DEVICE_TYPE).c
I2C_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(I2C_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
I2CSLAVE_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(I2CSLAVE_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
I2C_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(I2C_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
I2CSLAVE_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(I2CSLAVE_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
I2C_R4F_DRV_LIB  = lib/libi2c_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
I2CSLAVE_R4F_DRV_LIB  = lib/libi2cSlave_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)

###################################################################################
# I2C Driver Build:
#   Build the R4 Library
###################################################################################
i2cDrv: buildDirectories $(I2C_R4F_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(I2C_R4F_DRV_LIB)  $(I2C_R4F_DRV_LIB_OBJECTS)

###################################################################################
# I2C Slave Driver Build:
# - XWR14xx, XWR16xx, XWR18xx, XWR68xx: Build the R4 Library
###################################################################################
i2cSlaveDrv: buildDirectories $(I2CSLAVE_R4F_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(I2CSLAVE_R4F_DRV_LIB)  $(I2CSLAVE_R4F_DRV_LIB_OBJECTS)

###################################################################################
# Clean the I2C Driver Libraries
###################################################################################
i2cDrvClean:
	@echo 'Cleaning the I2C Driver Library Objects'
	@$(DEL) $(I2C_R4F_DRV_LIB_OBJECTS) $(I2C_R4F_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Clean the I2C Slave Driver Libraries
###################################################################################
i2cSlaveDrvClean:
	@echo 'Cleaning the I2C Slave Driver Library Objects'
	@$(DEL) $(I2CSLAVE_R4F_DRV_LIB_OBJECTS) $(I2CSLAVE_R4F_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(I2C_R4F_DRV_DEPENDS)
-include $(I2CSLAVE_R4F_DRV_DEPENDS)

