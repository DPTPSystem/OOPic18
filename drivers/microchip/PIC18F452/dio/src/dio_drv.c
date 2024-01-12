/******************************************************
 * file name:   dio_drv.c
 * title:       dio driver
 * autor:       DPTP System
 * date:        2024.01.09.
 * email:       don_peter[kukac]freemail[pont]hu
 * device:      PIC18
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F452
******************************************************/

#include "dio_drv.h"
#include <xc.h>

static uint8_t volatile *const dio_reg_in[DIO_PORTS_NUM] = {
    (uint8_t *)&PORTA, 
    (uint8_t *)&PORTB, 
    (uint8_t *)&PORTC, 
    (uint8_t *)&PORTD, 
    (uint8_t *)&PORTE, 
};

static uint8_t volatile *const dio_reg_dir[DIO_PORTS_NUM] = {
    (uint8_t *)&TRISA, 
    (uint8_t *)&TRISB, 
    (uint8_t *)&TRISC, 
    (uint8_t *)&TRISD, 
    (uint8_t *)&TRISE, 
};

static uint8_t volatile *const dio_reg_out[DIO_PORTS_NUM] = {
    (uint8_t *)&LATA,
    (uint8_t *)&LATB, 
    (uint8_t *)&LATC, 
    (uint8_t *)&LATD, 
    (uint8_t *)&LATE,
};

/*static uint16_t volatile *const pullup_reg_vu16[DIO_PORTS_NUM] = {
    (uint16_t *)&CNPU1,
    (uint16_t *)&CNPU2,
};

static uint16_t volatile *const adc_port_cfg_reg_vu16[DIO_PORTS_NUM] = {
    (uint16_t *)&ADPCFG,
};*/

/*const uint8_t ad_pin_lut_au8[DIO_PIN_NUMBER] = { 
    [PORTB_0] = 1, 
    [PORTB_1] = 2, 
    [PORTB_2] = 3, 
    [PORTB_3] = 4, 
    [PORTB_4] = 5, 
    [PORTB_5] = 6, 
    [PORTB_6] = 7, 
    [PORTB_7] = 8,  
    };

const uint8_t cnpu_pin_lut_au8[DIO_PIN_NUMBER] = 
{
    [PORTC_14] = 1,
    [PORTC_13] = 2,
    [PORTB_0] = 3,
    [PORTB_1] = 4,
    [PORTB_2] = 5,
    [PORTB_3] = 6,
    [PORTB_4] = 7,
    [PORTB_5] = 8,
};*/

static int16_t dio_drv_init(struct dio_drv_st *self)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst *)self;
    volatile uint8_t     number      = 0; // Port Number
    volatile uint8_t     position    = 0; // Pin Number

    number   = dio_drv_pst->cfg_st.pin_u8 / DIO_PIN_PER_PORT_NUM;
    position = dio_drv_pst->cfg_st.pin_u8 % DIO_PIN_PER_PORT_NUM;

    /*switch (dio_drv_pst->cfg_st.pullup_u1)
    {
        case DIO_PULLUP_ENABLE:
            if(cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] > 0)
            {
                if(cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] <= 16)
                {
                    *pullup_reg_vu16[0] |= (1UL << (cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 1));
                }
                else
                {
                    *pullup_reg_vu16[1] |= (1UL << (cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 17));
                }
            }
            break;
        case DIO_PULLUP_DISABLE:
            if(cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] > 0)
            {
                if(cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] <= 16)
                {
                    *pullup_reg_vu16[0] &= ~ (1UL << (cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 1));
                }
                else
                {
                    *pullup_reg_vu16[1] &= ~ (1UL << (cnpu_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 17));
                }
            }
            break;
        default:
            break;
    }*/

    switch (dio_drv_pst->cfg_st.direction_u1)
    {
        case DIO_OUTPUT:
            switch (dio_drv_pst->cfg_st.mode_u1)
            {
                case DIO_MODE_NORMAL:
                    /*if (ad_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] > 0)
                    {
                        *adc_port_cfg_reg_vu16[0] &= ~(1UL << (ad_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 1));
                    }*/
                    *dio_reg_dir[number] &= ~(1UL << (position));
                    break;
            }
            break;
        case DIO_INPUT:
            switch(dio_drv_pst->cfg_st.mode_u1)
            {
                case DIO_MODE_ANALOG:
                    /*if (ad_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] > 0)
                    {
                        *adc_port_cfg_reg_vu16[0] &= ~(1UL << (ad_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 1));
                        *dio_reg_dir[number] |= (1UL << (position));
                    }*/
                    break;
                case DIO_MODE_NORMAL:
                    /*if (ad_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] > 0)
                    {
                        *adc_port_cfg_reg_vu16[0] |= (1UL << (ad_pin_lut_au8[dio_drv_pst->cfg_st.pin_u8] - 1));
                    }*/
                    *dio_reg_dir[number] |= (1UL << (position));
                    break;
            }
            break;
        default:
            break;
    }

    switch (dio_drv_pst->cfg_st.state_u1)
    {
        case DIO_HIGH:
            switch (dio_drv_pst->cfg_st.polarity_u1)
            {
                case DIO_ACTIVE_HIGH:
                    *dio_reg_out[number] |= (1UL << (position));
                    break;
                case DIO_ACTIVE_LOW:
                    *dio_reg_out[number] &= ~(1UL << (position));
                    break;
                default:
                    break;
            }
            break;
        case DIO_LOW:
            switch (dio_drv_pst->cfg_st.polarity_u1)
            {
                case DIO_ACTIVE_HIGH:
                    *dio_reg_out[number] &= ~(1UL << (position));
                    break;
                case DIO_ACTIVE_LOW:
                    *dio_reg_out[number] |= (1UL << (position));
                    break;
                default:
                    break;
            }
        default:
            break;
    }

    return 0;
}

