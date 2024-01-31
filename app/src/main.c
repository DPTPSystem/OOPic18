/******************************************************
 * file name:   main.c
 * title:       main program, VGM Player
 * autor:       DPTP System
 * date:        2024.01.04.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      PIC18
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F4x2 and PIC18F46k22
******************************************************/

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "main.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
volatile uart_tst uart_st;
//sn76489_tst sn76489_st;

//------------------------------------------------------------------------------
// Interrupt functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Peripheral and other pin settings
//------------------------------------------------------------------------------

 void led_toggle()
{
    static uint8_t i;
    if(i++ % 2)
    {
        led_out_pst->set(led_out_pst, 0); 

    }
    else
    {
        led_out_pst->set(led_out_pst, 1);
    }             
}

void setup_pins()
{
    // Digit in/out //
    TRISA = 0b00000010;			
	TRISB = 0b00000000;		
	TRISC = 0b10010000;		
	TRISD = 0;
	TRISE = 0;
    
    // See gpio driver (gpio.h and gpio.c)
    gpio_init_pins();

	INTCON = 1;					// A interrupt tiltása

	RCONbits.IPEN = 1;			// Kétszintû megszakítási mód beállítása
	INTCONbits.GIEH = 1;		// A magas prioritású interrupt
    INTCONbits.GIEL = 1;		// Az alacsony prioritású interrupt
	INTCONbits.GIE = 1;			// Megszakítások engedélyezése
    
    PIE1bits.RCIE = 1;			// USART Receive Interrupt Enable bit
	IPR1bits.RCIP = 1;			// USART Receive Interrupt Priority bit
	
	PIE1bits.TXIE = 0;			// USART Transmit Interrupt disabled bit
	IPR1bits.TXIP = 0;			// USART Transmit Interrupt Priority bit

	ADCON1 = 0x07;				// Minden digitális
	ADCON0 = 0;

	CCP1CON = 0;				// Coperátorok kikapcsolva
    
    init_usart();
}


//******************************************************************************
// MAIN Function
//******************************************************************************
int main()
{
  
    // All pin setting
    setup_pins();
    
    // uart driver init
    uart_drv_init(&uart_st);
    
    //sn76489_init(&sn76489_st);

    // As long as everything is fine, the LED does not light up
    led_out_pst->set(led_out_pst, 0); 
    
    timer0();   // timer enabled (1uS))

    while(1)
    {
        // If 1ms
        if(one_us_count_u16 >= 1000)
        {
            one_us_count_u16 = 0;
            led_toggle();
        }
    }
        
}