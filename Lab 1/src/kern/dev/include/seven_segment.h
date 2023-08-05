#ifndef __SEVEN_SEGMENT_H
#define __SEVEN_SEGMENT_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <stdarg.h>
#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>

    void led_on(uint8_t a);
    void led_off();
    void select_leds(uint8_t a);
    void led_init();

#ifdef __cplusplus
}
#endif
#endif
