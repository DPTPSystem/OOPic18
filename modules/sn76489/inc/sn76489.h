/******************************************************
 * file name:   sn76489.h
 * autor:       DPTP System
 * date:        2024.01.03.
 * email:       don_peter@freemail.hu
 * device:      SN76489AN Driver
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F4x2 and PIC18F46k22
******************************************************/
#ifndef SN76489_H
#define SN76489_H

#include "common_types.h"
#include "dio_if.h"

// SN76489AN - MCU for GPIO specifications definition (Transferred to the structure)
// #define SN_WE(x)		HAL_GPIO_WritePin(SN_WE_GPIO_Port, SN_WE_Pin, x)    // HIGH 1 or LOW 0
// #define SN_PORT(data) 	GPIOC->ODR = data;
#define SET         1
#define RES         0
#define CH0         0b10000000
#define CH1         0b10100000
#define CH2         0b11000000
#define CH3         0b11100000
#define MUTE        0b11111
#define SHIFT(data, right, left)  ((data >> right & 0x01 ) << left)

typedef enum SN76489_STATUS
{
	SN76489_STATUS_INIT,
	SN76489_STATUS_ONLINE,
	SN76489_STATUS_OFFLINE,
    SN76489_STATUS_BUSY,
	SN76489_STATUS_UNKNOWN
}sn76489_status_ten;

typedef struct sn76489_cfg_tst
{
    dio_if_tst          *dout_relay1_pst;
    //void                (*toggle)(void);
} sn76489_cfg_tst;

typedef struct sn76489_tst
{
    /* PUBLIC */
    sn76489_cfg_tst     config_st;
    sn76489_status_ten  status_en;

    /* PRIVATE */
    void                (*write_enabled)(struct sn76489_tst *self, bool high_or_low_u8);
    void                (*write_port)(struct sn76489_tst *self, uint8_t data_u8);
    void                (*send_byte)(struct sn76489_tst *self, uint8_t byte_u8);
    void                (*psg_silence)(struct sn76489_tst *self);
}sn76489_tst;

void sn76489_init(sn76489_tst *self);
#endif //SN76489_H