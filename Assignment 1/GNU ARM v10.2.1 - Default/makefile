################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/subdir.mk
-include gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_protocol_crypto/src/subdir.mk
-include gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/src/subdir.mk
-include gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_alt/source/subdir.mk
-include gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/subdir.mk
-include gecko_sdk_3.2.7/util/silicon_labs/silabs_core/memory_manager/subdir.mk
-include gecko_sdk_3.2.7/protocol/bluetooth/src/subdir.mk
-include gecko_sdk_3.2.7/protocol/bluetooth/lib/EFR32BG13P/GCC/subdir.mk
-include gecko_sdk_3.2.7/platform/service/udelay/src/subdir.mk
-include gecko_sdk_3.2.7/platform/service/system/src/subdir.mk
-include gecko_sdk_3.2.7/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_3.2.7/platform/service/power_manager/src/subdir.mk
-include gecko_sdk_3.2.7/platform/service/mpu/src/subdir.mk
-include gecko_sdk_3.2.7/platform/service/iostream/src/subdir.mk
-include gecko_sdk_3.2.7/platform/service/device_init/src/subdir.mk
-include gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/rail_util_pti/subdir.mk
-include gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/subdir.mk
-include gecko_sdk_3.2.7/platform/middleware/glib/glib/subdir.mk
-include gecko_sdk_3.2.7/platform/middleware/glib/dmd/display/subdir.mk
-include gecko_sdk_3.2.7/platform/emlib/src/subdir.mk
-include gecko_sdk_3.2.7/platform/driver/i2cspm/src/subdir.mk
-include gecko_sdk_3.2.7/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_3.2.7/platform/common/src/subdir.mk
-include gecko_sdk_3.2.7/platform/bootloader/api/subdir.mk
-include gecko_sdk_3.2.7/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/subdir.mk
-include gecko_sdk_3.2.7/platform/Device/SiliconLabs/EFR32BG13P/Source/subdir.mk
-include gecko_sdk_3.2.7/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/subdir.mk
-include gecko_sdk_3.2.7/hardware/driver/memlcd/src/memlcd_usart/subdir.mk
-include gecko_sdk_3.2.7/hardware/driver/memlcd/src/subdir.mk
-include gecko_sdk_3.2.7/hardware/board/src/subdir.mk
-include gecko_sdk_3.2.7/app/common/util/app_log/subdir.mk
-include gecko_sdk_3.2.7/app/bluetooth/common/ota_dfu/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := ecen5823-assignment1-kiranj26
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: ecen5823-assignment1-kiranj26.axf

