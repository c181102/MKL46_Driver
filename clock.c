/*
 * clock.c
 *
 *  Created on: Mar 2, 2024
 *      Author: ADMIN
 */

#include "clock.h"

void CLOCK_PortEnable(PORT_Type* PORTx)
{
	uint8_t port = ((uint32_t)PORTx - PORTA_BASE)/(PORTB_BASE - PORTA_BASE) + SIM_SCGC5_PORTA_SHIFT;

	SIM->SCGC5 |= (1U << port);
}

uint8_t CLOCK_CheckPortEnable(PORT_Type* PORTx)
{
	uint8_t port = ((uint32_t)PORTx - PORTA_BASE)/(PORTB_BASE - PORTA_BASE) + SIM_SCGC5_PORTA_SHIFT;
	uint32_t ret = (SIM->SCGC5 & (1U << port));
	return (ret != 0);
}

void CLOCK_PITEnable()
{
	SIM->SCGC6 |= SIM_SCGC6_PIT(1U);
}

uint32_t CLOCK_GetBusClock()
{
	SystemCoreClockUpdate();
	uint32_t outdiv4 = (SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> SIM_CLKDIV1_OUTDIV4_SHIFT;
	uint32_t bus_clock = SystemCoreClock/(outdiv4+1);
	return bus_clock;
}

void CLOCK_ADCEnable(ADC_Type* ADCx)
{
	if (ADCx == ADC0)
	{
		SIM->SCGC6 |= SIM_SCGC6_ADC0(1U);
	}
}

void CLOCK_UART0Enable()
{
	SIM->SCGC4 |= SIM_SCGC4_UART0(1U);
}
