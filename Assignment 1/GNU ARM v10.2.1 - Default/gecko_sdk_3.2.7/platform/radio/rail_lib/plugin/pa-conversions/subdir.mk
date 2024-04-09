################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c \
../gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c 

OBJS += \
./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o \
./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o 

C_DEPS += \
./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.d \
./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o: ../gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/config" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/config/btconf" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/autogen" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/Device/SiliconLabs/EFR32BG13P/Include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/app/common/util/app_assert" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/app/common/util/app_log" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/common/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/protocol/bluetooth/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/board/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/bootloader" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/bootloader/api" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/CMSIS/Include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/device_init/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/middleware/glib/dmd" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/middleware/glib" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/emlib/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/middleware/glib/glib" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/driver/i2cspm/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/iostream/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/memlcd/src/ls013b7dh03" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/config" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_alt/include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/memlcd/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/memlcd/inc/memlcd_usart" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/mpu/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/app/bluetooth/common/ota_dfu" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/power_manager/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/common" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/protocol/ble" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/protocol/ieee802154" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/protocol/zwave" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/chip/efr32/efr32xg1x" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/efr32xg1x" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/rail_util_pti" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/silicon_labs/silabs_core/memory_manager" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/common/toolchain/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/system/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/sleeptimer/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_protocol_crypto/src" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/udelay/inc" -O0 -Wall -Wextra -fno-builtin -fomit-frame-pointer -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o: ../gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DEFR32BG13P632F512GM48=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/config" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/config/btconf" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/autogen" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/Device/SiliconLabs/EFR32BG13P/Include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/app/common/util/app_assert" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/app/common/util/app_log" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/common/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/protocol/bluetooth/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/board/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/bootloader" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/bootloader/api" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/CMSIS/Include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/device_init/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/middleware/glib/dmd" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/middleware/glib" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/emlib/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/middleware/glib/glib" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/driver/i2cspm/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/iostream/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/memlcd/src/ls013b7dh03" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/config" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_alt/include" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/memlcd/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/memlcd/inc/memlcd_usart" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/mpu/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/app/bluetooth/common/ota_dfu" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/power_manager/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/common" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/protocol/ble" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/protocol/ieee802154" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/protocol/zwave" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/chip/efr32/efr32xg1x" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/efr32xg1x" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/rail_util_pti" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/silicon_labs/silabs_core/memory_manager" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/common/toolchain/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/system/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/sleeptimer/inc" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_protocol_crypto/src" -I"/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/service/udelay/inc" -O0 -Wall -Wextra -fno-builtin -fomit-frame-pointer -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


