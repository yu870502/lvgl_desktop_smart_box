#include "ui.h"
#include "sntp_sync.h"
#include "network_moni.h"
#include "app_main.h"

void update_temp(lv_obj_t *target, float *v){
    float t = *v;
    lv_meter_set_indicator_value(target, temp_needle, (int)t);

    char str[16] = {0};
    sprintf(str, "%.2f°C", t);
    lv_label_set_text(temp_val_label, str);

    memset(str, 0, sizeof(str));
    sprintf(str, "%.2f°C", t);
    lv_label_set_text(temp_label, str);    
}
void update_humi(lv_obj_t *target, int *v){
    int h = *v;
    lv_meter_set_indicator_value(target, humi_needle, h);

    char str[16] = {0};
    sprintf(str, "%d%%", h);
    lv_label_set_text(humi_val_label, str);

    memset(str, 0, sizeof(str));
    sprintf(str, "%d%%", h);
    lv_label_set_text(humi_label, str);
}
void update_time(lv_obj_t *target, void *arg){
    char time_str[32] = {0};
    time_t now = time(NULL);
    struct tm now_tm, old_tm;

    if(now != curTime){
        localtime_r(&curTime, &old_tm);
        curTime = now;
        localtime_r(&now, &now_tm);
    }

    //第一屏幕
    if(now_tm.tm_year != old_tm.tm_year || now_tm.tm_mon != old_tm.tm_mon || now_tm.tm_mday != old_tm.tm_mday){
        strftime(time_str, sizeof(time_str), "%Y\n%m-%d", &now_tm);
        lv_label_set_text(time_YMD_label, time_str);
    }

    if(now_tm.tm_wday != old_tm.tm_wday){
        strftime(time_str, sizeof(time_str), "%a", &now_tm);
        lv_label_set_text(time_week_label, time_str);
    }

    if(now_tm.tm_hour != old_tm.tm_hour || now_tm.tm_min != old_tm.tm_min || now_tm.tm_sec != old_tm.tm_sec){
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &now_tm);
        lv_label_set_text(time_label, time_str);
    }

    //第二屏幕
    if(now_tm.tm_hour != old_tm.tm_hour || now_tm.tm_min != old_tm.tm_min || now_tm.tm_sec != old_tm.tm_sec){
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &now_tm);
        lv_label_set_text(time_label_2, time_str);
    }
    if(now_tm.tm_year != old_tm.tm_year || now_tm.tm_mon != old_tm.tm_mon \
        || now_tm.tm_mday != old_tm.tm_mday || now_tm.tm_wday != old_tm.tm_wday){
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %a", &now_tm);
        lv_label_set_text(date_label, time_str);
    }
}

// 更新函数
void update_wifi_rssi(lv_obj_t *target, int *rssi) {
    char str[32] = {0};
    sprintf(str, WIFI_RSSI_LABLE_FORMAT, *rssi);
    lv_label_set_text(target, str);
}

// 更新函数
void update_wifi_name(lv_obj_t *target, char *name) {
    char str[64] = {0};
    sprintf(str, WIFI_NAME_LABLE_FORMAT, name, curCC);
    lv_label_set_text(target, str);
}

void update_sysRunTime(lv_obj_t *target, TimeHMS_t *t)
{
    char str[64] = {0};
    sprintf(str, SYSRUNTIME_LABLE_FORMAT, t->h, t->m);
    lv_label_set_text(target, str);
}