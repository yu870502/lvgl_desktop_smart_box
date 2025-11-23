#include "lvgl.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static lv_obj_t* temp_meter;
static lv_obj_t* humi_meter; 
static lv_obj_t* time_label;

lv_meter_indicator_t* temp_needle = NULL;
lv_meter_indicator_t* humi_needle = NULL;

lv_obj_t* temp_val_label = NULL;
lv_obj_t* humi_val_label = NULL;

// 创建温度仪表盘
lv_obj_t* create_temp_meter(){
    temp_meter = lv_meter_create(lv_scr_act());
    if(!temp_meter){
        printf("create temp meter failed\n");
        return NULL;
    }
    lv_obj_set_size(temp_meter, 96, 96);

    lv_meter_scale_t* scale = lv_meter_add_scale(temp_meter);
    // 设置小刻度数量为21，宽度为1，长度为3，颜色为黑色
    lv_meter_set_scale_ticks(temp_meter, scale, 21, 1, 3, lv_color_black());
    /* 设置主刻度的步长为4,宽度为2，长度为屏幕高度除以6，颜色为黑色， 刻度与数值的间距为8 */    
    lv_meter_set_scale_major_ticks(temp_meter, scale, 4, 2, 6, lv_color_black(), 8);
    // 移除内边距
    lv_obj_set_style_pad_all(temp_meter, 0, LV_PART_MAIN);
    lv_obj_set_style_text_font(temp_meter, &lv_font_montserrat_10, LV_PART_TICKS);

    //设置表盘最小值-20，最大值80，最大角度270，旋转角度135
    // 在单色屏240×96上, 的实用设置：lv_meter_set_scale_range(temp_meter, scale, min, max, 270, 135);
    // 这样设置后：angle_range=270：刻度范围270度,rotation=135：整个仪表盘逆时针旋转135度
    // 效果：0刻度在左下角，满量程在右下角，完美适配横向窄屏幕
    lv_meter_set_scale_range(temp_meter, scale, -20, 80, 270, 135);

    // 添加指针 - 关键代码
    temp_needle = lv_meter_add_needle_line(temp_meter, scale, 2, lv_color_black(), -20);

    temp_val_label = lv_label_create(temp_meter);
    lv_label_set_text(temp_val_label, "0°C");
    lv_obj_set_style_text_font(temp_val_label, &lv_font_montserrat_12, 0);
    lv_obj_align(temp_val_label, LV_ALIGN_BOTTOM_MID, 0, -2);
    
    return temp_meter;
}

// 创建湿度仪表盘
lv_obj_t* create_humi_meter(){
    humi_meter = lv_meter_create(lv_scr_act());
    if(!humi_meter){
        printf("create temp meter failed\n");
        return NULL;
    }
    lv_obj_set_size(humi_meter, 96, 96);
    lv_obj_align(humi_meter, LV_ALIGN_RIGHT_MID, 0, 0); 

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

void update_temp_display(float t){
    // 更新温度指针
    lv_meter_set_indicator_value(temp_meter, temp_needle, (int)t);
    char str[16] = {0};
    sprintf(str, "%.2f°C", t);
    lv_label_set_text(temp_val_label, str);
}
void update_humi_display(float h){
    // 更新湿度指针
    lv_meter_set_indicator_value(humi_meter, humi_needle, (int)h);
    char str[16] = {0};
    sprintf(str, "%.2f%%", h);
    lv_label_set_text(humi_val_label, str);
}
void update_time_display(void){
    // 更新时间
    char time_str[32];
    time_t now = time(NULL);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    lv_label_set_text(time_label, time_str);
}

void create_weather_dashboard(void) {    
    // 创建温度表盘
    create_temp_meter();
    
    // 创建湿度表盘
    create_humi_meter();

    // // 创建时间显示
    // time_label = lv_label_create(lv_scr_act());
    // lv_obj_set_style_text_font(time_label, &lv_font_montserrat_14, 0);
    // lv_obj_align(time_label, LV_ALIGN_BOTTOM_MID, 0, -5);
}
