###################################################################################
# mmWave SDK common makefile
###################################################################################

###################################################################################
# Tool Definitions:
###################################################################################
XDC      = $(XDC_INSTALL_PATH)/xdc
XS       = $(XDC_INSTALL_PATH)/xs
C674_CC  = $(C674_CODEGEN_INSTALL_PATH)/bin/cl6x
C674_AR  = $(C674_CODEGEN_INSTALL_PATH)/bin/ar6x
C674_LD  = $(C674_CODEGEN_INSTALL_PATH)/bin/cl6x
R4F_CC   = $(R4F_CODEGEN_INSTALL_PATH)/bin/armcl
R4F_LD   = $(R4F_CODEGEN_INSTALL_PATH)/bin/armcl
R4F_AR   = $(R4F_CODEGEN_INSTALL_PATH)/bin/armar

###################################################################################
# XDCPATH:
###################################################################################
export XDCPATH=$(BIOS_INSTALL_PATH);$(MMWAVE_SDK_INSTALL_PATH)

###################################################################################
# Platform specific definitions:
# Legend:
# MMWAVE_SDK_DEVICE     : awr14xx, awr16xx, awr18xx, iwr14xx, iwr16xx, iwr18xx, iwr68xx
# MMWAVE_SDK_DEVICE_TYPE: xwr14xx (common for awr14xx, iwr14xx),
#                         xwr16xx (common for awr16xx, iwr16xx),
#                         xwr18xx (common for awr18xx, iwr18xx),
#                         xwr68xx (common for          iwr68xx)
# PLATFORM_DEFINE       : SOC_XWR14XX, SOC_XWR16XX, SOC_XWR18XX, SOC_XWR68XX
# XDC_PLATFORM_DEFINE   : AWR14XX, AWR16XX, AWR18XX, IWR14XX, IWR16XX, IWR18XX, IWR68XX
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE), awr14xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr14xx.mak
else ifeq ($(MMWAVE_SDK_DEVICE), awr16xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr16xx.mak
else ifeq ($(MMWAVE_SDK_DEVICE), awr18xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr18xx.mak
else ifeq ($(MMWAVE_SDK_DEVICE), iwr14xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr14xx.mak
else ifeq ($(MMWAVE_SDK_DEVICE), iwr16xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr16xx.mak
else ifeq ($(MMWAVE_SDK_DEVICE), iwr18xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr18xx.mak
else ifeq ($(MMWAVE_SDK_DEVICE), iwr68xx)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_xwr68xx.mak
else
$(error Please set the environment variable MMWAVE_SDK_DEVICE.)
endif
$(info SHMEM_ALLOC is set to $(SHMEM_ALLOC))

###################################################################################
# OS specific definitions:
###################################################################################
ifeq ($(OS),Windows_NT)
GENERATE_BIN = @$(MMWAVE_SDK_INSTALL_PATH)/scripts/windows/generateBin.bat
GENERATE_METAIMAGE = @$(MMWAVE_SDK_INSTALL_PATH)/scripts/windows/generateMetaImage.bat
GENERATE_HS_METAIMAGE = @$(MMWAVE_SDK_INSTALL_PATH)/scripts/windows/generateHSMetaImage.bat
DEL      = $(XDC_INSTALL_PATH)/bin/rm -Rf
DOXYGEN  = doxygen
COPY_CMD = $(XDC_INSTALL_PATH)/bin/cp -f
MKDIR    = $(XDC_INSTALL_PATH)/bin/mkdir
MAKE     = gmake
else
GENERATE_BIN = @$(MMWAVE_SDK_INSTALL_PATH)/scripts/unix/generateBin.sh
GENERATE_METAIMAGE = @$(MMWAVE_SDK_INSTALL_PATH)/scripts/unix/generateMetaImage.sh
GENERATE_HS_METAIMAGE = @$(MMWAVE_SDK_INSTALL_PATH)/scripts/unix/generateHSMetaImage.sh
DEL      = rm -Rf
DOXYGEN  = doxygen
COPY_CMD = cp -f
MKDIR    = mkdir
MAKE     = make
endif

###################################################################################
# R4F Tools Flag definitions:
###################################################################################

# Device specific R4 flags
#   R4F_XSFLAGS_ENUM_TYPE
#   R4F_CFLAGS_ENUM_TYPE
#     These two flags are used in conjunction to enforce that enum types are compiled as 32bit integers. 
#     For devices where DSP and ARM coexist such as xwr16xx, they must be set to ensure 
#     that enum types are consistent between ARM and DSP. 
#     For devices where DSP is not present (xwr14xx), it is not necessary
#     to force enum types to be 32bit integers and if forced, it may increase
#     program memory usage.

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), xwr16xx xwr18xx xwr68xx),)
R4F_XSFLAGS_ENUM_TYPE = --compileOptions "--enum_type=int" 
R4F_CFLAGS_ENUM_TYPE  = --enum_type=int
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), xwr14xx)
R4F_XSFLAGS_ENUM_TYPE = 
R4F_CFLAGS_ENUM_TYPE  =
else
$(error please set the device specific R4 flags.)
endif

