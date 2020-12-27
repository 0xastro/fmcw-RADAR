###################################################################################
# Test Logger Library Makefile
###################################################################################
.PHONY: testLoggerLib testLoggerLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Source Files:
###################################################################################
TEST_LOGGER_SOURCES = logger.c

###################################################################################
# Additional include paths required
###################################################################################
STD_INCL = -i$(XDC_INSTALL_PATH)/packages -i$(BIOS_INSTALL_PATH)

###################################################################################
# Driver Objects:
# - XWR14xx: Build only for R4
# - XWR16xx/XWR18xx/XWR68xx: Build R4 and DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
TEST_LOGGER_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R4F_OBJ_EXT)))
TEST_LOGGER_C674_LIB_OBJECTS =
else
TEST_LOGGER_R4F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R4F_OBJ_EXT)))
TEST_LOGGER_C674_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Driver Dependency:
###################################################################################
TEST_LOGGER_R4F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R4F_DEP_EXT)))
TEST_LOGGER_C674_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
TEST_LOGGER_R4F_DRV_LIB  = lib/libtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
TEST_LOGGER_C674_DRV_LIB = lib/libtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

###################################################################################
# mmWave Library:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
testLoggerLib: buildDirectories $(TEST_LOGGER_R4F_LIB_OBJECTS) $(TEST_LOGGER_C674_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(TEST_LOGGER_R4F_DRV_LIB) $(TEST_LOGGER_R4F_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(TEST_LOGGER_C674_DRV_LIB) $(TEST_LOGGER_C674_LIB_OBJECTS)
endif

###################################################################################
# Clean the mmWave Library
###################################################################################
testLoggerLibClean:
	@echo 'Cleaning the Test Logger Library Objects'
	$(DEL) $(TEST_LOGGER_R4F_LIB_OBJECTS) $(TEST_LOGGER_R4F_DRV_LIB)
	$(DEL) $(TEST_LOGGER_C674_LIB_OBJECTS) $(TEST_LOGGER_C674_DRV_LIB)
	$(DEL) $(TEST_LOGGER_R4F_DEPENDS) $(TEST_LOGGER_C674_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(TEST_LOGGER_R4F_DEPENDS)
-include $(TEST_LOGGER_C674_DEPENDS)

