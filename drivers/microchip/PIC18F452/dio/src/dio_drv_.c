/**
 * @file DigitIO.c
 * @author Peter Toth
 * @date 2024.01.05.
 * @brief DIO driver for some PIC18 device
 * 
 * Supported devices:
 *      PIC18F442
 *      PIC18F452
 *      PIC18F46K22
   
 */
#include <xc.h>
#include <stddef.h>
#include "dio_if.h"
#include "dio_drv.h"

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
const uint8_t rab_cnpu_lut_au8[DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM + DIO_PORT_E_PIN_NUM] =
                                                            {
                                                            19, 20, 21, 22, 23, 24, 31,     // A PORT
                                                            8, 9, 10, 11, 14, 15, 16, 17,   // B PORT  
                                                            32, 35, 36, 37, 42, 43, 44, 1,  // C PORT
                                                            38, 39, 40, 41, 2, 3, 4, 5,     // D PORT
                                                            25, 26, 27                      // E PORT
                                                            };

dio_err_code_tun dio_drv_init(dio_drv_tst *dio_drv_pst)
{
    dio_err_code_tun return_code_un = SUCCESS;

    uint16_t pin_tmp_u16 = 1; /**< Stores the selected pin by the user */
    uint16_t cn_pin_temp_u16 = 1; /**< Stores the corresponding CN pin for Pull-up */
    uint8_t init_state_temp_u8 = 0;

    volatile uint16_t *value_reg_vu16; //LATx
    volatile uint16_t *direction_reg_vu16; //TRISx
    volatile uint16_t *mode_reg_vu16; //ODCx
    volatile uint16_t *pullup_reg_vu16; //CNPU1,CNPU2

    //DIO interface initialization
    dio_drv_pst->dio_if_st.callback = dio_drv_callback;
    dio_drv_pst->dio_if_st.set = dio_drv_set;
    dio_drv_pst->dio_if_st.get = dio_drv_get;
    dio_drv_pst->dio_if_st.filtered_state_b = false;
    //DIO driver variable initialization
    dio_drv_pst->cnt_u16 = 0;

    //Select Direction register and corresponding pin
    if(dio_drv_pst->drv_cfg_st.pin_u8 < DIO_PORT_A_PIN_NUM)
    {
        pin_tmp_u16 = (pin_tmp_u16 << dio_drv_pst->drv_cfg_st.pin_u8);
        direction_reg_vu16 = &TRISA;
        value_reg_vu16 = &LATA;
    }
    //Port B pins from 7-15
    else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM)) &&
            (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM)))
    {
        pin_tmp_u16 = (pin_tmp_u16 << (dio_drv_pst->drv_cfg_st.pin_u8 - DIO_PORT_A_PIN_NUM));
        direction_reg_vu16 = &TRISB;
        value_reg_vu16 = &LATB;
    }
    //Port C pins from 16-23
    else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM)) &&
            (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM)))
    {
        pin_tmp_u16 = (pin_tmp_u16 << (dio_drv_pst->drv_cfg_st.pin_u8 - (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM)));
        direction_reg_vu16 = &TRISC;
        value_reg_vu16 = &LATC;
    }
    //Port D pins from 24-31
    else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM)) &&
            (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM)))
    {
        pin_tmp_u16 = (pin_tmp_u16 << (dio_drv_pst->drv_cfg_st.pin_u8 - (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM)));
        direction_reg_vu16 = &TRISD;
        value_reg_vu16 = &LATD;
    }
    //Port E pins from 32-34
    else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM)) &&
            (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM + DIO_PORT_E_PIN_NUM)))
    {
        pin_tmp_u16 = (pin_tmp_u16 << (dio_drv_pst->drv_cfg_st.pin_u8 - (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM)));
        direction_reg_vu16 = &TRISE;
        value_reg_vu16 = &LATE;
    }

    //Set Registers
    if(dio_drv_pst->drv_cfg_st.direction_u1 == DIO_OUTPUT)
    {
        //Set mode for OC capable pins if pin set to output
        if((mode_reg_vu16 != 0) && (dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN))
        {
            *mode_reg_vu16 |= pin_tmp_u16; //set OD register bit to 1
            *value_reg_vu16 &= ~(pin_tmp_u16); //Set output latch to 0 as TRISx will control the OD
            value_reg_vu16 = &TRISB; //Change value register to TRISB
            *value_reg_vu16 |= pin_tmp_u16; //Disable pull down transistor
        }
        else if((mode_reg_vu16 != 0) && (dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_NORMAL))
        {
            *mode_reg_vu16 &= ~(pin_tmp_u16); //Clear OD bit if pin is capable
            *direction_reg_vu16 &= ~(pin_tmp_u16); //clear bit
        }
        else if((mode_reg_vu16 == 0) && (dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_NORMAL))
        {
            *direction_reg_vu16 &= ~(pin_tmp_u16); //clear bit
        }
    }
    else if(dio_drv_pst->drv_cfg_st.direction_u1 == DIO_INPUT)
    {
        *direction_reg_vu16 |= pin_tmp_u16; //set bit
        dio_drv_pst->drv_cfg_st.mode_u1 = DIO_MODE_NORMAL; //If pin is not capable of OD then set it to normal mode
    }

    //Set pull-up if enabled and direction is input or out-OD
    if((dio_drv_pst->drv_cfg_st.direction_u1 == DIO_INPUT) ||
            (dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN && (mode_reg_vu16 != 0)))
    {
        if(dio_drv_pst->drv_cfg_st.pullup_u1 == DIO_PULLUP_ENABLE)
        {
            *pullup_reg_vu16 |= cn_pin_temp_u16;
        }
        else
        {
            *pullup_reg_vu16 &= ~(cn_pin_temp_u16);
        }
    }
    else
    {
        *pullup_reg_vu16 &= ~(cn_pin_temp_u16); //Disable pull-up
    }

    //Set initial output state
    if(dio_drv_pst->drv_cfg_st.direction_u1 == DIO_OUTPUT)
    {
        //XOR the initial state with the polarity, so in Active Low mode, the
        //output data will be negated
        init_state_temp_u8 = (dio_drv_pst->drv_cfg_st.initial_sate_u1 ^ dio_drv_pst->drv_cfg_st.polarity_u1);

        //If mode is Open drain then state is set with TRIS
        //TRIS = 0, Transistor pulls down
        //TRIS = 1, Transistor not controlled
        //To set OD pin high, do not drive the transistor so set TRIS to 1
        //In normal mode, set latch to 1 for high, 0 for low.
        if(init_state_temp_u8 == DIO_INIT_STATE_ON)
        {
            *value_reg_vu16 |= pin_tmp_u16; // Set register bit
        }
        else
        {
            *value_reg_vu16 &= ~(pin_tmp_u16); // Clear register bit
        }
    }

    return return_code_un;
}

