/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_timer.h" 

#include "lvgl.h"
#include "lv_mem.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "esp_log.h"
#include "onewire_bus.h"
#include "ds18b20.h"

#include "uc1638.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "humidity_sensor.h"
#include "smartconfig.h"

#include "ui.h"
#include "network_moni.h"
#include "sntp_sync.h"
#include "update_ui.h"

#include "driver/pulse_cnt.h"
#include "ec11.h"

static const char *TAG = "main";

/*
 This code displays some fancy graphics on the 320x240 LCD on an ESP-WROVER_KIT board.
 This example demonstrates the use of both spi_device_transmit as well as
 spi_device_queue_trans/spi_device_get_trans_result and pre-transmit callbacks.

 Some info about the ILI9341/ST7789V: It has an C/D line, which is connected to a GPIO here. It expects this
 line to be low for a command and high for data. We use a pre-transmit callback here to control that
 line: every transaction has as the user-definable argument the needed state of the D/C line and just
 before the transaction is sent, the callback will set this line to the correct state.
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////// Please update the following configuration according to your HardWare spec /////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#define KEY_GPIO 19
#define GPIO_INPUT_PIN_SEL (1ULL<<KEY_GPIO)

#define ESP_INTR_FLAG_DEFAULT 0

#define LV_ENABLE 1

#define EXAMPLE_ONEWIRE_BUS_GPIO    27
#define EXAMPLE_ONEWIRE_MAX_DS18B20 2

static QueueHandle_t ec11_evt_queue = NULL;
static QueueHandle_t gpio_evt_queue = NULL;

int ds18b20_device_num = 0;
onewire_bus_handle_t bus = NULL;
ds18b20_device_handle_t ds18b20s[EXAMPLE_ONEWIRE_MAX_DS18B20];

float curTemp;

void add_scale_labels(void);
// 初始化滚动文本
void init_scroll_text(void);

static lv_obj_t *temp_bar;
static lv_obj_t *scroll_label;
static const char *scroll_texts[] = {
    "   This is my show time    ",
    "欲买桂花同载酒,终不似少年游。", \
    "系统运行中,传感器工作正常",
};
static int current_text_index = 0;
static lv_anim_t scroll_anim;

// 显存镜像[12页][240列]，每字节存储8垂直像素
uint8_t display_data[12][240];
void area_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    uint8_t start_page = area->y1 / 8;    // 计算起始页：Y起点/8（ST7567每页8行）
    uint8_t end_page = area->y2 / 8;      // 计算结束页：Y终点/8
    uint8_t start_col = area->x1;         // 起始列=脏区X起点
    uint8_t end_col = area->x2;           // 结束列=脏区X终点

    // ESP_LOGI(TAG, "x1:%d, x2:%d, y1:%d, y2:%d\n", area->x1, area->x2, area->y1, area->y2);
    // ESP_LOGI(TAG, "func:%s, start_page:%u, end_page:%u, start_col:%u, end_col:%u\n", __FUNCTION__, start_page, end_page, start_col, end_col);

    uint32_t width = area->x2 - area->x1 + 1;

    // 将LVGL的color_p数据转换为ST7567的页数据格式
    for (uint8_t page = start_page; page <= end_page; page++) {
        for (uint8_t col = start_col; col <= end_col; col++) {
            for (uint8_t bit = 0; bit < 8; bit++) {         // 处理当前列的8个垂直像素
                uint8_t global_y = page * 8 + bit;          // 计算实际Y坐标：页号*8 + 位号
                if (global_y >= area->y1 && global_y <= area->y2) { // 检查像素是否在脏区内
                    uint32_t idx = (global_y - area->y1) * width + (col - area->x1);    // 计算在color_p中的索引
                    
                    // 根据LVGL像素数据设置显示数据
                    if (color_p[idx].full != 0) {
                        display_data[page][col] &= ~(1 << bit);
                    }
                    else{
                        display_data[page][col] |= (1 << bit);
                    }
                }
            }
        }
        // windowsPrograme(page, page, start_col, end_col, &display_data[page][start_col]);     //TODO:
        customPageWindowProgramming(page, page, start_col, end_col, &display_data[page][start_col]);
    }
    // windowsPrograme(start_page, end_page, start_col, end_col, &display_data[start_page][start_col]); //TODO:
    // customPageWindowProgramming(start_page, end_page, start_col, end_col, &display_data[start_page][start_col]);
}

void init_scroll_text(void) {
    // 创建滚动标签
    scroll_label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(scroll_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // 循环滚动模式
    lv_label_set_text(scroll_label, scroll_texts[0]);
    lv_obj_set_style_text_font(scroll_label, &lv_font_montserrat_14, 0);
    
    // 放在温度显示区域下方
    lv_obj_set_width(scroll_label, 90); // 设置宽度限制滚动范围
}

// 更新滚动文本（可选：定时切换不同文本）
void update_scroll_text(void) {
    if (scroll_label == NULL) return;
    
    // 切换到下一个文本
    current_text_index = (current_text_index + 1) % 4;
    lv_label_set_text(scroll_label, scroll_texts[current_text_index]);
}

void add_scale_labels(void) {
    const char *scale_texts[] = {"0", "10", "20", "30", "40", "50"};
    
    for (int i = 0; i < 6; i++) {
        lv_obj_t *scale_label = lv_label_create(lv_scr_act());
        lv_label_set_text(scale_label, scale_texts[i]);
        lv_obj_set_style_text_font(scale_label, &lv_font_montserrat_12, 0);
        lv_obj_align_to(scale_label, temp_bar, LV_ALIGN_OUT_TOP_MID, 
                       -90 + (i * 36), -5);  // 往上移一点给大字体留空间
    }
}

void getDs18b20TempTask(void *)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        // trigger curTemp conversion for all sensors on the bus
        ESP_ERROR_CHECK(ds18b20_trigger_temperature_conversion_for_all(bus));
        float temp;
        for (int i = 0; i < ds18b20_device_num; i ++) {
            ESP_ERROR_CHECK(ds18b20_get_temperature(ds18b20s[i], &temp));
            ESP_LOGI(TAG, "temperature read from DS18B20[%d]: %.2f°C", i, temp);
            if(temp != curTemp){
                curTemp = temp;
                update_ui_safe(temp_meter, update_temp, (void *)&curTemp, sizeof(curTemp));
            }
        }
    }
}

void init_ds18b20(void)
{
    // install 1-wire bus
    onewire_bus_config_t bus_config = {
        .bus_gpio_num = EXAMPLE_ONEWIRE_BUS_GPIO,
        .flags = {
            .en_pull_up = true, // enable the internal pull-up resistor in case the external device didn't have one
        }
    };
    onewire_bus_rmt_config_t rmt_config = {
        .max_rx_bytes = 10, // 1byte ROM command + 8byte ROM number + 1byte device command
    };
    ESP_ERROR_CHECK(onewire_new_bus_rmt(&bus_config, &rmt_config, &bus));

    onewire_device_iter_handle_t iter = NULL;
    onewire_device_t next_onewire_device;
    esp_err_t search_result = ESP_OK;

    // create 1-wire device iterator, which is used for device search
    ESP_ERROR_CHECK(onewire_new_device_iter(bus, &iter));
    ESP_LOGI(TAG, "Device iterator created, start searching...");
    do {
        search_result = onewire_device_iter_get_next(iter, &next_onewire_device);
        if (search_result == ESP_OK) { // found a new device, let's check if we can upgrade it to a DS18B20
            ds18b20_config_t ds_cfg = {};
            onewire_device_address_t address;
            // check if the device is a DS18B20, if so, return the ds18b20 handle
            if (ds18b20_new_device_from_enumeration(&next_onewire_device, &ds_cfg, &ds18b20s[ds18b20_device_num]) == ESP_OK) {
                ds18b20_get_device_address(ds18b20s[ds18b20_device_num], &address);
                ESP_LOGI(TAG, "Found a DS18B20[%d], address: %016llX", ds18b20_device_num, address);
                ds18b20_device_num++;
            } else {
                ESP_LOGI(TAG, "Found an unknown device, address: %016llX", next_onewire_device.address);
            }
        }
    } while (search_result != ESP_ERR_NOT_FOUND);
    ESP_ERROR_CHECK(onewire_del_device_iter(iter));
    ESP_LOGI(TAG, "Searching done, %d DS18B20 device(s) found", ds18b20_device_num);

    xTaskCreate(getDs18b20TempTask, "getTemp", 2 * 1024, NULL, 5, NULL);
}

static TaskHandle_t s_task_handle;

static adc_channel_t channel[2] = {ADC_CHANNEL_4, ADC_CHANNEL_5};
static int voltage = 0;
static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    //Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    return (mustYield == pdTRUE);
}

void continuous_read_task(void *)
{
    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0xcc, EXAMPLE_READ_LEN);

    s_task_handle = xTaskGetCurrentTaskHandle();

    adc_continuous_handle_t handle = NULL;
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));

    while (1) {

        /**
         * This is to show you the way to use the ADC continuous mode driver event callback.
         * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
         * However in this example, the data processing (print) is slow, so you barely block here.
         *
         * Without using this event callback (to notify this task), you can still just call
         * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        char unit[] = EXAMPLE_ADC_UNIT_STR(EXAMPLE_ADC_UNIT);

        while (1) {
            // ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
            ret = adc_continuous_read(handle, result, 4, &ret_num, 0);
            if (ret == ESP_OK) {
                // ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32" bytes", ret, ret_num);
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
                    // printf("reslut size:%d\n", sizeof(adc_digi_output_data_t));
                    uint32_t chan_num = EXAMPLE_ADC_GET_CHANNEL(p);
                    uint32_t data = EXAMPLE_ADC_GET_DATA(p);
                    adc_cali_raw_to_voltage(cali_handle, data, &voltage);
                    float humi = 0.0;
                    /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
                    if (chan_num < SOC_ADC_CHANNEL_NUM(EXAMPLE_ADC_UNIT)) {
                        // ESP_LOGI(TAG, "Raw data:%#x, data:%d, voltage:%dmv, Channel: %"PRIu32, p->val, data, voltage, chan_num);
                        humi = (float)(voltage * 244) / 10000.0;
                        ESP_LOGI(TAG, "humi:%f", humi);
                        if(!humiObj){
                            ESP_LOGW(TAG, "humiObj is none");
                        }
                        else if(humi != humiObj->curVal){
                            humiObj->curVal = humi;
                            update_ui_safe(humi_meter, update_humi, &humiObj->curVal, sizeof(humiObj->curVal));   
                        }
                    } else {
                        ESP_LOGW(TAG, "Invalid data [%s_%"PRIu32"_%"PRIx32"]", unit, chan_num, data);
                    }
                }
                /**
                 * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
                 * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
                 * usually you don't need this delay (as this task will block for a while).
                 */
            } else if (ret == ESP_ERR_TIMEOUT) {
                //We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    ESP_ERROR_CHECK(adc_continuous_stop(handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(handle));
}

