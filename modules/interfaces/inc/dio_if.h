/**
 * @file dio_if.h
 * @author Zsolt Papp
 * @date 2020.07.23.
 * @brief Interface for Digital IO pins
 *
 */
#ifndef DIO_IF_H
#define	DIO_IF_H

#include "common_types.h"

/** @brief Digital IO interface
 *  
 * filtered_state_b - Filtered input value. This value changes only if the input
 *                    data value remains unchanged for the duration of the defined 
 *                    filter_time_u16.
 * 
 * callback - Callback function with the given time period, that is given in
 *            the driver configuration with the callback_time_ms_u16 variable.
 * 
 * set - Function to set the given value on the given DIO pin.
 * 
 * get - Function to read the current value (not filtered) on the given pin pad.
 */
typedef struct dio_if_tst
{
    void        (*callback)(struct dio_if_tst *self); /**< Pointer to digit in filter/debounce function */
    void        (*set)(struct dio_if_tst *self, bool state_b); /**< Pointer to Output set function */
    bool        (*get)(struct dio_if_tst *self); /**< Pointer to a function that reads the current value of the input pin(pad) value */
    
    uint16_t    callback_time_ms_u16;      /**< Callback function time in ms from where the DIO Callback will be called */
    uint16_t    filter_time_ms_u16;        /**< Desired Filter/Debounce time in ms */
    bool        filtered_state_b;          /**< Filtered pin state (Boolean) */
} dio_if_tst;


#endif	/* DIO_IF_H END */