void dio_drv_callback(dio_if_tst *self)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst*) self; //Driver structure derived from interface

    uint16_t port_tmp_u16 = 0; //LATx
    uint16_t pin_rshift_u16 = 0;


    if(dio_drv_pst->drv_cfg_st.pin_u8 < DIO_PORT_A_PIN_NUM) //PORTA (0-4)
    {
        if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_NORMAL)
        {
            port_tmp_u16 = PORTA;
            pin_rshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8;
        }
        else
        {
            return; //Pin does not support OD mode
        }
    }
    else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM)) &&
            (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM))) //PORTB pin (5-20)
    {
        //OD mode is not supported for every pin
        if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN)
        {
            if((dio_drv_pst->drv_cfg_st.pin_u8 > 9) && (dio_drv_pst->drv_cfg_st.pin_u8 < 17))
            {
                port_tmp_u16 = PORTB;
            }
            else
            {
                return; //Not valid pin number for OD mode
            }
        }
        else//Normal mode is supported for every pin
        {
            port_tmp_u16 = PORTB;
        }
        
        pin_rshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8 - DIO_PORT_A_PIN_NUM;
    }

    port_tmp_u16 >>= pin_rshift_u16;
    port_tmp_u16 &= 1;

    //Calculate value based on polarity
    port_tmp_u16 ^= dio_drv_pst->drv_cfg_st.polarity_u1;

    if((bool) port_tmp_u16 != self->filtered_state_b)
    {
        dio_drv_pst->cnt_u16 += self->callback_time_ms_u16;

        if(dio_drv_pst->cnt_u16 >= self->filter_time_ms_u16)
        {
            dio_drv_pst->cnt_u16 = 0;
            self->filtered_state_b = (bool)port_tmp_u16;
        }
    }
    else
    {
        dio_drv_pst->cnt_u16 = 0;
    }
}

