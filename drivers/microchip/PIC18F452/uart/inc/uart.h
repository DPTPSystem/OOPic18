/******************************************************
 * file name:   uart.h
 * title:       uart driver
 * autor:       DPTP System
 * date:        2024.01.30.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      PIC18
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F452
******************************************************/

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "cpu_set.h"

#define BUFFER_SIZE             64
#define SPBRG_BRGH_H_X(X)       ((FOSC / X / 16) - 1)
#define SPBRG_BRGH_L_X(X)       ((FOSC / X / 64) - 1)
#define MIN_BAUD_RATE           (2441UL)     // BRGH = 0, SYNC = 0
#define MID_BAUD_RATE           (9803UL)     // BRGH = 1, SYNC = 0
#define MAX_BAUD_RATE           (2500000ULL) // BRGH = 1, SYNC = 0
#define SET_BAUD_RATE           (125000ULL)

typedef enum UART_STATE
{
    UART_INIT = 0,
    UART_READY,
    UART_ERROR
}uart_state_ten;

typedef struct uart_config_tst uart_config_tst;
struct uart_config_tst
{
    void        (*set_baud)(uint32_t baud_rate_u32);
};

typedef struct uart_tst *uart_tst;
struct uart_tst
{
    /* Public */
    uart_config_tst uart_conf_st;
    
    /* Private */
    uart_state_ten  state_en;
    uint8_t         read_data_u8;
    uint8_t         buff_u8[BUFFER_SIZE];   // data storage
    uint8_t         buff_index_u8;
    uint32_t        data_size_u32;
    uint8_t         (*read)(uart_tst self, uint8_t data_u8);
    void            (*write)(uint8_t data_u8);
    void            (*buff_clear)(uart_tst self);
};

extern volatile uart_tst uart_st;

void uart_drv_init(uart_tst self);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* UART_H */

