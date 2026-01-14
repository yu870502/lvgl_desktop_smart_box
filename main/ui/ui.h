#include "lvgl.h"
#include <stdio.h>
#include <time.h>

#define WIFI_NAME_LABLE_FORMAT "WIFI:%s\nCC:%s"
#define WIFI_RSSI_LABLE_FORMAT "RSSI:%ddb"
#define SYSRUNTIME_LABLE_FORMAT "run time:%d(h):%d(m)"

typedef struct {
    lv_obj_t* target;    // 目标控件
    void* data;          // 数据指针
    void (*update_func)(lv_obj_t*, void*); // 更新函数
} UI_Update_Message;

extern lv_obj_t *base_info_scr1;
extern lv_obj_t *base_info_scr2;
extern lv_obj_t *dev_info_scr;
extern lv_obj_t *cur_scr;

// extern lv_obj_t *default_scr;
// extern lv_obj_t *base_info_scr;
// extern lv_obj_t *cur_scr;

extern lv_obj_t *time_label_2;
extern lv_obj_t *date_label;
extern lv_obj_t *temp_label;
extern lv_obj_t *humi_label;

extern lv_obj_t* temp_meter;
extern lv_obj_t* humi_meter;

extern lv_meter_indicator_t* temp_needle;
extern lv_meter_indicator_t* humi_needle;

extern lv_obj_t* temp_val_label;
extern lv_obj_t* humi_val_label;

extern lv_obj_t* wifi_name_label;
extern lv_obj_t* wifi_rssi_label;

extern lv_obj_t* time_YMD_label;
extern lv_obj_t* time_label;
extern lv_obj_t* time_week_label;

extern lv_obj_t* sysRunTime_label;

void process_ui_messages(void);

void my_gui_init(void);

void update_ui_safe(lv_obj_t* target, void *func, void* data, size_t data_size);
