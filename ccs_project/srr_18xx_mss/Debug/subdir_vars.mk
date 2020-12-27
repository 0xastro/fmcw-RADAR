################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../mss.cfg 

CMD_SRCS += \
../mss_linker.cmd \
../r4f_linker.cmd 

C_SRCS += \
../mss_cli.c \
../mss_main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg/ 

C_DEPS += \
./mss_cli.d \
./mss_main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./mss_cli.obj \
./mss_main.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

OBJS__QUOTED += \
"mss_cli.obj" \
"mss_main.obj" 

C_DEPS__QUOTED += \
"mss_cli.d" \
"mss_main.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../mss_cli.c" \
"../mss_main.c" 


