/*
 * gpio.c
 *
 *  Created on: Feb 21, 2024
 *      Author: DELL
 */

#include "gpio.h"
#include <assert.h>

void GPIO_Init(GPIO_Type* GPIOx, uint8_t pin, GPIO_Config_Type* config)
{
	assert(IS_GPIO(GPIOx));
	assert(IS_GPIO_PIN(pin));

	GPIOx->PDDR &= ~GPIO_PDDR_PDD_MASK;

	if (config->direction != GPIO_PDDR_INPUT)
	{
		assert(IS_GPIO_PIN_VALUE(config->initial_value));
		assert(IS_GPIO_DIRECTION(config->direction));

		GPIOx->PDDR = (config->direction << pin);
		GPIOx->PDOR = (config->initial_value << pin);
	}
}

void GPIO_WritePin(GPIO_Type* GPIOx, uint8_t pin, GPIO_PinValue value)
{
	assert(IS_GPIO(GPIOx));
	assert(IS_GPIO_PIN(pin));
	assert(IS_GPIO_PIN_VALUE(value));

	GPIOx->PDOR |= (value << pin);
}

GPIO_PinValue GPIO_ReadPin(GPIO_Type* GPIOx, uint8_t pin)
{
	assert(IS_GPIO(GPIOx));
	assert(IS_GPIO_PIN(pin));

	return (GPIOx->PDIR >> pin) & 0x1U;
}

void GPIO_ClearPin(GPIO_Type* GPIOx, uint8_t pin)
{
	assert(IS_GPIO(GPIOx));
	assert(IS_GPIO_PIN(pin));

	GPIOx->PCOR = (1U << pin);
}

void GPIO_SetPin(GPIO_Type* GPIOx, uint8_t pin)
{
	assert(IS_GPIO(GPIOx));
	assert(IS_GPIO_PIN(pin));

	GPIOx->PSOR = (1U << pin);
}

void GPIO_TogglePin(GPIO_Type* GPIOx, uint8_t pin)
{
	assert(IS_GPIO(GPIOx));
	assert(IS_GPIO_PIN(pin));

	GPIOx->PTOR = (1U << pin);
}
