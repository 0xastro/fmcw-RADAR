###################################################################################
# CRYPTO Driver Makefile
###################################################################################
.PHONY: cryptoDrv cryptoDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
CRYPTO_DRV_SOURCES = aes.c aes_crypto.c hmac.c hmac_crypto.c Crypto.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   CRYPTO driver is not available
# - XWR16xx/XWR18xx:
#   CRYPTO driver is available for R4
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx), )
CRYPTO_DRV_SOURCES += crypto_$(MMWAVE_SDK_DEVICE_TYPE).c
CRYPTO_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(CRYPTO_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
CRYPTO_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CRYPTO_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx), )
CRYPTO_R4F_DRV_LIB  = lib/libcrypto_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
endif

###################################################################################
# CRYPTO Driver Build:
# - XWR16xx/XWR18xx: Build the R4 Library
###################################################################################
cryptoDrv: buildDirectories $(CRYPTO_R4F_DRV_LIB_OBJECTS)
ifndef CRYPTO_R4F_DRV_LIB_OBJECTS
	@echo "Error: $@ not supported on $(MMWAVE_SDK_DEVICE)"
else
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(CRYPTO_R4F_DRV_LIB)  $(CRYPTO_R4F_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the CRYPTO Driver Libraries
###################################################################################
cryptoDrvClean:
	@echo 'Cleaning the CRYPTO Driver Library Objects'
ifdef CRYPTO_R4F_DRV_LIB_OBJECTS
	@$(DEL) $(CRYPTO_R4F_DRV_DEPENDS)
	@$(DEL) $(CRYPTO_R4F_DRV_LIB_OBJECTS) $(CRYPTO_R4F_DRV_LIB)
	@$(DEL) $(PLATFORM_OBJDIR)
endif

###################################################################################
# Dependency handling
###################################################################################
-include $(CRYPTO_R4F_DRV_DEPENDS)

