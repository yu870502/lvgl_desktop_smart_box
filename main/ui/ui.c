#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lvgl.h"

#include "ui.h"

QueueHandle_t ui_update_queue;

lv_obj_t* temp_meter;
lv_obj_t* humi_meter; 
lv_obj_t* wifi_label;
lv_obj_t* time_YMD_label;
lv_obj_t* time_label;
lv_obj_t* time_week_label;

lv_meter_indicator_t* temp_needle = NULL;
lv_meter_indicator_t* humi_needle = NULL;

lv_obj_t* temp_val_label = NULL;
lv_obj_t* humi_val_label = NULL;

// 创建温度仪表盘
lv_obj_t* create_meter(){
    //创建温度
    temp_meter = lv_meter_create(lv_scr_act());
    if(!temp_meter){
        printf("create temp meter failed\n");
        return NULL;
    }
    lv_obj_set_size(temp_meter, 96, 96);

    lv_meter_scale_t* scale_temp = lv_meter_add_scale(temp_meter);
    // 设置小刻度数量为21，宽度为1，长度为3，颜色为黑色
    lv_meter_set_scale_ticks(temp_meter, scale_temp, 21, 1, 3, lv_color_black());
    /* 设置主刻度的步长为4,宽度为2，长度为屏幕高度除以6，颜色为黑色， 刻度与数值的间距为8 */    
    lv_meter_set_scale_major_ticks(temp_meter, scale_temp, 4, 2, 6, lv_color_black(), 8);
    // 移除内边距
    lv_obj_set_style_pad_all(temp_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_text_font(temp_meter, &lv_font_montserrat_10, LV_PART_TICKS);

    //设置表盘最小值-20，最大值80，最大角度270，旋转角度135
    // 在单色屏240×96上, 的实用设置：lv_meter_set_scale_range(temp_meter, scale_temp, min, max, 270, 135);
    // 这样设置后：angle_range=270：刻度范围270度,rotation=135：整个仪表盘逆时针旋转135度
    // 效果：0刻度在左下角，满量程在右下角，完美适配横向窄屏幕
    lv_meter_set_scale_range(temp_meter, scale_temp, -20, 80, 270, 135);

    // 添加指针 - 关键代码
    temp_needle = lv_meter_add_needle_line(temp_meter, scale_temp, 2, lv_color_black(), -20);

    temp_val_label = lv_label_create(temp_meter);
    lv_label_set_text(temp_val_label, "0°C");
    lv_obj_set_style_text_font(temp_val_label, &lv_font_montserrat_12, 0);
    lv_obj_align(temp_val_label, LV_ALIGN_BOTTOM_MID, 0, -2);
//------------------------------------------------------------------------------------------
    //创建湿度表盘
    humi_meter = lv_meter_create(lv_scr_act());
    if(!humi_meter){
        printf("create temp meter failed\n");
        return NULL;
    }
    lv_obj_set_size(humi_meter, 96, 96);
    // lv_obj_align(humi_meter, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_align_to(humi_meter, temp_meter, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_meter_scale_t* scale = lv_meter_add_scale(humi_meter);
    // 设置小刻度数量为11，宽度为1，长度为3，颜色为黑色
    lv_meter_set_scale_ticks(humi_meter, scale, 11, 1, 3, lv_color_black());
    /* 设置主刻度的步长为2,宽度为2，长度为屏幕高度除以6，颜色为黑色， 刻度与数值的间距为8 */    
    lv_meter_set_scale_major_ticks(humi_meter, scale, 2, 2, 6, lv_color_black(), 8);
    // 移除内边距
    lv_obj_set_style_pad_all(humi_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_text_font(humi_meter, &lv_font_montserrat_10, LV_PART_TICKS);

    //设置表盘最小值-20，最大值80，最大角度270，旋转角度135
    // 在单色屏240×96上, 的实用设置：lv_meter_set_scale_range(humi_meter, scale, min, max, 270, 135);
    // 这样设置后：angle_range=270：刻度范围270度,rotation=135：整个仪表盘逆时针旋转135度
    // 效果：0刻度在左下角，满量程在右下角，完美适配横向窄屏幕
    lv_meter_set_scale_range(humi_meter, scale, 0, 100, 270, 135);

    // 添加指针 - 关键代码
    humi_needle = lv_meter_add_needle_line(humi_meter, scale, 2, lv_color_black(), -20);

    humi_val_label = lv_label_create(humi_meter);
    lv_label_set_text(humi_val_label, "0%");
    lv_obj_set_style_text_font(humi_val_label, &lv_font_montserrat_12, 0);
    lv_obj_align(humi_val_label, LV_ALIGN_BOTTOM_MID, 0, -2);

    return humi_meter;
}

// 创建函数
void create_wifi_display() {
    wifi_label = lv_label_create(lv_scr_act());
    lv_label_set_text(wifi_label, WIFI_RSSI_LABLE_FORMAT);
    lv_obj_set_style_text_font(wifi_label, &lv_font_montserrat_12, 0);
    lv_obj_align_to(wifi_label, humi_meter, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
}

void my_gui_init(void) {
    ui_update_queue = xQueueCreate(20, sizeof(UI_Update_Message)); 

    // 创建温度表盘
    create_meter();

    create_wifi_display();

    // 创建时间显示
    time_YMD_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(time_YMD_label, &lv_font_montserrat_10, 0);
    lv_obj_align_to(time_YMD_label, wifi_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    time_week_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(time_week_label, &lv_font_montserrat_12, 0);
    lv_obj_align_to(time_week_label, time_YMD_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    time_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_10, 0);
    lv_obj_align_to(time_label, time_week_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

}

void process_ui_messages(void) {
    UI_Update_Message msg;
    while(xQueueReceive(ui_update_queue, &msg, 0) == pdTRUE) {
        if(msg.target && msg.update_func) {
            msg.update_func(msg.target, msg.data);
        }
        if(msg.data){
            free(msg.data);
            msg.data = NULL;
        }
    }
}


// 通用更新函数模板
void update_ui_safe(lv_obj_t* target, void *func, void* data, size_t data_size) {
    UI_Update_Message msg = {
        .target = target,
        .update_func = func,
        .data = NULL
    };
 
    if(data && data_size > 0) {
        msg.data = pvPortMalloc(data_size);
        if(msg.data) memcpy(msg.data, data, data_size);
    }
 
    xQueueSend(ui_update_queue, &msg, portMAX_DELAY);
}
