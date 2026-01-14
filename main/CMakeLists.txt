# set(ESP32X_PROJECT_ROOT_DIR "${IDF_PATH}/my_esp32_prj")

# 在ESP-IDF组件系统中:组件CMakeLists.txt（如main/CMakeLists.txt）：不能使用 ADD_SUBDIRECTORY
# add_subdirectory(paho.mqtt.embedded-c)  

file(GLOB srcs
    "*.c"
    "lv_port/*.c"
    "ui/*.c"
    "network_moni/*.c"
    "sntp_sync/*.c"
    # "linkkit_mqtt/*.c"
    )

file(GLOB inc
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lv_port
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/network_moni
    ${CMAKE_CURRENT_SOURCE_DIR}/sntp_sync
    # ${CMAKE_CURRENT_SOURCE_DIR}/linkkit_mqtt
    ${CMAKE_CURRENT_SOURCE_DIR}/dbg_test
    )

# 方法1：使用foreach
# message("inc = ---------------------------------------")
# foreach(item ${inc})
#     message("  ${item}")
# endforeach()

# 注册组件（包含所有源文件和头文件路径）
idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${inc}

    PRIV_REQUIRES
        lvgl
        esp_driver_gpio
        esp_timer
        esp_wifi
        esp_adc
        ds18b20
        onewire_bus
        uc1638
        humidity_sensor
        WLAN
        ec11
        esp_driver_pcnt
        esp_system
        # LinkSDK
)

# set_source_files_properties(app_main.c PROPERTIES COMPILE_FLAGS "-save-temps=obj")