static void dio_drv_set(dio_if_tst *self, bool state_b)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst *)self; // Driver structure derived from interface
    dio_drv_pst->cfg_st.state_u1 = state_b;
    volatile uint8_t number   = 0; // Port Number
    volatile uint8_t position = 0; // Pin Number

    number   = dio_drv_pst->cfg_st.pin_u8 / DIO_PIN_PER_PORT_NUM;
    position = dio_drv_pst->cfg_st.pin_u8 % DIO_PIN_PER_PORT_NUM;

    switch (dio_drv_pst->cfg_st.state_u1)
    {
        case DIO_HIGH:
            switch (dio_drv_pst->cfg_st.polarity_u1)
            {
                case DIO_ACTIVE_HIGH:
                    *dio_reg_out[number] |= (1UL << (position));
                    break;
                case DIO_ACTIVE_LOW:
                    *dio_reg_out[number] &= ~(1UL << (position));
                    break;
                default:
                    break;
            }
            break;
        case DIO_LOW:
            switch (dio_drv_pst->cfg_st.polarity_u1)
            {
                case DIO_ACTIVE_HIGH:
                    *dio_reg_out[number] &= ~(1UL << (position));
                    break;
                case DIO_ACTIVE_LOW:
                    *dio_reg_out[number] |= (1UL << (position));
                    break;
                default:
                    break;
            }
        default:
            break;
    }
}

static bool dio_drv_get(dio_if_tst *self)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst *)self; // Driver structure derived from interface

    volatile uint8_t port_number  = dio_drv_pst->cfg_st.pin_u8 / DIO_PIN_PER_PORT_NUM; // Port Number
    volatile uint8_t pin_position = dio_drv_pst->cfg_st.pin_u8 % DIO_PIN_PER_PORT_NUM; // Pin Number

    volatile uint8_t port_state = *dio_reg_in[port_number];
    volatile uint8_t pin_mask   = (1UL << (pin_position));

    switch (dio_drv_pst->cfg_st.polarity_u1)
    {
        case DIO_ACTIVE_HIGH:
            return ((port_state & pin_mask) ? DIO_HIGH : DIO_LOW);
            break;
        case DIO_ACTIVE_LOW:
            return ((port_state & pin_mask) ? DIO_LOW : DIO_HIGH);
            break;
        default:
            return 0;
            break;
    }
}

static void dio_drv_callback(dio_if_tst *self)
{
    dio_drv_tst *dio_drv_pst = (dio_drv_tst *)self;

    volatile bool pin_state_b = dio_drv_pst->super.get(&dio_drv_pst->super);

    if (pin_state_b != self->filtered_state_b)
    {
        dio_drv_pst->cnt_u16 += dio_drv_pst->cfg_st.callback_time_ms_u16;

        if (dio_drv_pst->cnt_u16 >= dio_drv_pst->cfg_st.filter_time_ms_u16)
        {
            dio_drv_pst->cnt_u16   = 0;
            self->filtered_state_b = pin_state_b;
        }
    }
    else
    {
        dio_drv_pst->cnt_u16 = 0;
    }
}

void init_dio_drv(dio_drv_tst *self)
{
    // DIO interface initialization
    self->super.callback         = (void (*)(dio_if_tst *))dio_drv_callback;
    self->super.set              = (void (*)(dio_if_tst *, bool))dio_drv_set;
    self->super.get              = (bool (*)(dio_if_tst *))dio_drv_get;
    self->super.filtered_state_b = false;

    self->init = dio_drv_init;

    self->init(self);
}