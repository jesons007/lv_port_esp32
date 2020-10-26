/* LVGL Example project
 * 
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "driver/periph_ctrl.h"

/* Littlevgl specific */
#include "lvgl/lvgl.h"

#include "nano_tft.h"

static void inline print_timer_counter(uint64_t counter_value)
{
    printf("Counter: 0x%08x%08x     %d     %.8f ms\n", (uint32_t) (counter_value >> 32),
           (uint32_t) (counter_value), (uint32_t)counter_value, (double) counter_value / 5000);
}
static void example_tg0_timer_init(int timer_idx,
                                   bool auto_reload)
{
    /* Select and initialize basic parameters of the timer */
    timer_config_t config = {
        .divider = 16,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = auto_reload,
    }; // default clock source is APB
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    
}

/*********************
 *      DEFINES
 *********************/
#define LV_TICK_PERIOD_MS 1

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
void guiTask(void);
void lvgl_first_demo_start(void);
void task1_cb(lv_task_t* task);

lv_obj_t * btn; 
/**********************
 *   APPLICATION MAIN
 **********************/
void app_main() 
{
    uint64_t task_counter_value;
    example_tg0_timer_init(TIMER_0, 0);

    Nano_tft_init();
    guiTask();

    lvgl_first_demo_start();
    lv_task_create(task1_cb,1000,LV_TASK_PRIO_MID,NULL);

    while(1)
	{
		lv_task_handler();
        vTaskDelay(1);//理论上延时1个ms，实际只有0.几个ms，数字大时才准确，1000 = 1000ms
	}
}
void task1_cb(lv_task_t* task)
{
    lv_event_send(btn, LV_EVENT_CLICKED, NULL);
}
static void lv_tick_task(void *arg) {
    (void) arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}


static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{

    Nano_tft_Fill_Color(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);
    lv_disp_flush_ready(disp_drv);
}
void guiTask(void) 
{
    lv_init();
    
//--------注册显示驱动-------------------
    static lv_color_t buf1[240*240];
    static lv_disp_buf_t disp_buf;

    lv_disp_buf_init(&disp_buf, buf1, NULL, 240*240);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush;//TODO
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
//--------注册显示驱动end----------------


    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
}

static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, NULL);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
void lvgl_first_demo_start(void)
{
    btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 100, 40);                          /*Set its size*/
    lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn, NULL);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/


	lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label1, "Hello world!");
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_align(btn, label1, LV_ALIGN_OUT_TOP_MID, 0, -10);

    lv_obj_set_state(btn,LV_STATE_FOCUSED);


    static lv_style_t obj1_style;
    lv_style_init(&obj1_style);
    lv_style_set_bg_color(&obj1_style,LV_STATE_DEFAULT,LV_COLOR_MAKE(69, 231, 47));

    lv_obj_t *obj1;
    obj1 = lv_obj_create(lv_scr_act(),NULL);
    lv_obj_add_style(obj1,LV_OBJ_PART_MAIN,&obj1_style);
    lv_obj_set_size(obj1,50,50);
    lv_obj_align(obj1, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_state(obj1,LV_STATE_FOCUSED);

}
