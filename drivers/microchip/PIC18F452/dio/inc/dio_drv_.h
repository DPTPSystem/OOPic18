/******************************************************
 * file name:   dio_drv.h
 * title:       dio driver
 * autor:       DPTP System
 * date:        2024.01.04.
 * email:       don_peter@freemail.hu
 * device:      PIC18
 * platform:    C Code, VS Code project
 * MCU:         PIC18F452
******************************************************/

#ifndef DIO_DRV_P18F452_H
#define	DIO_DRV_P18F452_H

#include "dio_if.h"

#define DIO_INPUT               1
#define DIO_OUTPUT              0

#define DIO_STATE_ACTIVE        1
#define DIO_STATE_INACTIVE      0

#define DIO_ACTIVE_LOW          1
#define DIO_ACTIVE_HIGH         0

#define DIO_MODE_NORMAL         0
#define DIO_MODE_OPEN_DRAIN     1

#define DIO_PULLUP_ENABLE       1
#define DIO_PULLUP_DISABLE      0

#define DIO_INIT_STATE_ON       1
#define DIO_INIT_STATE_OFF      0

#define DIO_PORT_A_PIN_NUM      7   /**< Number of Port A DIO pins */
#define DIO_PORT_B_PIN_NUM      8   /**< Number of Port B DIO pins */
#define DIO_PORT_C_PIN_NUM      8   /**< Number of Port C DIO pins */
#define DIO_PORT_D_PIN_NUM      8   /**< Number of Port D DIO pins */
#define DIO_PORT_E_PIN_NUM      3   /**< Number of Port E DIO pins */


/** @brief DIO driver error codes.
 * 
 */
typedef enum{
        SUCCESS = 0, 
        WRONG_PIN_NUM,
        WRITE_INPUT_PIN,
        NOT_SUPP_PIN_MODE
}dio_err_code_tun;


/** @brief Port pin binding to numbers for consistent driver control.
 * 
 * Port A pins from 0-6
 * Port B pins from 7-14 ((0-14)    + number of Port A pins)
 * Port C pins from 15-22 ((15-22)  + number of Port A,B pins)
 * Port D pins from 23-30 ((23-30)  + number of Port A,B,C pins)
 * Port E pins from 31-33 ((31-33)  + number of Port A,B,C,D pins)
 */
typedef enum{
            RPA0 =  0, RPA1, RPA2, RPA3, RPA4, RPA5, RPA6,
            RPB0 =  7, RPB1, RPB2, RPB3, RPB4, RPB5, RPB6, RPB7, 
            RPC0 = 15, RPC1, RPC2, RPC3, RPC4, RPC5, RPC6, RPC7,
            RPD0 = 23, RPD1, RPD2, RPD3, RPD4, RPD5, RPD6, RPD7,
            RPE0 = 31, RPE1, RPE2
}dio_pins_tun;



/** @brief DIO driver configuration structure
 *  
 *  All port pins are defined as inputs after Device reset.
 * 
 *  For setting up the driver, use the predefined values.
 *  Predefined values van be accessed by using the "DIO_" prefix or
 *  for pin selection, use the "dio_pins_tun" enumerator type.
 */
typedef struct dio_drv_cfg_tst
{
    uint8_t     direction_u1   : 1;        /**< Pin direction. 1 - in, 0 - out*/
    uint8_t     polarity_u1    : 1;        /**< 0 - Active High (not inverting), 1 - Active Low (inverting) (xor with the value) */
    uint8_t     mode_u1        : 1;        /**< 0 - normal, 1 - Open drain if TRISx is '0' */
    uint8_t     pullup_u1      : 1;        /**< 0 - Internal Pull-up Disabled, 1 - Internal Pull-up Enabled */
    uint8_t     initial_sate_u1: 1;        /**< Initial output state of the pin after init (xor-ed with the polarity),
                                         *   For open drain mode, if initial state set to 1, means that
                                         *   the transistor wont be driven (so drain value wont be pulled down) */
    uint8_t     pin_u8;                    /**< Associated GPIO pin number (select from the "dio_pins_tun" list) */
}dio_drv_cfg_tst;


/** @brief DIO driver structure
 *  
 *  Use this structure to define DIO pins that you want to use in your application.
 *  After Initialization, you must only use the DIO interface methods and variables.
 */
typedef struct dio_drv_tst
{
    dio_if_tst          dio_if_st;  /**< DIO interface structure */
    dio_drv_cfg_tst     drv_cfg_st; /**< DIO driver config structure */

    uint16_t            cnt_u16;    /**< Filter time counter */ 
}dio_drv_tst;


/** @brief Initialize GPIO pin with the given settings
 * 
 * Pull-Up enabled only for input mode and open-drain output mode
 * 
 * Open-Drain configuration:
 *      - Direction bit to output
 *      - Mode to Open-Drain
 *      - Pin number should be RB5-RB11
 * 
 * @param dio_drv_cfg_pst - DIO driver structure pointer for a given pin
 */
dio_err_code_tun dio_drv_init(dio_drv_tst *dio_drv_pst);


/** @brief DIO driver callback for input filtering
 *  
 *  Filter function for input data filter/debounce.
 * 
 *  @param dio_if_pst - DIO interface pointer for the corresponding pin
 */
void dio_drv_callback(dio_if_tst *self);


/** @brief DIO function to set the desired output data on the corresponding pin
 * 
 * @param dio_if_pst - DIO interface pointer for the corresponding pin
 * @param state_b - State to set on the given pin
 */
void dio_drv_set(dio_if_tst *self, bool state_b);


/** @brief Read GPIO pin current pin (pad) value
 * 
 * @param dio_if_pst - DIO interface pointer for the corresponding pin
 * @return - Read actual pin value.
 */
bool dio_drv_get(dio_if_tst *self);

#endif	/* DIO_DRV_P18F452_H */

