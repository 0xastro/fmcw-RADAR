################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1284321553:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1284321553-inproc

build-1284321553-inproc: ../mss.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_50_08_24_core/xs" --xdcpath="C:/ti/bios_6_73_01_01/packages;//Mac/Home/Downloads/anb0s-eclox-f6d31bd;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.R4F -p ti.platforms.cortexR:AWR16XX:false:200 -r release -c "C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS" --compileOptions "--enum_type=int " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1284321553 ../mss.cfg
configPkg/compiler.opt: build-1284321553
configPkg/: build-1284321553

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/bin/armcl" -mv7R4 --code_state=16 --float_support=VFPv3D16 -me -O3 --include_path="C:/Users/astro/workspace_v9_2/srr_18xx_mss" --include_path="C:/ti/mmwave_sdk_03_01_00_02" --include_path="C:/ti/mmwave_sdk_03_01_00_02/packages" --include_path="C:/ti/ccs920/ccs/tools/compiler/ti-cgt-arm_18.12.3.LTS/include" --define=_LITTLE_ENDIAN --define=MMWAVE_L3RAM_NUM_BANK=8 --define=MMWAVE_SHMEM_TCMA_NUM_BANK=0 --define=MMWAVE_SHMEM_TCMB_NUM_BANK=0 --define=MMWAVE_SHMEM_BANK_SIZE=0x20000 --define=SOC_XWR18XX --define=SUBSYS_MSS --define=DOWNLOAD_FROM_CCS --define=SHMEM_ALLOC=0x00000008 --define=DebugP_ASSERT_ENABLED -g --c99 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --enum_type=int --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


