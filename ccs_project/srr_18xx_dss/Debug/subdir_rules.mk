################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.oe674: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-c6000_8.3.4/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/Users/astro/workspace_v9_2/srr_18xx_dss" --include_path="C:/ti/mmwave_sdk_03_01_00_02/packages" --include_path="C:/ti/mathlib_c674x_3_1_2_1/packages" --include_path="C:/ti/dsplib_c64Px_3_4_0_0/packages/ti/dsplib/src/DSP_fft16x16/c64P" --include_path="C:/ti/dsplib_c64Px_3_4_0_0/packages/ti/dsplib/src/DSP_fft32x32/c64P" --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-c6000_8.3.4/include" --define=SOC_XWR18XX --define=SUBSYS_DSS --define=ENABLE_ADVANCED_FRAME --define=MMWAVE_L3RAM_NUM_BANK=8 --define=MMWAVE_SHMEM_BANK_SIZE=0x20000 --define=MMWAVE_L3RAM_SIZE=0x100000 --define=SHMEM_ALLOC=0x00000008 --define=DOWNLOAD_FROM_CCS --define=DebugP_ASSERT_ENABLED -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --disable_push_pop --obj_extension=.oe674 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

clusteringDBscan.oe674: ../clusteringDBscan.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-c6000_8.3.4/bin/cl6x" -mv6740 --abi=eabi -O3 --include_path="C:/Users/astro/workspace_v9_2/srr_18xx_dss" --include_path="C:/ti/mmwave_sdk_03_01_00_02/packages" --include_path="C:/ti/mathlib_c674x_3_1_2_1/packages" --include_path="C:/ti/dsplib_c64Px_3_4_0_0/packages/ti/dsplib/src/DSP_fft16x16/c64P" --include_path="C:/ti/dsplib_c64Px_3_4_0_0/packages/ti/dsplib/src/DSP_fft32x32/c64P" --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-c6000_8.3.4/include" --define=SOC_XWR18XX --define=SUBSYS_DSS --define=ENABLE_ADVANCED_FRAME --define=MMWAVE_L3RAM_NUM_BANK=8 --define=MMWAVE_SHMEM_BANK_SIZE=0x20000 --define=MMWAVE_L3RAM_SIZE=0x100000 --define=SHMEM_ALLOC=0x00000008 --define=DOWNLOAD_FROM_CCS --define=DebugP_ASSERT_ENABLED -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --disable_push_pop --obj_extension=.oe674 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1967277135:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1967277135-inproc

build-1967277135-inproc: ../dss.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_50_08_24_core/xs" --xdcpath="C:/ti/bios_6_73_01_01/packages;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C674 -p ti.platforms.c6x:AWR16XX:false:600 -r release -c "C:/ti/ccs920/ccs/tools/compiler/ti-cgt-c6000_8.3.4" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1967277135 ../dss.cfg
configPkg/compiler.opt: build-1967277135
configPkg/: build-1967277135


