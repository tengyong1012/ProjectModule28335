################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"c:/ti/ccsv6/tools/compiler/c2000_6.2.7/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="c:/ti/ccsv6/tools/compiler/c2000_6.2.7/include" --include_path="D:/C28335/Proj/ProjectModule28335/Dev/Can" --include_path="D:/C28335/Proj/ProjectModule28335/Usr" --include_path="D:/C28335/Proj/ProjectModule28335/Dev/Sram" --include_path="D:/C28335/Proj/ProjectModule28335/Dev/E2prom" --include_path="D:/C28335/Proj/ProjectModule28335/Dev/I2c" --include_path="D:/C28335/Proj/ProjectModule28335/Dev/Gpio" --include_path="D:/C28335/Proj/ProjectModule28335/Dev/Scia" --include_path="D:/C28335/Proj/ProjectModule28335/DSP2833x_common/include" --include_path="D:/C28335/Proj/ProjectModule28335/DSP2833x_common/source" --include_path="D:/C28335/Proj/ProjectModule28335/DSP2833x_headers/include" --include_path="D:/C28335/Proj/ProjectModule28335/DSP2833x_headers/source" -g --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