# XS Flags which are used to build the Unit Test Projects and demos:
# Use the platform define from TI RTOS but do not use the default linker command file (false)
# and set the CPU freq to 200.
R4F_XSFLAGS = -t ti.targets.arm.elf.R4Ft -p ti.platforms.cortexR:$(XDC_PLATFORM_DEFINE):false:200   \
              -r release -c $(R4F_CODEGEN_INSTALL_PATH) -DMMWAVE_SDK_DEVICE_TYPE=$(MMWAVE_SDK_DEVICE_TYPE) \
              $(R4F_XSFLAGS_ENUM_TYPE)

# Setup the Include paths for the R4 Builds:
R4F_INCLUDE = -i$(MMWAVE_SDK_INSTALL_PATH) -i$(R4F_CODEGEN_INSTALL_PATH)/include $(STD_INCL)

# Compiler flags used for the R4 Builds:
R4F_CFLAGS  = -mv7R4 --code_state=16 --float_support=VFPv3D16 --abi=eabi -me            \
              --define=SUBSYS_MSS --define=$(PLATFORM_DEFINE)                           \
              --define=_LITTLE_ENDIAN --define=DebugP_ASSERT_ENABLED $(R4F_INCLUDE) 	\
              -g -O3 -display_error_number --diag_warning=225 --diag_wrap=off 			\
              --little_endian --preproc_with_compile --gen_func_subsections 			\
			  --emit_warnings_as_errors $(R4F_CFLAGS_ENUM_TYPE)

ifeq ($(DOWNLOAD_FROM_CCS), yes)
R4F_CFLAGS += --define=DOWNLOAD_FROM_CCS
endif

# Linker flags used for the R4 Builds:
R4F_LDFLAGS = -mv7R4 --code_state=16 --float_support=VFPv3D16 --abi=eabi -me -g         \
              --display_error_number --diag_warning=225 --diag_wrap=off -z              \
              --reread_libs --warn_sections --rom_model --unused_section_elimination    \
              -i$(R4F_CODEGEN_INSTALL_PATH)/lib --reread_libs --emit_warnings_as_errors

# Add Compiler/Linker Flags for L3RAM size
# Add Compiler/Linker Flags for L3RAM number of banks
ifneq ($(MMWAVE_L3RAM_NUM_BANK), '')
R4F_CFLAGS += --define=MMWAVE_L3RAM_NUM_BANK=$(MMWAVE_L3RAM_NUM_BANK)
R4F_LDFLAGS += --define=MMWAVE_L3RAM_NUM_BANK=$(MMWAVE_L3RAM_NUM_BANK)
endif

# Add Compiler/Linker Flags for shared memory TCMA number of banks
ifneq ($(MMWAVE_SHMEM_TCMA_NUM_BANK), '')
R4F_CFLAGS += --define=MMWAVE_SHMEM_TCMA_NUM_BANK=$(MMWAVE_SHMEM_TCMA_NUM_BANK)
R4F_LDFLAGS += --define=MMWAVE_SHMEM_TCMA_NUM_BANK=$(MMWAVE_SHMEM_TCMA_NUM_BANK)
endif

