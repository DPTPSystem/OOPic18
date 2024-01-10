/******************************************************
 * file name:   sn76489.c
 * autor:       DPTP System
 * date:        2024.01.03.
 * email:       don_peter@freemail.hu
 * device:      SN76489AN Driver
 * platform:    C Code, MPLAB X
 * MCU:         PIC18F4x2 and PIC18F46k22
******************************************************/
#include "sn76489.h"
#include "gpio.h"

static void write_enabled(sn76489_tst *self, bool high_or_low_u8)
{
    // write enabled gpio pin setting
    if(self->status_en != SN76489_STATUS_ONLINE)
    {
        return;
    }
    // HAL_GPIO_WritePin(SN_WE_GPIO_Port, SN_WE_Pin, x)    // HIGH 1 or LOW 0
}

static void write_port(sn76489_tst *self, uint8_t data_u8)
{
    // write data
    if(self->status_en != SN76489_STATUS_ONLINE)
    {
        return;
    }
    // GPIOC->ODR = data;
}

static void send_byte(sn76489_tst *self, uint8_t byte_u8)
{
    // write data
    uint16_t temp_u16 = 0;

    if(self->status_en != SN76489_STATUS_ONLINE)
    {
        return;
    }

	temp_u16 = byte_u8>>7;
	temp_u16 |= SHIFT((uint16_t)byte_u8, 6, 1);//(byte_u8>>6&0x01)<<1;
	temp_u16 |= SHIFT((uint16_t)byte_u8, 5, 2);
	temp_u16 |= SHIFT((uint16_t)byte_u8, 4, 3);
	temp_u16 |= SHIFT((uint16_t)byte_u8, 3, 4);
	temp_u16 |= SHIFT((uint16_t)byte_u8, 2, 5);
	temp_u16 |= SHIFT((uint16_t)byte_u8, 1, 6);
	temp_u16 |= SHIFT((uint16_t)byte_u8, 0, 7);
    self->write_port(self, (uint8_t)temp_u16);
	self->write_enabled(self, RES);
	//Delay_us(11);
	self->write_enabled(self, SET);
}


// PSG all chanal reset and mute
static void psg_silence(sn76489_tst *self)
{
    if(self->status_en != SN76489_STATUS_ONLINE)
    {
        return;
    }
    self->status_en = SN76489_STATUS_BUSY;
    self->send_byte(self, CH0|MUTE);	// 0 csatonra Hangerõ néma (0x9F)
	self->send_byte(self, CH1|MUTE);	// 1 csatonra Hangerõ néma (0xBF)
	self->send_byte(self, CH2|MUTE);	// 2 csatonra Hangerõ néma (0xDF)
	self->send_byte(self, CH3|MUTE);	// 3 csatonra Hangerõ néma (0xFF)
    self->status_en = SN76489_STATUS_ONLINE;
}

void sn76489_init(sn76489_tst *self)
{
    self->psg_silence = psg_silence;
    self->send_byte = send_byte;
    self->write_enabled = write_enabled;
    self->write_port = write_port;
    self->status_en = SN76489_STATUS_ONLINE;
}