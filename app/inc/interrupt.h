/******************************************************
 * file name:   interrupt.h
 * title:       high and low priority interrupts
 * autor:       DPTP System
 * date:        2024.01.09.
 * email:       don_peter@freemail.hu
 * device:      STM32
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F4x2 and PIC18F46k22
******************************************************/
#ifndef INTERRUPT_H
#define	INTERRUPT_H

void __interrupt(high_priority) hi_isr(void) 
{
	/* Itt t�rt�nik az alacsony priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
}

void __interrupt(low_priority) lo_isr(void) 
{
	/* Itt t�rt�nik az alacsony priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
    if (INTCONbits.TMR0IF)
	{
		TMR0L = 0xF0;
        LATDbits.LATD0 = !LATDbits.LATD0;
        INTCONbits.TMR0IF = 0;   // clear the flag
    }
}

#endif	/* INTERRUPT_H */

