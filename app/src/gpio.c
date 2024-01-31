/******************************************************
 * file name:   gpio.c
 * title:       gpio pin configurations
 * autor:       DPTP System
 * date:        2024.01.04.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      PIC18
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F4x2 and PIC18F46k22
******************************************************/

#include "gpio.h"
#include "xc.h"

// LED
dio_drv_tst led_out_st= { .cfg_st = { 
                                            .pin_u8             = PORTA_0,
                                            .direction_u1       = DIO_OUTPUT,
                                            .mode_u1            = DIO_MODE_NORMAL,
                                            .state_u1           = DIO_STATE_INACTIVE,
                                            .polarity_u1        = DIO_ACTIVE_HIGH
                                        }
                        };
// D0
dio_drv_tst d0_out_st= { .cfg_st = { 
                                            .pin_u8             = PORTD_0,
                                            .direction_u1       = DIO_OUTPUT,
                                            .mode_u1            = DIO_MODE_NORMAL,
                                            .state_u1           = DIO_STATE_INACTIVE,
                                            .polarity_u1        = DIO_ACTIVE_HIGH
                                        }
                        };

// LED
dio_if_tst *led_out_pst;

void gpio_init_pins(void)
{
    init_dio_drv(&led_out_st);
    init_dio_drv(&d0_out_st);
    
    //Initialize output interfaces

    // LED
    led_out_pst = &led_out_st.super;
    led_out_pst->callback_time_ms_u16 = 0;
    led_out_pst->filter_time_ms_u16 = 0;

    //Initialize input interfaces
   
}

//Alacsony piorítású interrupt indítása
void timer0(void){
	/*********************************************
	+	TIMER0 & INTERRUPT INIT
	+	Timer0 low interrupt
	**********************************************/
    // Számoláshoz, 8bit-es mód
	// (FOSC/4) / Prescaler / TMR0H TMR0L
    // (40M/4) / 	2		/ 1
	INTCON2bits.TMR0IP = 0;		// Timer0 megszakítás alacsony prioritású legyen
	
	T0CONbits.TMR0ON = 1;		// Timer0 enabled
	T0CONbits.T08BIT = 1;		// 8 bites mód kiválasztás
	T0CONbits.T0CS = 0;			// Meghajtás belsõ órajelrõl
	T0CONbits.T0SE = 0;			// Külsõ órajel fel- vagy lefutó élének választása
	T0CONbits.PSA = 1;			// Elõosztás bekapcsolása
	//Elõosztási arány beállítása (000 = 1:2, 001 = 1:4, 010 = 1:8, 011 = 0:16,
								// 100 = 1:32, 101 = 1:64, 110 = 1:128, 111 = 1:256)
	T0CONbits.T0PS2 = 0;		// Elõosztás 1:2 osztásra állítása
	T0CONbits.T0PS1 = 0;
	T0CONbits.T0PS0 = 0;
	INTCONbits.TMR0IE = 1;		// Timer tiltása
	TMR0L = 0xF0;
}

void init_usart(void){
	//TRISC = 0b10010000;
	TXSTA = 0b00100000;
    TXSTAbits.BRGH = 0;
	RCSTA = 0b00010000;
	// Baud rate if BRGH = 1
    // Baud Rate = FOSC / (16 * (X + 1))
    // SPBRG = 40000000 / (16* 19 + 1)) = 125000
    // X = (FOSC / Baud Rate / 16) - 1
    // Default settings 9600 baudrate
	SPBRG = 64;
	RCSTAbits.SPEN = 1;
}
