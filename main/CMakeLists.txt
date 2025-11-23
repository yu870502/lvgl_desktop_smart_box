#set(ESP32X_PROJECT_ROOT_DIR "${IDF_PATH}/my_esp32_prj")

# 合并你的源文件和 LVGL 源文件
set(srcs
    "app_main.c"
    lv_port/lv_port_disp.c
    lv_port/lv_port_fs.c
    lv_port/lv_port_indev.c
    ui/ui.c
)

# 注册组件（包含所有源文件和头文件路径）
idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS 
        "."
        lv_port
        ui
    PRIV_REQUIRES
        lvgl
        esp_driver_gpio
        esp_timer
        ds18b20
        onewire_bus
        uc1638
        humidity_sensor
        smartconfig
)

set_source_files_properties(app_main.c PROPERTIES COMPILE_FLAGS "-save-temps=obj")
