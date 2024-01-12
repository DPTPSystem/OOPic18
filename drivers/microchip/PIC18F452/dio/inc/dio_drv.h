/******************************************************
 * file name:   dio_drv.h
 * title:       dio driver
 * autor:       DPTP System
 * date:        2024.01.09.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      PIC18
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F452
******************************************************/

#ifndef DIO_DRV_P18F452_H
#define	DIO_DRV_P18F452_H

#include "dio_if.h"

typedef enum
{
    DIO_OUTPUT = 0,
    DIO_INPUT,
}dio_direction_ten;

typedef enum
{
    DIO_STATE_INACTIVE = 0,
    DIO_STATE_ACTIVE,
}dio_state_ten;

typedef enum
{
    DIO_ACTIVE_HIGH = 0,
    DIO_ACTIVE_LOW,
}dio_active_level_ten;

typedef enum
{
    DIO_MODE_NORMAL = 0,
    DIO_MODE_ANALOG,
}dio_mode_ten;

typedef enum
{
    DIO_PULLUP_DISABLE = 0,
    DIO_PULLUP_ENABLE,
}dio_pullup_ten;

typedef enum
{
    DIO_PULLDOWN_DISABLE = 0,
    DIO_PULLDOWN_ENABLE,
}dio_pulldown_ten;

typedef enum
{
    DIO_LOW = 0,
    DIO_HIGH,  
}dio_init_state_ten;

/** @brief DIO driver error codes.
 *
 */
typedef enum
{
    SUCCESS = 0,
    WRONG_PIN_NUM,
    WRITE_INPUT_PIN,
    NOT_SUPP_PIN_MODE
} dio_err_code_tun;

/** @brief All PINs of the PORTs
 * 
 * RA0 - 19
 * RA1 - 20
 * RA2 - 21
 * RA3 - 22
 * RA4 - 23
 * RA5 - 24
 * RA6 - 31
 * 
 * RB0 - 8
 * RB1 - 9
 * RB2 - 10
 * RB3 - 11
 * RB4 - 14
 * RB5 - 15
 * RB6 - 16
 * RB7 - 17
 * 
 * RC0 - 32
 * RC1 - 35
 * RC2 - 36
 * RC3 - 37
 * RC4 - 42
 * RC5 - 43
 * RC6 - 44
 * RC7 - 1
 * 
 * RD0 - 38
 * RD1 - 39
 * RD2 - 40
 * RD3 - 41
 * RD4 - 2
 * RD5 - 3
 * RD6 - 4
 * RD7 - 5
 * 
 * RE0 - 25
 * RE1 - 26
 * RE2 - 27
 */

typedef enum
{
    // A PORT
    PORTA_0,    // RA0/AN0
    PORTA_1,    // RA1/AN1
    PORTA_2,    // RA2/AN2/VREF+
    PORTA_3,    // RA3/AN3/VREF+
    PORTA_4,    // RA4/T0CKI
    PORTA_5,    // RA5/AN4/SS/LVDIN
    PORTA_6,    // RA6/OSC2/CLKO
    PORTA_7,    // RESERVED
            
    // B PORTS       
    PORTB_0,    // RB0/INT0
    PORTB_1,    // RB1/INT1
    PORTB_2,    // RB2/INT2
    PORTB_3,    // RB3/CCP2*
    PORTB_4,    // RB4/-
    PORTB_5,    // RB5/PGM
    PORTB_6,    // RB6/PGC
    PORTB_7,    // RB7/PGD
            
    // C PORTS       
    PORTC_0,    // RC0/T1OSO/T1CKI
    PORTC_1,    // RC1/T1OSI/CCP2*
    PORTC_2,    // RC2/CCP1
    PORTC_3,    // RC3/SCK/SCL
    PORTC_4,    // RC4/SDI/SDA
    PORTC_5,    // RC5/SDO
    PORTC_6,    // RC6/TX/CK
    PORTC_7,    // RC7/RX/DT
            
    // D PORTS       
    PORTD_0,    // RD0/PSP0
    PORTD_1,    // RD1/PSP1
    PORTD_2,    // RD2/PSP2
    PORTD_3,    // RD3/PSP3
    PORTD_4,    // RD4/PSP4
    PORTD_5,    // RD5/PSP5
    PORTD_6,    // RD6/PSP6
    PORTD_7,    // RD7/PSP7
            
    // E PORTS       
    PORTE_0,    // RE0/AN5/RD
    PORTE_1,    // RE1/AN6/WR
    PORTE_2,    // RE2/AN7/CS
    PORTE_3,    // RESERVED
    PORTE_4,    // RESERVED
    PORTE_5,    // RESERVED
    PORTE_6,    // RESERVED
    PORTE_7,    // RESERVED
            
    DIO_PIN_NUMBER,
} dio_channel_ten;

#define DIO_PIN_PER_PORT_NUM (8)
#define DIO_PORTS_NUM ((DIO_PIN_NUMBER) / DIO_PIN_PER_PORT_NUM)

/** @brief DIO driver configuration structure
 *
 *  All port pins are defined as inputs after Device reset.
 *
 *  For setting up the driver, use the predefined values.
 *  Predefined values van be accessed by using the "DIO_" prefix or
 *  for pin selection, use the "dio_pins_tun" enumerator type.
 */
typedef struct
{
    uint8_t direction_u1    : 1,       
            polarity_u1     : 1,       
            mode_u1         : 2,      
            pullup_u1       : 1,      
            pulldown_u1     : 1,      
            state_u1        : 1;     

                                    /*   For open drain mode, if initial state set to 1, means that*/
                                    /*   the transistor wont be driven (so drain value wont be pulled down) */
    uint8_t  pin_u8;               /**< Associated GPIO pin number (select from the "dio_pins_tun" list) */
    uint16_t callback_time_ms_u16; /**< Callback function time in ms from where the DIO Callback will be called */
    uint16_t filter_time_ms_u16;   /**< Desired Filter/Debounce time in ms */
} dio_drv_cfg_tst;

/** @brief DIO driver structure
 *
 *  Use this structure to define DIO pins that you want to use in your application.
 *  After Initialization, you must only use the DIO interface methods and variables.
 */
typedef struct dio_drv_st
{
    dio_if_tst  super;  /**< DIO interface structure */
    dio_drv_cfg_tst cfg_st; /**< DIO driver config structure */

    uint16_t cnt_u16; /**< Filter time counter */
    int16_t        (*init)(struct dio_drv_st *self);
} dio_drv_tst;

void init_dio_drv(dio_drv_tst *dio_drv_pst);



#endif // DIO_DRV_DSPIC30F_H