int start_humidity_sensor(void *arg)
{
    humiObj = calloc(1, sizeof(humidity_t));
    if(!humiObj){
        ESP_LOGW(TAG, "Create humidity sensor failed\n");
        return -1;
    }
    xTaskCreate(continuous_read_task, "continuous_read", 4096, NULL, 5, NULL);
    return 0; 
}


static uint32_t gpio_isr_times = 0;
static void gpio_isr_handler(void* arg)
{
    static uint32_t last_time = 0;
    uint32_t now = xTaskGetTickCountFromISR();
    if (now - last_time > pdMS_TO_TICKS(20)) {  // 20ms 消抖
        if(gpio_isr_times >=5){
            gpio_isr_times = 0;
        }
        gpio_isr_times++;        
        xQueueSendFromISR(gpio_evt_queue, &gpio_isr_times, NULL);
    }
    last_time = now;    
}

uint32_t read_gpio_key_status()
{
    uint32_t recvTimes = 0;
    if (xQueueReceive(gpio_evt_queue, &recvTimes, 0)){
        ESP_LOGI(TAG, "recvTimes : %lu\n", recvTimes);
        return LV_KEY_ENTER;
    }
    return 0;
}

// 新增：定时器回调函数（用于 lv_tick_inc）
void lv_tick_timer_cb(TimerHandle_t xTimer)
{
    lv_tick_inc(1);
}

