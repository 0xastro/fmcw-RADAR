###################################################################################
# Mailbox Driver Makefile
###################################################################################
.PHONY: mailboxDrv mailboxDrvClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Driver Source Files:
###################################################################################
MAILBOX_DRV_SOURCES = mailbox.c

###################################################################################
# Driver Source Files:
# - XWR14xx:
#   Mailbox driver is available for R4F
# - XWR16xx/XWR18xx/XWR68xx:
#   Mailbox driver is available for both the DSP and R4F
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
MAILBOX_DRV_SOURCES         += mailbox_xwr14xx.c
MAILBOX_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(MAILBOX_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
MAILBOX_C674_DRV_LIB_OBJECTS =
else ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
MAILBOX_DRV_SOURCES         += mailbox_$(MMWAVE_SDK_DEVICE_TYPE).c
MAILBOX_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(MAILBOX_DRV_SOURCES:.c=.$(R4F_OBJ_EXT)))
MAILBOX_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(MAILBOX_DRV_SOURCES:.c=.$(C674_OBJ_EXT)))
else
echo "Unsupported device!"
endif

###################################################################################
# Driver Dependency:
###################################################################################
MAILBOX_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_DRV_SOURCES:.c=.$(R4F_DEP_EXT)))
MAILBOX_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(MAILBOX_DRV_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
MAILBOX_R4F_DRV_LIB  = lib/libmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
MAILBOX_C674_DRV_LIB = lib/libmailbox_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# Mailbox Driver Build:
# - XWR14xx: Build the R4F Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4F & DSP Library
###################################################################################
mailboxDrv: buildDirectories $(MAILBOX_R4F_DRV_LIB_OBJECTS) $(MAILBOX_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(MAILBOX_R4F_DRV_LIB)  $(MAILBOX_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(MAILBOX_C674_DRV_LIB) $(MAILBOX_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the Mailbox Driver Libraries
###################################################################################
mailboxDrvClean:
	@echo 'Cleaning the Mailbox Driver Library Objects'
	@$(DEL) $(MAILBOX_R4F_DRV_LIB_OBJECTS) $(MAILBOX_R4F_DRV_LIB)
	@$(DEL) $(MAILBOX_C674_DRV_LIB_OBJECTS) $(MAILBOX_C674_DRV_LIB)
	@$(DEL) $(MAILBOX_R4F_DRV_DEPENDS)
	@$(DEL) $(MAILBOX_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MAILBOX_R4F_DRV_DEPENDS)
-include $(MAILBOX_C674_DRV_DEPENDS)

