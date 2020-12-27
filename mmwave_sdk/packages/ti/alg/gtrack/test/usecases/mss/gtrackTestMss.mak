###################################################################################
# GTRACK Usecase Unit Test on MSS Makefile
###################################################################################
.PHONY: gtrackTestMss gtrackTestMssClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/usecases/mss
vpath %.asm test/usecases/mss
vpath %.c test/common

###################################################################################
# The GTRACK Unit test requires additional libraries
###################################################################################
GTRACK_USECASE_MSS_TEST_STD_LIBS = $(R4F_COMMON_STD_LIB)						\
						  -llibgtrack$(MMWAVE_SDK_LIB_BUILD_OPTION).$(R4F_LIB_EXT) \
                          -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)						  
GTRACK_USECASE_MSS_TEST_LOC_LIBS = $(R4F_COMMON_LOC_LIB)						\
						  -ilib \
                          -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib						  

###################################################################################
# Unit Test Files
###################################################################################
GTRACK_USECASE_MSS_TEST_CFG		= test/usecases/mss/mss.cfg
GTRACK_USECASE_MSS_TEST_CMD		= $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
GTRACK_USECASE_MSS_TEST_CONFIGPKG	= test/usecases/mss/mss_configPkg_$(MMWAVE_SDK_DEVICE_TYPE)
GTRACK_USECASE_MSS_TEST_MAP		= test/usecases/mss/$(MMWAVE_SDK_DEVICE_TYPE)_gtrack$(MMWAVE_SDK_LIB_BUILD_OPTION)_usecase_mss.map
GTRACK_USECASE_MSS_TEST_OUT		= test/usecases/mss/$(MMWAVE_SDK_DEVICE_TYPE)_gtrack$(MMWAVE_SDK_LIB_BUILD_OPTION)_usecase_mss.$(R4F_EXE_EXT)
GTRACK_USECASE_MSS_TEST_BIN		= test/usecases/mss/$(MMWAVE_SDK_DEVICE_TYPE)_gtrack$(MMWAVE_SDK_LIB_BUILD_OPTION)_usecase_mss.bin
GTRACK_USECASE_MSS_TEST_APP_CMD	= test/usecases/mss/mss_gtrack_linker.cmd
GTRACK_USECASE_MSS_TEST_SOURCES	= main_mss.c	\
							  gtrackApp.c		\
							  gtrackAlloc.c		\
							  gtrackLog.c
							  
# Get the list of C and assembly files
GTRACK_USECASE_MSS_TEST_C_SOURCES   = $(filter %.c,   $(GTRACK_USECASE_MSS_TEST_SOURCES))
GTRACK_USECASE_MSS_TEST_ASM_SOURCES = $(filter %.asm, $(GTRACK_USECASE_MSS_TEST_SOURCES))
							  
GTRACK_USECASE_MSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_USECASE_MSS_TEST_C_SOURCES:.c=.$(R4F_DEP_EXT)))
GTRACK_USECASE_MSS_TEST_DEPENDS 	 += $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_USECASE_MSS_TEST_ASM_SOURCES:.asm=.$(R4F_DEP_EXT)))

GTRACK_USECASE_MSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_USECASE_MSS_TEST_C_SOURCES:.c=.$(R4F_OBJ_EXT)))
GTRACK_USECASE_MSS_TEST_OBJECTS  	+= $(addprefix $(PLATFORM_OBJDIR)/, $(GTRACK_USECASE_MSS_TEST_ASM_SOURCES:.asm=.o))

###################################################################################
# RTSC Configuration:
###################################################################################
gtrackTestMssRTSC:
	@echo 'Configuring RTSC packages...'
	$(XS) --xdcpath="$(XDCPATH)" xdc.tools.configuro $(R4F_XSFLAGS) -o $(GTRACK_USECASE_MSS_TEST_CONFIGPKG) $(GTRACK_USECASE_MSS_TEST_CFG)
	@echo 'Finished configuring packages'
	@echo ' '

###################################################################################
# Build Unit Test for 2D Tracker:
###################################################################################
gtrackTestMss: R4F_CFLAGS += --define=GTRACK_$(MMWAVE_SDK_LIB_BUILD_OPTION)
gtrackTestMss: BUILD_CONFIGPKG=$(GTRACK_USECASE_MSS_TEST_CONFIGPKG)
gtrackTestMss: R4F_CFLAGS += --cmd_file=$(BUILD_CONFIGPKG)/compiler.opt
gtrackTestMss: buildDirectories gtrackTestMssRTSC $(GTRACK_USECASE_MSS_TEST_OBJECTS)
	$(R4F_LD) $(R4F_LDFLAGS) $(GTRACK_USECASE_MSS_TEST_LOC_LIBS) $(GTRACK_USECASE_MSS_TEST_STD_LIBS) 	\
	-l$(GTRACK_USECASE_MSS_TEST_CONFIGPKG)/linker.cmd --map_file=$(GTRACK_USECASE_MSS_TEST_MAP) 		\
	$(GTRACK_USECASE_MSS_TEST_OBJECTS) $(PLATFORM_R4F_LINK_CMD) $(GTRACK_USECASE_MSS_TEST_APP_CMD) 	\
	$(R4F_LD_RTS_FLAGS) -o $(GTRACK_USECASE_MSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the GTRACK $(MMWAVE_SDK_LIB_BUILD_OPTION) MSS Unit Test '
	@echo '******************************************************************************'


###################################################################################
# Cleanup Unit Test:
###################################################################################
gtrackTestMssClean:
	@echo 'Cleaning the GTRACK $(MMWAVE_SDK_LIB_BUILD_OPTION) MSS Unit Test objects'
	@$(DEL) $(GTRACK_USECASE_MSS_TEST_OBJECTS) $(GTRACK_USECASE_MSS_TEST_OUT) $(GTRACK_USECASE_MSS_TEST_BIN)
	@$(DEL) $(GTRACK_USECASE_MSS_TEST_MAP) $(GTRACK_USECASE_MSS_TEST_DEPENDS)
	@echo 'Cleaning the GTRACK $(MMWAVE_SDK_LIB_BUILD_OPTION) MSS Unit RTSC package'
	@$(DEL) $(GTRACK_USECASE_MSS_TEST_CONFIGPKG)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(GTRACK_USECASE_MSS_TEST_DEPENDS)

