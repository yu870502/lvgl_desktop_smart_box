
/**
 * @file lv_port_indev_templ.h
 *
 */

/*Copy this file as "lv_port_indev.h" and set this value to "1" to enable content*/
#if 1

#include <stdatomic.h>

#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_port_indev_init(void);

// extern lv_indev_t * indev_touchpad;
// extern lv_indev_t * indev_mouse;
// extern lv_indev_t * indev_keypad;
extern lv_indev_t * indev_encoder;
// extern lv_indev_t * indev_button;

extern atomic_int_fast32_t encoder_diff_atomic;
extern atomic_uint_fast8_t encoder_state_atomic ;

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