# Tool invocations
ecen5823-assignment1-kiranj26.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/autogen/linkerfile.ld" -Xlinker --gc-sections -Xlinker -Map="ecen5823-assignment1-kiranj26.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -u _printf_float -o ecen5823-assignment1-kiranj26.axf -Wl,--start-group "./app.o" "./app_properties.o" "./main.o" "./autogen/gatt_db.o" "./autogen/sl_bluetooth.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_i2cspm_init.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_usart_instances.o" "./autogen/sl_power_manager_handler.o" "./gecko_sdk_3.2.7/app/bluetooth/common/ota_dfu/sl_ota_dfu.o" "./gecko_sdk_3.2.7/app/common/util/app_log/app_log.o" "./gecko_sdk_3.2.7/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_3.2.7/hardware/board/src/sl_board_init.o" "./gecko_sdk_3.2.7/hardware/driver/memlcd/src/memlcd_usart/sl_memlcd_spi.o" "./gecko_sdk_3.2.7/hardware/driver/memlcd/src/sl_memlcd.o" "./gecko_sdk_3.2.7/hardware/driver/memlcd/src/sl_memlcd_display.o" "./gecko_sdk_3.2.7/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o" "./gecko_sdk_3.2.7/platform/Device/SiliconLabs/EFR32BG13P/Source/GCC/startup_efr32bg13p.o" "./gecko_sdk_3.2.7/platform/Device/SiliconLabs/EFR32BG13P/Source/system_efr32bg13p.o" "./gecko_sdk_3.2.7/platform/bootloader/api/btl_interface.o" "./gecko_sdk_3.2.7/platform/bootloader/api/btl_interface_storage.o" "./gecko_sdk_3.2.7/platform/common/src/sl_slist.o" "./gecko_sdk_3.2.7/platform/common/src/sl_status.o" "./gecko_sdk_3.2.7/platform/common/src/sl_string.o" "./gecko_sdk_3.2.7/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_3.2.7/platform/driver/i2cspm/src/sl_i2cspm.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_assert.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_cmu.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_core.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_crypto.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_emu.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_gpio.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_i2c.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_letimer.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_msc.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_prs.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_rtcc.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_system.o" "./gecko_sdk_3.2.7/platform/emlib/src/em_usart.o" "./gecko_sdk_3.2.7/platform/middleware/glib/dmd/display/dmd_memlcd.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/bmp.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_bitmap.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_circle.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_font_narrow_6x8.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_font_normal_8x8.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_font_number_16x20.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_line.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_polygon.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_rectangle.o" "./gecko_sdk_3.2.7/platform/middleware/glib/glib/glib_string.o" "./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o" "./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o" "./gecko_sdk_3.2.7/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o" "./gecko_sdk_3.2.7/platform/service/device_init/src/sl_device_init_dcdc_s1.o" "./gecko_sdk_3.2.7/platform/service/device_init/src/sl_device_init_emu_s1.o" "./gecko_sdk_3.2.7/platform/service/device_init/src/sl_device_init_hfxo_s1.o" "./gecko_sdk_3.2.7/platform/service/device_init/src/sl_device_init_lfxo_s1.o" "./gecko_sdk_3.2.7/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_3.2.7/platform/service/iostream/src/sl_iostream.o" "./gecko_sdk_3.2.7/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./gecko_sdk_3.2.7/platform/service/iostream/src/sl_iostream_stdlib_config.o" "./gecko_sdk_3.2.7/platform/service/iostream/src/sl_iostream_uart.o" "./gecko_sdk_3.2.7/platform/service/iostream/src/sl_iostream_usart.o" "./gecko_sdk_3.2.7/platform/service/mpu/src/sl_mpu.o" "./gecko_sdk_3.2.7/platform/service/power_manager/src/sl_power_manager.o" "./gecko_sdk_3.2.7/platform/service/power_manager/src/sl_power_manager_debug.o" "./gecko_sdk_3.2.7/platform/service/power_manager/src/sl_power_manager_hal_s0_s1.o" "./gecko_sdk_3.2.7/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_3.2.7/platform/service/sleeptimer/src/sl_sleeptimer_hal_prortc.o" "./gecko_sdk_3.2.7/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_3.2.7/platform/service/system/src/sl_system_init.o" "./gecko_sdk_3.2.7/platform/service/system/src/sl_system_process_action.o" "./gecko_sdk_3.2.7/platform/service/udelay/src/sl_udelay.o" "./gecko_sdk_3.2.7/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" "./gecko_sdk_3.2.7/protocol/bluetooth/src/sl_bt_mbedtls_context.o" "./gecko_sdk_3.2.7/util/silicon_labs/silabs_core/memory_manager/sl_malloc.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/aes.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/bignum.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/cipher.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/cipher_wrap.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/cmac.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/ctr_drbg.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/ecdh.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/ecdsa.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/ecp.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/ecp_curves.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/entropy.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/entropy_poll.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/error.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/hmac_drbg.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/platform.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/platform_util.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/psa_crypto_client.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/sha256.o" "./gecko_sdk_3.2.7/util/third_party/crypto/mbedtls/library/threading.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_alt/source/sl_entropy.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_alt/source/sl_mbedtls.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/src/crypto_aes.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/src/crypto_ecp.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/src/mbedtls_cmac.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_mbedtls_support/src/mbedtls_sha.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_protocol_crypto/src/sli_protocol_crypto_crypto.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/crypto_management.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_crypto_transparent_driver_aead.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_crypto_transparent_driver_cipher.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_crypto_transparent_driver_hash.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_crypto_transparent_driver_mac.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_crypto_trng_driver.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_psa_driver_common.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_psa_driver_init.o" "./gecko_sdk_3.2.7/util/third_party/crypto/sl_component/sl_psa_driver/src/sli_psa_trng.o" "./src/gpio.o" "./src/lcd.o" "./src/log.o" "/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/protocol/bluetooth/lib/EFR32BG13P/GCC/binapploader.o" "/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/protocol/bluetooth/lib/EFR32BG13P/GCC/libbluetooth.a" "/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/protocol/bluetooth/lib/EFR32BG13P/GCC/libpsstore.a" "/Users/kiran/SimplicityStudio/v5_workspace/ecen5823-assignment1-kiranj26/gecko_sdk_3.2.7/platform/radio/rail_lib/autogen/librail_release/librail_efr32xg13_gcc_release.a" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: ecen5823-assignment1-kiranj26.out'
	arm-none-eabi-objcopy "ecen5823-assignment1-kiranj26.axf" "ecen5823-assignment1-kiranj26.out"
	@echo ' '

	@echo 'Building hex file: ecen5823-assignment1-kiranj26.hex'
	arm-none-eabi-objcopy -O ihex "ecen5823-assignment1-kiranj26.axf" "ecen5823-assignment1-kiranj26.hex"
	@echo ' '

	@echo 'Building bin file: ecen5823-assignment1-kiranj26.bin'
	arm-none-eabi-objcopy -O binary "ecen5823-assignment1-kiranj26.axf" "ecen5823-assignment1-kiranj26.bin"
	@echo ' '

	@echo 'Building s37 file: ecen5823-assignment1-kiranj26.s37'
	arm-none-eabi-objcopy -O srec "ecen5823-assignment1-kiranj26.axf" "ecen5823-assignment1-kiranj26.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "ecen5823-assignment1-kiranj26.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) ecen5823-assignment1-kiranj26.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