void led_key_init()
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_LOW_LEVEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(5, sizeof(uint32_t));
    // xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);    //初始化允许注册中断处理函数。必须调用此函数后才能添加中断处理程序。
    gpio_isr_handler_add(KEY_GPIO, gpio_isr_handler, NULL);
}

static esp_timer_handle_t lvgl_tick_timer = NULL;

// 定时回调函数，每 1ms 触发
static void lv_tick_task(void *arg) {
    lv_tick_inc(1);
}
 
// 初始化 LVGL Tick 定时器
void lvgl_tick_timer_init(void) {
    const esp_timer_create_args_t timer_args = {
        .callback = &lv_tick_task,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lv_tick_timer"
    };
 
    esp_timer_create(&timer_args, &lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer, 1000); // 1ms 触发
}


static void btnPwron_event_handler(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);

    if(e->code == LV_EVENT_CLICKED){
        bool checked = lv_obj_has_state(btn, LV_STATE_CHECKED);
        ESP_LOGI(TAG, "checked:%d\n", checked);
        lv_obj_align_to(label, btn, LV_ALIGN_CENTER, 0, 0);
        if(checked) {
            lv_label_set_text(label, "OFF");
            lv_obj_align_to(label, btn, LV_ALIGN_CENTER, 0, 0);
        } else {
            lv_label_set_text(label, "ON");          
        }
    }
}

