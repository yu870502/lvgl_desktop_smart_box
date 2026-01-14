#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lvgl.h"

#include "ui.h"

#include "dbg_test.h"

static const char *TAG = "ui";

lv_obj_t *base_info_scr1 = NULL;
lv_obj_t *base_info_scr2 = NULL;
lv_obj_t *dev_info_scr = NULL;
lv_obj_t *cur_scr = NULL;

QueueHandle_t ui_update_queue;

lv_obj_t* temp_meter;
lv_obj_t* humi_meter; 

lv_obj_t* wifi_name_label;
lv_obj_t* wifi_rssi_label;

lv_obj_t* time_YMD_label;
lv_obj_t* time_label;
lv_obj_t* time_week_label;

lv_obj_t* sysRunTime_label;

lv_meter_indicator_t* temp_needle = NULL;
lv_meter_indicator_t* humi_needle = NULL;

lv_obj_t* temp_val_label = NULL;
lv_obj_t* humi_val_label = NULL;

lv_obj_t *time_label_2 = NULL;
lv_obj_t *date_label = NULL;
lv_obj_t *temp_label = NULL;
lv_obj_t *humi_label = NULL;

// 创建温度仪表盘
void create_scr1(void){
    // base_info_scr1 = lv_scr_act();
    base_info_scr1 = lv_obj_create(NULL);

    //创建温度表盘
    temp_meter = lv_meter_create(base_info_scr1);
    if(!temp_meter){
        printf("create temp meter failed\n");
        return;
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
    humi_meter = lv_meter_create(base_info_scr1);
    if(!humi_meter){
        printf("create temp meter failed\n");
        return;
    }
    lv_obj_set_size(humi_meter, 96, 96);
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

    // 创建时间显示
    time_YMD_label = lv_label_create(base_info_scr1);
    lv_obj_set_style_text_font(time_YMD_label, &lv_font_montserrat_14, 0);
    lv_label_set_text(time_YMD_label, "1970\n12-31");   //占位
    lv_obj_align_to(time_YMD_label, humi_meter, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);

    time_week_label = lv_label_create(base_info_scr1);
    lv_obj_set_style_text_font(time_week_label, &lv_font_montserrat_12, 0);
    lv_obj_align_to(time_week_label, time_YMD_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    time_label = lv_label_create(base_info_scr1);
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_10, 0);
    lv_obj_align_to(time_label, time_week_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
}

// 创建基本信息显示界面2
void create_scr2(void) {
    base_info_scr2 = lv_obj_create(NULL);
    
    // 1. 大时间显示 (左侧，大字体，时分秒用冒号分隔)
    time_label_2 = lv_label_create(base_info_scr2);
    lv_label_set_text(time_label_2, "00:00:01");
    lv_obj_set_style_text_font(time_label_2, &lv_font_montserrat_38, 0);  // 大字体
    // lv_obj_set_style_text_color(time_label_2, lv_color_white(), 0);
    lv_obj_align(time_label_2, LV_ALIGN_TOP_LEFT, 10, 10);
    
    // 2. 日期显示 (时间下方)
    date_label = lv_label_create(base_info_scr2);
    lv_label_set_text(date_label, "2025-01-01 Fri");
    lv_obj_set_style_text_font(date_label, &lv_font_montserrat_16, 0);
    // lv_obj_set_style_text_color(date_label, lv_color_white(), 0);
    // 对齐到时间label下方
    lv_obj_align_to(date_label, time_label_2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    
    // 3. 计算竖线位置 (在时间label右侧)
    lv_coord_t time_width = lv_obj_get_width(time_label_2);
    lv_coord_t time_x = lv_obj_get_x(time_label_2);
    lv_coord_t line_x = time_x + time_width + 2;  // 距离时间15像素
    
    // 创建竖线分隔符 (从顶部到底部)
    lv_obj_t *vline = lv_obj_create(base_info_scr2);
    lv_obj_remove_style_all(vline);
    lv_obj_set_size(vline, 2, 80);  // 竖线高度80像素，几乎占满屏幕高度
    lv_obj_set_pos(vline, line_x, 2);  // 从y=8开始，留点边距
    lv_obj_set_style_bg_color(vline, lv_color_white(), 0);

    // 4. 右侧区域 - 温度显示 (竖线右侧)
    temp_label = lv_label_create(base_info_scr2);
    lv_label_set_text(temp_label, "25.0°C");
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_14, 0);
    // lv_obj_set_style_text_color(temp_label, lv_color_white(), 0);
    // 对齐到竖线右侧
    lv_obj_align_to(temp_label, vline, LV_ALIGN_OUT_RIGHT_TOP, 15, 15);
    
    // 5. 右侧区域 - 湿度显示 (温度下方)
    humi_label = lv_label_create(base_info_scr2);
    lv_label_set_text(humi_label, "50%");
    lv_obj_set_style_text_font(humi_label, &lv_font_montserrat_14, 0);
    // lv_obj_set_style_text_color(humi_label, lv_color_white(), 0);
    // 对齐到温度label下方
    lv_obj_align_to(humi_label, temp_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
}

int create_dev_info_scr(){
    dev_info_scr = lv_obj_create(NULL);

    lv_obj_t * label = lv_label_create(dev_info_scr);
    lv_label_set_text(label, "Base Info:");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);

    wifi_name_label = lv_label_create(dev_info_scr);
    lv_label_set_text(wifi_name_label, WIFI_NAME_LABLE_FORMAT);
    lv_obj_set_style_text_font(wifi_name_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(wifi_name_label, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    wifi_rssi_label = lv_label_create(dev_info_scr);
    lv_label_set_text(wifi_rssi_label, WIFI_RSSI_LABLE_FORMAT);
    lv_obj_set_style_text_font(wifi_rssi_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(wifi_rssi_label, wifi_name_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    sysRunTime_label = lv_label_create(dev_info_scr);
    lv_label_set_text(sysRunTime_label, "Run Time: 0");
    lv_obj_set_style_text_font(sysRunTime_label, &lv_font_montserrat_16, 0);
    lv_obj_align_to(sysRunTime_label, wifi_rssi_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    return 0;
}

void my_gui_init(void) {
    ui_update_queue = xQueueCreate(20, sizeof(UI_Update_Message)); 

    // 创建温度表盘
    create_scr1();
    create_scr2();
    create_dev_info_scr();    

    cur_scr = base_info_scr2;
    lv_scr_load(cur_scr);

    lv_group_t * _widget_grps = lv_group_create();
    // lv_group_add_obj(group, obj);
    // lv_indev_set_group(indev_encoder, group);
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
    // UBaseType_t n = uxQueueMessagesWaiting(ui_update_queue);
    // ESP_LOGI("UI", "Queue wait num: %d", n);
    // n =  uxQueueSpacesAvailable( ui_update_queue );
    // ESP_LOGI("UI", "Queue free num: %d", n);
    xQueueSend(ui_update_queue, &msg, portMAX_DELAY);
}

