/*
 * sim.c
 *
 *  Created on: Feb 18, 2024
 *      Author: ADMIN
 */

#include "sim.h"
#include <stdint.h>

void SIM_SCGC5_PortClockGateControl(PORT_Type* PORTx, SIM_SCGC5_Port_Type clock)
{
	uint8_t port = 0;
	if (PORTx == PORTA)
	{
		port = 0x9U;
	}
	else if (PORTx == PORTB)
	{
		port = 0xAU;
	}
	else if (PORTx == PORTC)
	{
		port = 0xBU;
	}
	else if (PORTx == PORTD)
	{
		port = 0xCU;
	}
	else if (PORTx == PORTE)
	{
		port = 0xDU;
	}

	SIM->SCGC5 &= ~(0b11111U << 9);
	SIM->SCGC5 |= (0x01U << port);
}


