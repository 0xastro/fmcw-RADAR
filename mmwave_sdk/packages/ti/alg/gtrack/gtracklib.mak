###################################################################################
# GTRACK Library Makefile
###################################################################################
.PHONY: gtracklib gtracklibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# GTRACK Library Source Files:
###################################################################################
GTRACK_LIB_SOURCES = gtrack_create.c			\
					 gtrack_delete.c			\
					 gtrack_step.c				\
					 gtrack_module.c			\
					 gtrack_unit_create.c		\
					 gtrack_unit_delete.c		\
					 gtrack_unit_get.c			\
					 gtrack_unit_event.c		\
					 gtrack_unit_predict.c		\
					 gtrack_unit_report.c		\
					 gtrack_unit_score.c		\
					 gtrack_unit_start.c		\
					 gtrack_unit_stop.c			\
					 gtrack_unit_update.c		\
					 gtrack_utilities.c			\
					 gtrack_utilities_2d.c		\
					 gtrack_utilities_3d.c		\
					 gtrack_math.c				\
					 gtrack_listlib.c 			

###################################################################################
# Enabling Debug Support
###################################################################################
R4F_CFLAGS  += --define=GTRACK_LOG_ENABLED --define=GTRACK_ASSERT_ENABLED
C674_CFLAGS  += --define=GTRACK_LOG_ENABLED --define=GTRACK_ASSERT_ENABLED

###################################################################################
# GTRACK Library Source Files:
# - XWR14xx
#   GTRACK Library is available for the R4
# - for all other platforms:
#   GTRACK Library is available for the R4 and DSP
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),xwr14xx)
GTRACK_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
GTRACK_C674_DRV_LIB_OBJECTS =
else
GTRACK_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
GTRACK_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))
endif

###################################################################################
# Library Dependency:
###################################################################################
GTRACK_R4F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_LIB_SOURCES:.c=.$(R4F_DEP_EXT)))
GTRACK_C674_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_LIB_SOURCES:.c=.$(C674_DEP_EXT)))

###################################################################################
# GTRACK Library Names:
###################################################################################
GTRACK_R4F_DRV_LIB  = lib/libgtrack$(MMWAVE_SDK_LIB_BUILD_OPTION).$(R4F_LIB_EXT)
GTRACK_C674_DRV_LIB = lib/libgtrack$(MMWAVE_SDK_LIB_BUILD_OPTION).$(C674_LIB_EXT)

R4F_CFLAGS += -i../ -i ../include

###################################################################################
# GTRACK Library Build:
# - XWR14xx: Build the R4 Library
# - XWR16xx/XWR18xx/XWR68xx: Build the R4 & DSP Library
###################################################################################
gtracklib: R4F_CFLAGS  += --define=GTRACK_$(MMWAVE_SDK_LIB_BUILD_OPTION)
gtracklib: C674_CFLAGS  += --define=GTRACK_$(MMWAVE_SDK_LIB_BUILD_OPTION)
gtracklib: buildDirectories $(GTRACK_R4F_DRV_LIB_OBJECTS) $(GTRACK_C674_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R4F_AR) $(R4F_AR_OPTS) $(GTRACK_R4F_DRV_LIB) $(GTRACK_R4F_DRV_LIB_OBJECTS)
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),xwr16xx xwr18xx xwr68xx), )
	$(C674_AR) $(C674_AR_OPTS) $(GTRACK_C674_DRV_LIB) $(GTRACK_C674_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the GTRACK Libraries
###################################################################################
gtracklibClean:
	@echo 'Cleaning the GTRACK Library Objects'
	@$(DEL) $(GTRACK_R4F_DRV_LIB_OBJECTS) $(GTRACK_R4F_DRV_LIB)
	@$(DEL) $(GTRACK_C674_DRV_LIB_OBJECTS) $(GTRACK_C674_DRV_LIB)
	@$(DEL) $(GTRACK_R4F_DRV_DEPENDS) $(GTRACK_C674_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(GTRACK_R4F_DRV_DEPENDS)
-include $(GTRACK_C674_DRV_DEPENDS)