# Add Compiler/Linker Flags for shared memory TCMB number of banks
ifneq ($(MMWAVE_SHMEM_TCMB_NUM_BANK), '')
R4F_CFLAGS += --define=MMWAVE_SHMEM_TCMB_NUM_BANK=$(MMWAVE_SHMEM_TCMB_NUM_BANK)
R4F_LDFLAGS += --define=MMWAVE_SHMEM_TCMB_NUM_BANK=$(MMWAVE_SHMEM_TCMB_NUM_BANK)
endif

# Add Compiler/Linker Flags for shared memory bank size
ifneq ($(MMWAVE_SHMEM_BANK_SIZE), '')
R4F_CFLAGS += --define=MMWAVE_SHMEM_BANK_SIZE=$(MMWAVE_SHMEM_BANK_SIZE)
R4F_LDFLAGS += --define=MMWAVE_SHMEM_BANK_SIZE=$(MMWAVE_SHMEM_BANK_SIZE)
endif

# all options used to enable build warnings as errors
EMIT_WARNINGS_AS_ERRORS_OPTIONS := --emit_warnings_as_errors -pdew

# if MMWAVE_DISABLE_WARNINGS_AS_ERRORS is 1 then remove the emit_warnings_as_errors flag
ifneq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)", "")
    ifeq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)","1")
        R4F_CFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(R4F_CFLAGS))
        R4F_LDFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(R4F_LDFLAGS))
    endif
endif

# Archiver options:
R4F_AR_OPTS = rs

# Linker flag for RTS
# NOTE: this should be the last linker flag in the linker command line, just before the -o option.
R4F_LD_RTS_FLAGS = -l"rtsv7R4_T_le_v3D16_eabi.lib"

# File extension to use for R4F library file
R4F_LIB_EXT = aer4f

# File extension to use for R4F Objects
R4F_OBJ_EXT = oer4f

# File extension to use for R4F Executable
R4F_EXE_EXT = xer4f

# File extension to use for R4F Dependency
R4F_DEP_EXT = d

# File suffix to use for xs files
R4F_XS_SUFFIX = per4ft


# This is a list of all the minimum common libraries which are required on the R4 to build
# any application. NOTE: Applications should add any other libraries which are required
R4F_COMMON_STD_LIB = -llibosal_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)     \
                     -llibesm_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)      \
                     -llibsoc_$(MMWAVE_SDK_DEVICE_TYPE).$(R4F_LIB_EXT)
R4F_COMMON_LOC_LIB = -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/osal/lib       \
                     -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/esm/lib        \
                     -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/soc/lib

###################################################################################
# C674 Tools Flag definitions:
###################################################################################

# XS Flags which are used to build the Unit Test Projects
C674_XSFLAGS = -t ti.targets.elf.C674 -p ti.platforms.c6x:$(XDC_PLATFORM_DEFINE):false:600                  \
               -DMMWAVE_SDK_DEVICE=$(MMWAVE_SDK_DEVICE) -DMMWAVE_SDK_DEVICE_TYPE=$(MMWAVE_SDK_DEVICE_TYPE)  \
               -r release -c $(C674_CODEGEN_INSTALL_PATH)

# Setup the Include paths for C674 Builds:
C674_INCLUDE = -i$(MMWAVE_SDK_INSTALL_PATH) -i$(C674_CODEGEN_INSTALL_PATH)/include $(STD_INCL)

# Compiler Flags for C674 Builds:
C674_CFLAGS  = -mv6740 --abi=eabi --gcc -g -O3 -ms0 -mo --define=SUBSYS_DSS					\
               --define=$(PLATFORM_DEFINE) --define=_LITTLE_ENDIAN --display_error_number   \
               --define=DebugP_ASSERT_ENABLED --diag_warning=225 --diag_wrap=off 			\
               --preproc_with_compile $(C674_INCLUDE) --emit_warnings_as_errors

