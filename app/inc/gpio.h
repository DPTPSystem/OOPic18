/******************************************************
 * file name:   gpio.h
 * title:       gpio pin, dio driver init, variables and prototypes
 * autor:       DPTP System
 * date:        2024.01.04.
 * email:       don_peter@freemail.hu
 * device:      PIC18
 * platform:    C Code, VS Code project
 * MCU:         PIC18F452
******************************************************/
#ifndef GPIO_H
#define	GPIO_H

#include "dio_drv.h"

extern dio_if_tst *led_out_pst;

void gpio_init_pins(void);
void gpio_callback(void);
void timer0(void);

#endif	/* GPIO_H */