void dio_drv_set(dio_if_tst *self, bool state_b)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst*) self; //Driver structure derived from interface

    if(dio_drv_pst->drv_cfg_st.direction_u1 == DIO_OUTPUT)
    {
        uint8_t state_temp_u8 = 0;
        uint16_t port_pin_tmp_u16 = 1;
        uint16_t pin_lshift_u16 = 0;
        volatile uint16_t* value_reg_pvu16;
        
        //PORTA
        if(dio_drv_pst->drv_cfg_st.pin_u8 < DIO_PORT_A_PIN_NUM)
        {
            if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN)
            {
                return; //Port A pins does not support open drain mode.
            }
            else
            {
                value_reg_pvu16 = &LATA;
            }

            pin_lshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8;
        }
        //GPIOB pin
        else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM)) &&
                (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM)))
        {
            if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN)
            {
                return; //Port B pins does not support open drain mode.
            }
            else
            {
                value_reg_pvu16 = &LATB;
            }

            pin_lshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8 - DIO_PORT_A_PIN_NUM;
        }
        //GPIOC pin
        else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM)) &&
                (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM)))
        {
            if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN)
            {
                return; //Port C pins does not support open drain mode.
            }
            else
            {
                value_reg_pvu16 = &LATC;
            }

            pin_lshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8 - (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM);
        }
        //GPIOD pin
        else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM)) &&
                (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM)))
        {
            if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN)
            {
                return; //Port D pins does not support open drain mode.
            }
            else
            {
                value_reg_pvu16 = &LATD;
            }

            pin_lshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8 - (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM);
        }
        //GPIOE pin
        else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM)) &&
                (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM + DIO_PORT_E_PIN_NUM)))
        {
            if(dio_drv_pst->drv_cfg_st.mode_u1 == DIO_MODE_OPEN_DRAIN)
            {
                return; //Port E pins does not support open drain mode.
            }
            else
            {
                value_reg_pvu16 = &LATE;
            }

            pin_lshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8 - (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM + DIO_PORT_C_PIN_NUM + DIO_PORT_D_PIN_NUM);
        }
        else
        {
            // In case of invalid pin value no register shall be modified therefore the best is to
            // return from the function
            return;
        }

        //Select port pin register bit
        port_pin_tmp_u16 <<= pin_lshift_u16;

        //Calculate value with polarity
        state_temp_u8 = ((uint8_t) state_b ^ dio_drv_pst->drv_cfg_st.polarity_u1);

        //Set or clear register bit
        if(state_temp_u8 == 0)
        {
            *value_reg_pvu16 &= ~(port_pin_tmp_u16); // Clear bit
        }
        else
        {
            *value_reg_pvu16 |= port_pin_tmp_u16; //Set bit
        }
    }
    else
    {
        return; //Trying to write input pins
    }

}

bool dio_drv_get(dio_if_tst *self)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst*) self; //Driver structure derived from interface

    uint16_t port_tmp_u16 = 0; //PORTx
    uint16_t pin_rshift_u16 = 0;

    if(dio_drv_pst->drv_cfg_st.pin_u8 < DIO_PORT_A_PIN_NUM) //PORTA (0-4)
    {
        port_tmp_u16 = PORTA;
        pin_rshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8;
    }
    else if((dio_drv_pst->drv_cfg_st.pin_u8 >= (DIO_PORT_A_PIN_NUM)) &&
            (dio_drv_pst->drv_cfg_st.pin_u8 < (DIO_PORT_A_PIN_NUM + DIO_PORT_B_PIN_NUM))) //PORTB pin (5-20)
    {
        port_tmp_u16 = PORTB;
        pin_rshift_u16 = dio_drv_pst->drv_cfg_st.pin_u8 - DIO_PORT_A_PIN_NUM;
    }

    port_tmp_u16 >>= pin_rshift_u16;
    port_tmp_u16 &= 1;

    //Calculate value based on polarity
    port_tmp_u16 ^= dio_drv_pst->drv_cfg_st.polarity_u1;

    return port_tmp_u16;
}

