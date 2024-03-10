/*
 * gpio.h
 *
 *  Created on: Feb 21, 2024
 *      Author: DELL
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "MKL46Z4.h"

typedef enum
{
	GPIO_PDDR_INPUT,
	GPIO_PDDR_OUTPUT,
} GPIO_PDDR_Type;

typedef enum
{
	GPIO_PIN_LOW	= 0U,
	GPIO_PIN_HIGH	= 1U,
} GPIO_PinValue;

typedef struct
{
	GPIO_PDDR_Type direction;
	GPIO_PinValue initial_value;
} GPIO_Config_Type;

#define IS_GPIO(GPIO)		((GPIO) == GPIOA ||	\
							 (GPIO) == GPIOB ||	\
							 (GPIO) == GPIOC ||	\
							 (GPIO) == GPIOD ||	\
							 (GPIO) == GPIOE  ) ? 1 : 0
#define IS_GPIO_PIN(PIN)				((PIN) >= 0 && (PIN) < 31) ? 1 : 0
#define IS_GPIO_PIN_VALUE(VAL)			((VAL) == GPIO_PIN_LOW || (VAL) == GPIO_PIN_HIGH) ? 1 : 0
#define IS_GPIO_DIRECTION(DIRECTION)	((DIRECTION) == GPIO_PDDR_INPUT || (DIRECTION) == GPIO_PDDR_OUTPUT)	? 1 : 0

/*!
 * @brief Initializes a GPIO pin with the specified configuration.
 *
 * @param GPIOx The GPIO port base address (e.g., GPIOA, GPIOB).
 * @param pin The pin number within the port to initialize.
 * @param config Pointer to the configuration structure specifying how the pin should be configured.
 */
void GPIO_Init(GPIO_Type* GPIOx, uint8_t pin, GPIO_Config_Type* config);

/*!
 * @brief Writes a value to a GPIO pin.
 *
 * @param GPIOx The GPIO port base address.
 * @param pin The pin number within the port to set.
 * @param value The value to write to the pin (GPIO_PIN_LOW or GPIO_PIN_HIGH).
 */
void GPIO_WritePin(GPIO_Type* GPIOx, uint8_t pin, GPIO_PinValue value);

/*!
 * @brief Reads the current value of a GPIO pin.
 *
 * @param GPIOx The GPIO port base address.
 * @param pin The pin number within the port to read.
 * @return The current value of the GPIO pin.
 */
GPIO_PinValue GPIO_ReadPin(GPIO_Type* GPIOx, uint8_t pin);

/*!
 * @brief Clears a GPIO pin.
 *
 * @param GPIOx The GPIO port base address.
 * @param pin The pin number within the port to clear.
 */
void GPIO_ClearPin(GPIO_Type* GPIOx, uint8_t pin);

/*!
 * @brief Sets a GPIO pin.
 *
 * @param GPIOx The GPIO port base address.
 * @param pin The pin number within the port to set.
 */
void GPIO_SetPin(GPIO_Type* GPIOx, uint8_t pin);

/*!
 * @brief Toggles the current state of a GPIO pin.
 *
 * @param GPIOx The GPIO port base address.
 * @param pin The pin number within the port to toggle.
 */
void GPIO_TogglePin(GPIO_Type* GPIOx, uint8_t pin);

#endif /* GPIO_H_ */
