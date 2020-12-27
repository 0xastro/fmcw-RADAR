###################################################################################
#   mmwavelib Makefile
#
#  
###################################################################################
.PHONY: mmwavelib mmwavelibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src/fft
vpath %.c src/detection
vpath %.c src/vecutils
vpath %.c src/matutils

##########################################################
# Source Files:
##########################################################
MMWAVELIB_SOURCES = mmwavelib_windowing.c	\
					mmwavelib_fft16pt.c		\
					mmwavelib_cfarca.c	    \
					mmwavelib_cfarca_sogo.c	\
					mmwavelib_cfaros.c	    \
					mmwavelib_log2abs.c	    \
					mmwavelib_accum.c   \
					mmwavelib_vecsum.c      \
					mmwavelib_vecsubc.c     \
					mmwavelib_vecsub.c     \
					mmwavelib_shift.c       \
					mmwavelib_histogram.c   \
					mmwavelib_vecsumabs.c   \
					mmwavelib_vecmul.c      \
					mmwavelib_maxpow.c      \
					mmwavelib_power.c       \
					mmwavelib_multipeak.c   \
					mmwavelib_secondpeak.c  \
					mmwavelib_dftsinglebin.c \
					mmwavelib_clustering.c \
					mmwavelib_dcremoval_float.c \
					mmwavelib_windowing_float.c \
					mmwavelib_poweracc_float.c  \
					mmwavelib_cfarcaall_float.c \
					mmwavelib_aoaest_float.c \
					mmwavelib_peakpruning.c   \
					mmwavelib_ffttwiddle_gen.c \
					mmwavelib_windowcoef_gen.c \
					mmwavelib_mattrans.c


# Compiler Flags for DSP Builds:
C674_CFLAGS  += -o3 -k --asm_directory=$(PLATFORM_OBJDIR)
OPTIMIZE_FOR_SIZE_OPTIONS := --opt_for_space=0 --opt_for_space=1 --opt_for_space=2 \
                             --opt_for_space=3 -ms0 -ms1 -ms2 -ms3
C674_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SIZE_OPTIONS),$(C674_CFLAGS))

##########################################################
# lib Objects:
##########################################################
MMWAVELIB_C674_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_SOURCES:.c=.$(C674_OBJ_EXT)))

##########################################################
#lib Dependency:
##########################################################
MMWAVELIB_C674_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_SOURCES:.c=.$(C674_DEP_EXT)))

##########################################################
# Library Name:
##########################################################
MMWAVELIB_C674_LIB = lib/libmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C674_LIB_EXT)

##########################################################
# mmwavelib Library Build:
# - XWR16xx/XWR18xx/XWR68xx: Build the DSP Library
##########################################################
mmwavelib: buildDirectories $(MMWAVELIB_C674_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(C674_AR) $(C674_AR_OPTS) $(MMWAVELIB_C674_LIB) $(MMWAVELIB_C674_LIB_OBJECTS)

##########################################################
# Clean the mmwavelib Library
##########################################################
mmwavelibClean:
	@echo 'Cleaning the mmwavelib Library Objects'
	@$(DEL) $(MMWAVELIB_C674_LIB_OBJECTS) $(MMWAVELIB_C674_LIB)
	@$(DEL) $(MMWAVELIB_C674_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

##########################################################
# Dependency handling
##########################################################
-include $(MMWAVELIB_C674_DEPENDS)

