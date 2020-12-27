################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../dss.cfg 

CMD_SRCS += \
../c674x_linker.cmd \
../dss_mrr_linker.cmd 

C_SRCS += \
../Extended_Kalman_Filter_xyz.c \
../clusteringDBscan.c \
../dss_config_edma_util.c \
../dss_data_path.c \
../dss_main.c \
../gen_twiddle_fft16x16.c \
../gen_twiddle_fft32x32.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./Extended_Kalman_Filter_xyz.d \
./clusteringDBscan.d \
./dss_config_edma_util.d \
./dss_data_path.d \
./dss_main.d \
./gen_twiddle_fft16x16.d \
./gen_twiddle_fft32x32.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./Extended_Kalman_Filter_xyz.oe674 \
./clusteringDBscan.oe674 \
./dss_config_edma_util.oe674 \
./dss_data_path.oe674 \
./dss_main.oe674 \
./gen_twiddle_fft16x16.oe674 \
./gen_twiddle_fft32x32.oe674 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"Extended_Kalman_Filter_xyz.oe674" \
"clusteringDBscan.oe674" \
"dss_config_edma_util.oe674" \
"dss_data_path.oe674" \
"dss_main.oe674" \
"gen_twiddle_fft16x16.oe674" \
"gen_twiddle_fft32x32.oe674" 

C_DEPS__QUOTED += \
"Extended_Kalman_Filter_xyz.d" \
"clusteringDBscan.d" \
"dss_config_edma_util.d" \
"dss_data_path.d" \
"dss_main.d" \
"gen_twiddle_fft16x16.d" \
"gen_twiddle_fft32x32.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../Extended_Kalman_Filter_xyz.c" \
"../clusteringDBscan.c" \
"../dss_config_edma_util.c" \
"../dss_data_path.c" \
"../dss_main.c" \
"../gen_twiddle_fft16x16.c" \
"../gen_twiddle_fft32x32.c" 


