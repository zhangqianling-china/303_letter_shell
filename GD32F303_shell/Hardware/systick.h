/*!
    \file  systick.h
    \brief the header file of systick
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-23, V1.0.0, demo for GD32F30x
*/

#ifndef SYS_TICK_H
#define SYS_TICK_H

#include <stdint.h>
void delay_init(void);
void delay_xus(uint32_t xus);
void delay_xms(uint32_t xms);

///* configure systick */
//void systick_config(void);
///* delay a time in milliseconds */
//void delay_1ms(uint32_t count);
///* delay decrement */
//void delay_decrement(void);

#endif /* SYS_TICK_H */
