#include "lvgl.h"

void update_temp_display(lv_obj_t *, float *);
void update_humi_display(lv_obj_t *, float *);
void update_time_display(lv_obj_t *, void *);
void update_wifi_rssi(lv_obj_t *, int rssi);