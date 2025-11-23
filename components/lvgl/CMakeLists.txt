# 收集所有源文件
file(GLOB_RECURSE LVGL_SOURCES
    "./*.c"
)

# 自动收集所有头文件目录
file(GLOB_RECURSE LVGL_HEADERS
    "./*.h"
)

# 去重头文件目录
set(LVGL_INCLUDE_DIRS "")
foreach(header ${LVGL_HEADERS})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND LVGL_INCLUDE_DIRS ${dir})
endforeach()
list(REMOVE_DUPLICATES LVGL_INCLUDE_DIRS)

idf_component_register(
    SRCS ${LVGL_SOURCES}
    INCLUDE_DIRS ${LVGL_INCLUDE_DIRS}
)
