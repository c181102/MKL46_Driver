/*
 * clock.h
 *
 *  Created on: Mar 2, 2024
 *      Author: ADMIN
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "MKL46Z4.h"

/*!
 * @brief Enables the clock for a specified PORT.
 *
 * @param PORTx The PORT peripheral base address (e.g., PORTA, PORTB, etc.) for which the clock is to be enabled.
 */
void CLOCK_PortEnable(PORT_Type* PORTx);

/*!
 * @brief Checks if the clock for a specified PORT is enabled.
 *
 * @param PORTx The PORT peripheral base address (e.g., PORTA, PORTB, etc.) to check the clock status for.
 * @return Returns 1 if the clock for the specified PORT is enabled, 0 otherwise.
 */
uint8_t CLOCK_CheckPortEnable(PORT_Type* PORTx);

/*!
 * @brief Enables the clock for the PIT timer.
 */
void CLOCK_PITEnable();

/*!
 * @brief Retrieves the current bus clock frequency.
 *
 * @return The frequency of the bus clock in Hz.
 */
uint32_t CLOCK_GetBusClock();

void CLOCK_ADCEnable(ADC_Type* ADCx);

void CLOCK_UART0Enable();

void CLOCK_UARTEnable(UART_Type* UARTx);

#endif /* CLOCK_H_ */
