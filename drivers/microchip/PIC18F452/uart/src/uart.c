/******************************************************
 * file name:   uart.c
 * title:       uart driver
 * autor:       DPTP System
 * date:        2024.01.30.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      PIC18
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F452
******************************************************/

#include "uart.h"

void uart_status(uart_tst self, uart_state_ten state_en)
{
    self->state_en = state_en;
}

void set_baud(uint32_t baud_rate_u32)
{
    // if baud rate is too low
    if(baud_rate_u32 < MID_BAUD_RATE && baud_rate_u32 >= MIN_BAUD_RATE)
    {
        TXSTAbits.BRGH = 0;
        SPBRG = (uint8_t)SPBRG_BRGH_L_X(baud_rate_u32);
    }
    else if(baud_rate_u32 >= MID_BAUD_RATE && baud_rate_u32 <= MAX_BAUD_RATE)
    {   // if baud rate is middle or high rate
        TXSTAbits.BRGH = 1;
        SPBRG = (uint8_t)SPBRG_BRGH_H_X(baud_rate_u32);
    }
    else    // if not added baudrate or data is faulty, setting default baud rate
    {
        SPBRG = (uint8_t)SPBRG_BRGH_L_X(MIN_BAUD_RATE);
//        uart_status(&self, UART_ERROR);
    }
}

void write_uart(uint8_t data_u8){
	TXREG = data_u8;
	while(!PIR1bits.TXIF);
}

void read_uart(uart_tst self){
	RCSTAbits.CREN = 1;
	while(!PIR1bits.RCIF);
    self->read_data_u8 = RCREG;
}

void buff_clear(uart_tst self)
{
    // delete buffer
//    memset(self->buff_u8, '\0', sizeof(self->buff_u8));
    for(uint8_t i = 0; i < BUFFER_SIZE; ++i)
    {
        self->buff_u8[i] = '\0';
    }
}

void uart_drv_init(uart_tst self)
{
    self->buff_index_u8 = 0;
    self->data_size_u32 = 0;
    self->read_data_u8 = 0;
    self->buff_clear = buff_clear;
    self->read = read_uart;
    self->write = write_uart;
    self->uart_conf_st.set_baud = set_baud;
    self->state_en = UART_READY;
    
    self->uart_conf_st.set_baud(SET_BAUD_RATE);
    self->buff_clear(&self);
}