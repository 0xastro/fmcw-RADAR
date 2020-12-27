################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS

GEN_OPTS__FLAG := --cmd_file="configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"configPkg/linker.cmd"

ORDERED_OBJS += \
"./mss_cli.obj" \
"./mss_main.obj" \
"./common/frame_cfg.obj" \
"../mss_linker.cmd" \
"../r4f_linker.cmd" \
$(GEN_CMDS__FLAG) \
-llibosal_xwr18xx.aer4f \
-llibesm_xwr18xx.aer4f \
-llibgpio_xwr18xx.aer4f \
-llibsoc_xwr18xx.aer4f \
-llibmailbox_xwr18xx.aer4f \
-llibpinmux_xwr18xx.aer4f \
-llibcrc_xwr18xx.aer4f \
-llibDMA_xwr18xx.aer4f \
-llibuart_xwr18xx.aer4f \
-llibcanfd_xwr18xx.aer4f \
-llibmmwavelink_xwr18xx.aer4f \
-llibmmwave_xwr18xx.aer4f \
-llibcli_xwr18xx.aer4f \
-lrtsv7R4_T_le_v3D16_eabi.lib \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include common/subdir_vars.mk
-include subdir_rules.mk
-include common/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
srr_18xx_mss.xer4f \

EXE_OUTPUTS__QUOTED += \
"srr_18xx_mss.xer4f" \

BIN_OUTPUTS += \
srr_18xx_mss.hex \

BIN_OUTPUTS__QUOTED += \
"srr_18xx_mss.hex" \


# All Target
all:
	@$(MAKE) --no-print-directory pre-build
	@$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "srr_18xx_mss.xer4f"

# Tool invocations
srr_18xx_mss.xer4f: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --define=_LITTLE_ENDIAN --define=MMWAVE_L3RAM_NUM_BANK=8 --define=MMWAVE_SHMEM_TCMA_NUM_BANK=0 --define=MMWAVE_SHMEM_TCMB_NUM_BANK=0 --define=MMWAVE_SHMEM_BANK_SIZE=0x20000 --define=SOC_XWR18XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=SHMEM_ALLOC=0x00000008 --define=DebugP_ASSERT_ENABLED -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=int --abi=eabi -z -m"srr_18xx_mss.map" --heap_size=0x800 --stack_size=0x800 -i"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/control/mmwave/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/control/mmwavelink/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/crc/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/esm/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/gpio/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/osal/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/pinmux/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/mailbox/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/dma/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/soc/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/uart/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/drivers/canfd/lib" -i"C:/ti/mmwave_sdk_03_01_00_02/packages/ti/utils/cli/lib" -i"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --reread_libs --disable_auto_rts --define=MMWAVE_L3RAM_SIZE=0x100000 --diag_warning=225 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="srr_18xx_mss.xml" --rom_model --unused_section_elimination=on -o "srr_18xx_mss.xer4f" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

srr_18xx_mss.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armhex"  -o "srr_18xx_mss.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "mss_cli.obj" "mss_main.obj" "common\frame_cfg.obj" 
	-$(RM) "mss_cli.d" "mss_main.d" "common\frame_cfg.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

pre-build:
	-rm -f C:/Users/astro/workspace_v9_2/srr_18xx_mss/srr_18xx_mss.bin
	-@echo ' '

post-build:
	-C:/ti/mmwave_sdk_03_01_00_02/packages/scripts/ImageCreator/out2rprc/out2rprc.exe srr_18xx_mss.xer4f srr_18xx_mss.bin
	-C:/ti/mmwave_sdk_03_01_00_02/packages/scripts/ImageCreator/multicore_image_generator/MulticoreImageGen.exe LE 37 0x00000008 xwr18xx_srr.bin 0x35510000 C:/Users/astro/workspace_v9_2/srr_18xx_mss/Debug/srr_18xx_mss.bin 0xb5510000 C:/ti/mmwave_sdk_03_01_00_02/firmware/radarss/xwr18xx_radarss_rprc.bin 0xd5510000 C:/Users/astro/workspace_v9_2/mrr_18xx_dss/Debug/mrr_18xx_dss.bin
	-C:/ti/mmwave_sdk_03_01_00_02/packages/scripts/ImageCreator/crc_multicore_image/crc_multicore_image.exe xwr18xx_srr.bin xwr18xx_srr.bin.tmp
	-C:/ti/mmwave_sdk_03_01_00_02/packages/scripts/ImageCreator/append_bin_crc/gen_bincrc32.exe xwr18xx_srr.bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

