#include "lvgl.h"
#include "app_main.h"

void update_temp(lv_obj_t *, float *);
void update_humi(lv_obj_t *, float *);
void update_time(lv_obj_t *, void *);
void update_wifi_rssi(lv_obj_t *, int rssi);
void update_wifi_name(lv_obj_t *target, char *name);
void update_sysRunTime(lv_obj_t *target, TimeHMS_t *t);
