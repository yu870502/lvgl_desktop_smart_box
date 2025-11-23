# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_C
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/cpu.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/cpu.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/esp_memory_utils.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/esp_memory_utils.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/chip_info.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/chip_info.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/cpu_region_protect.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/cpu_region_protect.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/esp_cpu_intr.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/esp_cpu_intr.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/rtc_clk.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/rtc_clk.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/rtc_clk_init.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/rtc_clk_init.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/rtc_init.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/rtc_init.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/rtc_sleep.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/rtc_sleep.c.obj"
  "/home/alientek/workspace/esp-idf/components/esp_hw_support/port/esp32/rtc_time.c" "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_hw_support/CMakeFiles/__idf_esp_hw_support.dir/port/esp32/rtc_time.c.obj"
  )
set(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "BOOTLOADER_BUILD=1"
  "ESP_PLATFORM"
  "IDF_VER=\"HEAD-HASH-NOTFOUND\""
  "NON_OS_BUILD=1"
  "SOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE"
  "SOC_XTAL_FREQ_MHZ=CONFIG_XTAL_FREQ"
  "_GLIBCXX_HAVE_POSIX_SEMAPHORE"
  "_GLIBCXX_USE_POSIX_SEMAPHORE"
  "_GNU_SOURCE"
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "config"
  "../../../../../../../../components/esp_hw_support/include"
  "../../../../../../../../components/esp_hw_support/include/soc"
  "../../../../../../../../components/esp_hw_support/include/soc/esp32"
  "../../../../../../../../components/esp_hw_support/dma/include"
  "../../../../../../../../components/esp_hw_support/ldo/include"
  "../../../../../../../../components/esp_hw_support/debug_probe/include"
  "../../../../../../../../components/esp_hw_support/mspi_timing_tuning/include"
  "../../../../../../../../components/esp_hw_support/power_supply/include"
  "../../../../../../../../components/esp_hw_support/port/include"
  "../../../../../../../../components/esp_hw_support/include/esp_private"
  "../../../../../../../../components/esp_hw_support/port/esp32/."
  "../../../../../../../../components/esp_hw_support/port/esp32/include"
  "../../../../../../../../components/log/include"
  "../../../../../../../../components/esp_rom/include"
  "../../../../../../../../components/esp_rom/esp32/include"
  "../../../../../../../../components/esp_rom/esp32/include/esp32"
  "../../../../../../../../components/esp_rom/esp32"
  "../../../../../../../../components/esp_common/include"
  "../../../../../../../../components/newlib/platform_include"
  "../../../../../../../../components/xtensa/esp32/include"
  "../../../../../../../../components/xtensa/include"
  "../../../../../../../../components/xtensa/deprecated_include"
  "../../../../../../../../components/soc/include"
  "../../../../../../../../components/soc/esp32"
  "../../../../../../../../components/soc/esp32/include"
  "../../../../../../../../components/soc/esp32/register"
  "../../../../../../../../components/efuse/include"
  "../../../../../../../../components/efuse/esp32/include"
  "../../../../../../../../components/spi_flash/include"
  "../../../../../../../../components/hal/platform_port/include"
  "../../../../../../../../components/hal/esp32/include"
  "../../../../../../../../components/hal/include"
  "../../../../../../../../components/bootloader_support/include"
  "../../../../../../../../components/bootloader_support/bootloader_flash/include"
  "../../../../../../../../components/bootloader_support/private_include"
  "../../../../../../../../components/esp_security/include"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/log/CMakeFiles/__idf_log.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_rom/CMakeFiles/__idf_esp_rom.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_common/CMakeFiles/__idf_esp_common.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/xtensa/CMakeFiles/__idf_xtensa.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/soc/CMakeFiles/__idf_soc.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/efuse/CMakeFiles/__idf_efuse.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/spi_flash/CMakeFiles/__idf_spi_flash.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/bootloader_support/CMakeFiles/__idf_bootloader_support.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_system/CMakeFiles/__idf_esp_system.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/hal/CMakeFiles/__idf_hal.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/micro-ecc/CMakeFiles/__idf_micro-ecc.dir/DependInfo.cmake"
  "/home/alientek/workspace/esp-idf/app/lvgl_st7567_spi/managed_components/espressif__ds18b20/examples/ds18b20_read/build/bootloader/esp-idf/esp_bootloader_format/CMakeFiles/__idf_esp_bootloader_format.dir/DependInfo.cmake"
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
