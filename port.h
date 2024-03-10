/*
 * port.h
 *
 *  Created on: Feb 18, 2024
 *      Author: ADMIN
 */

#ifndef PORT_H_
#define PORT_H_

#include "MKL46Z4.h"

typedef enum
{
	PORT_PULL_DISABLE 	= 0U,
	PORT_PULL_DOWN 		= 2U,
	PORT_PULL_UP 		= 3U,
} PORT_PULL_Type;

typedef enum
{
	PORT_MUX_ANALOG,
	PORT_MUX_GPIO,
	PORT_MUX_ALT2,
	PORT_MUX_ALT3,
	PORT_MUX_ALT4,
	PORT_MUX_ALT5,
	PORT_MUX_ALT6,
	PORT_MUX_ALT7,
} PORT_MUX_Type;

typedef enum
{
	PORT_IRQC_DISABLE		= 0U,
	PORT_IRQC_DMA_RISING	= 1U,
	PORT_IRQC_DMA_FALLING	= 2U,
	PORT_IRQC_BOTH_EDGE		= 3U,
	PORT_IRQC_INT_LOW		= 8U,
	PORT_IRQC_INT_RISING	= 9U,
	PORT_IRQC_INT_FALLING	= 10U,
	PORT_IRQC_INT_BOTH_EDGE	= 11U,
	PORT_IRQC_INT_HIGH		= 12U,
} PORT_IRQC_Type;

typedef void(*PORT_Callback)(uint8_t);

typedef struct
{
	PORT_PULL_Type 	pull;
	PORT_MUX_Type 	mux;
	PORT_IRQC_Type 	irqc;
	PORT_Callback	pCallback;
} PORT_Config_Type;

/*!
 * @brief Initializes a specific PORT pin with the given configuration.
 *
 * @param PORTx The PORT peripheral base address (e.g., PORTA, PORTB, etc.).
 * @param pin The specific pin number within the PORT to configure.
 * @param config Pointer to the configuration structure containing the desired settings for the pin.
 */
void PORT_Init(PORT_Type* PORTx, uint8_t pin, PORT_Config_Type* config);

#endif /* PORT_H_ */