void meminfo_task(void *arg)
{
    lv_mem_monitor_t mon_p;
    for(;;){
        ESP_LOGI(TAG, "ESP32 free mem: %d", esp_get_free_heap_size());
        ESP_LOGI(TAG, "--------------------------------");

        lv_mem_monitor(&mon_p);
        // ESP_LOGI(TAG, "总内存: %d\n", LV_MEM_SIZE);
        ESP_LOGI(TAG, "lv total: %d", mon_p.total_size);
        ESP_LOGI(TAG, "lv free: %d", mon_p.free_size);
        ESP_LOGI(TAG, "lv frag: %d", mon_p.frag_pct);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#define ROTARY_LEFT 0
#define ROTARY_RIGHT 1
#define ROTARY_NONE -1

//watch point isr
static bool ec11_rotary_evt_send(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_wakeup;
    // QueueHandle_t ec11_evt_queue = (QueueHandle_t)user_ctx;
    int rotary = ROTARY_NONE;
    if(edata->watch_point_value < 0){
        rotary = ROTARY_RIGHT;
    }
    else if(edata->watch_point_value > 0){
        rotary = ROTARY_LEFT; 
    }
    // send event data to ec11_evt_queue, from this interrupt callback
    xQueueSendFromISR(ec11_evt_queue, &rotary, &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);
}

void ec11_rotray_evt_handle_task(void *arg)
{
    int event = ROTARY_NONE;
    while (1) {
        if (xQueueReceive(ec11_evt_queue, &event, pdMS_TO_TICKS(1000))) {
            ESP_LOGI(TAG, "Watch point event, count: %d", event);
        } 
        // else {
        //     ESP_LOGE(TAG, "receive ec11 rotary evt err");
        // }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Build time:%s\n", __TIME__);

    smartconfig_run();

    led_key_init();

    lcd_init();

#if LV_ENABLE
    lv_init();            // LVGL 初始化
    lv_port_disp_init();  // 注册LVGL的显示任务
    lv_port_indev_init(); // 注册LVGL的触屏检测任务

    LV_LOG_WARN("测试警告日志");      // 应该输出

    my_gui_init();

    start_network_info_update();
    sntp_sync_start();
    init_ds18b20();
    start_humidity_sensor(NULL);

    xTaskCreate(meminfo_task, "meminfo", 2048, NULL, 6, NULL);

    ec11_evt_queue = xQueueCreate(15, sizeof(int));
    xTaskCreate(ec11_rotray_evt_handle_task, "ec11 rotary", 2048, NULL, 6, NULL);
    ec11_start(ec11_create(ec11_rotary_evt_send, ec11_evt_queue));

//-------------------------------------------------------------------------------------------
    TimerHandle_t lvgl_timer = xTimerCreate(
        "lv_tick",
        pdMS_TO_TICKS(1),
        pdTRUE,
        NULL,
        lv_tick_timer_cb);

    xTimerStart(lvgl_timer, 0);

    uint64_t timeCnt = 0;
    while(1) {
        process_ui_messages();
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
        timeCnt++;

        if(0 == timeCnt % 200){ //1s
            update_time(NULL, NULL);
        }

        if(0 == timeCnt % 400){ //5s
            if(base_info_scr1 == cur_scr){
                cur_scr = base_info_scr2;
            }
            else if(base_info_scr2 == cur_scr){
                cur_scr = dev_info_scr;
            }
            else if(dev_info_scr == cur_scr){
                cur_scr = base_info_scr1;
            }            
            lv_scr_load(cur_scr);
        }
    }
#endif
}