# Linker Flags for C674 Builds:
C674_LDFLAGS = -mv6740 --abi=eabi -g --define=$(PLATFORM_DEFINE) --display_error_number           \
               --diag_warning=225 --diag_wrap=off -z --reread_libs --warn_sections --ram_model    \
               -i$(C674_CODEGEN_INSTALL_PATH)/lib --emit_warnings_as_errors

# Treat "warning #10015-D: output file xyz cannot be loaded and run on a target system" as error
C674_LDFLAGS += --diag_error=10015

# Add Compiler/Linker Flags for L3RAM number of banks
ifneq ($(MMWAVE_L3RAM_NUM_BANK), '')
C674_CFLAGS += --define=MMWAVE_L3RAM_NUM_BANK=$(MMWAVE_L3RAM_NUM_BANK)
C674_LDFLAGS += --define=MMWAVE_L3RAM_NUM_BANK=$(MMWAVE_L3RAM_NUM_BANK)
endif

# Add Compiler/Linker Flags for shared memory bank size
ifneq ($(MMWAVE_SHMEM_BANK_SIZE), '')
C674_CFLAGS += --define=MMWAVE_SHMEM_BANK_SIZE=$(MMWAVE_SHMEM_BANK_SIZE)
C674_LDFLAGS += --define=MMWAVE_SHMEM_BANK_SIZE=$(MMWAVE_SHMEM_BANK_SIZE)
endif

# if MMWAVE_DISABLE_WARNINGS_AS_ERRORS is 1 then remove the emit_warnings_as_errors flag
ifneq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)", "")
    ifeq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)","1")
        C674_CFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(C674_CFLAGS))
        C674_LDFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(C674_LDFLAGS))
    endif
endif

# Archiver options:
C674_AR_OPTS = rs

# Linker flag for RTS
# NOTE: this should be the last linker flag in the linker command line, just before the -o option.
C674_LD_RTS_FLAGS = -lrts6740_elf.lib

# File extension to use for C674 library file
C674_LIB_EXT = ae674

# File extension to use for C674 Objects
C674_OBJ_EXT = oe674

# File extension to use for C674 Executable
C674_EXE_EXT = xe674

# File extension to use for C674 Dependency
C674_DEP_EXT = pp

# File suffix to use for xs files
C674_XS_SUFFIX = pe674


# This is a list of all the minimum common libraries which are required on the C674 to build
# any application. NOTE: Applications should add any other libraries which are required
C674_COMMON_STD_LIB = -llibosal_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)   \
                      -llibsoc_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)
C674_COMMON_LOC_LIB = -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/osal/lib      \
                      -i$(MMWAVE_SDK_INSTALL_PATH)/ti/drivers/soc/lib

###################################################################################
# Build the object directory
###################################################################################
buildDirectories:
	@$(MKDIR) -p $(PLATFORM_OBJDIR)

###################################################################################
# Build Suffix:
###################################################################################
$(PLATFORM_OBJDIR)/%.$(R4F_OBJ_EXT): %.c
	@echo '[R4 Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(R4F_CC) -c $(R4F_CFLAGS) -ppd=$(PLATFORM_OBJDIR)/"$(basename $(<F)).$(R4F_DEP_EXT)"  $< --output_file $@

$(PLATFORM_OBJDIR)/%.o: %.asm
	@echo '[R4F Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(R4F_CC) -c $(R4F_CFLAGS)  --asm_dependency="$<.d"  $< --output_file $@
	@$(R4F_CC) -c $(R4F_CFLAGS) $< --output_file $@

$(PLATFORM_OBJDIR)/%.$(C674_OBJ_EXT): %.c
	@echo '[C674 Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(C674_CC) -c $(C674_CFLAGS) -ppd=$(PLATFORM_OBJDIR)/"$(basename $(<F)).$(C674_DEP_EXT)" "$<" --output_file $@
