# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(HAL_COMPONENTS_DIR "$ENV{IDF_PATH}/my_esp32_prj/hal_components")

set(HAL_HUMIDITY_SENSOR_DIR "${HAL_COMPONENTS_DIR}/humidity_sensor")
list(APPEND EXTRA_COMPONENT_DIRS ${HAL_HUMIDITY_SENSOR_DIR})  # 指向HAL

set(HAL_UC1638_DIR "${HAL_COMPONENTS_DIR}/uc1638")
list(APPEND EXTRA_COMPONENT_DIRS ${HAL_UC1638_DIR})  # 指向HAL

set(HAL_FLEXIBLE_BUTTON_DIR "${HAL_COMPONENTS_DIR}/flexible_button")
list(APPEND EXTRA_COMPONENT_DIRS ${HAL_FLEXIBLE_BUTTON_DIR})  # 指向HAL

set(HAL_SMARTCONFIG_DIR "${HAL_COMPONENTS_DIR}/smartconfig")
list(APPEND EXTRA_COMPONENT_DIRS ${HAL_SMARTCONFIG_DIR})  # 指向HAL

message(STATUS "EXTRA_COMPONENT_DIRS = ${EXTRA_COMPONENT_DIRS}")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)
project(lvgl_ui)

