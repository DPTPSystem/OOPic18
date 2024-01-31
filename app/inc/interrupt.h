/******************************************************
 * file name:   interrupt.h
 * title:       high and low priority interrupts
 * autor:       DPTP System
 * date:        2024.01.09.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      STM32
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F4x2 and PIC18F46k22
******************************************************/
#ifndef INTERRUPT_H
#define	INTERRUPT_H

volatile uint16_t one_us_count_u16 = 0;

void __interrupt(high_priority) hi_isr(void) 
{
	/* Itt t�rt�nik a magas priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
    if(PIR1bits.RCIF){
		if(RCSTA & 6){	// data error
			unsigned char temp; // dump
			RCSTAbits.SREN = 0;
			temp = RCREG;       // data to drop
			RCSTAbits.SREN = 1;
		}else{
//			if(bUffEr){ Buffer[BufferI] = RCREG;    // Kiolvassuk az adatot �s elt�roljuk
//			}else{ Buffer2[BufferI] = RCREG; }
//			BufferI++;
//			DataSize++;     // Adat forgalom sz�mol�sa
//			BufferSize++;
		}
		PIR1bits.RCIF = 0;
	}
}

// 1uS
void __interrupt(low_priority) lo_isr(void) 
{
	/* Itt t�rt�nik az alacsony priorit�s� megszak�t�s t�nyleges kiszolg�l�sa */
    if (INTCONbits.TMR0IF)
	{
		TMR0L = 0xF0;
        //LATDbits.LATD0 = !LATDbits.LATD0;
        one_us_count_u16++;
        INTCONbits.TMR0IF = 0;   // clear the flag
    }
}

#endif	/* INTERRUPT_H */

