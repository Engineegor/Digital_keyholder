#pragma onse
#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <hal/gpio_hal.h>
#include <controls/keyPad.h>

const gpio_num_t DISPL_SCL_PIN	= GPIO_NUM_18;  //clock
const gpio_num_t DISPL_SI_PIN	= GPIO_NUM_23;  //data
const gpio_num_t DISPL_CS_PIN	= GPIO_NUM_25;  //chip select
const gpio_num_t DISPL_RS_PIN	= GPIO_NUM_27;  //dc(data/command)
const gpio_num_t DISPL_RSE_PIN	= GPIO_NUM_26;  //reset

const gpio_num_t BUTTON_INC_PIN	= GPIO_NUM_35;
const gpio_num_t BUTTON_DEC_PIN	= GPIO_NUM_34;
const gpio_num_t SWITCH_PIN		= GPIO_NUM_32;

const keypad_definition_t HWCONF_KEYPAD = {
    {GPIO_NUM_35, KEY_UP},
    {GPIO_NUM_34, KEY_DOWN},
/*	{GPIO_NUM_12, KEY_NEXT},
	{GPIO_NUM_13, KEY_PREV},/**/
    {GPIO_NUM_32, KEY_OK},
};

#endif //DEVICE_CONFIG_H