/*
 * sim.h
 *
 *  Created on: Feb 18, 2024
 *      Author: ADMIN
 */

#ifndef SIM_H_
#define SIM_H_

#include <MKL46Z4.h>

typedef enum
{
	SIM_SCGC5_PORT_CLOCK_DISABLE,
	SIM_SCGC5_PORT_CLOCK_ENABLE,
} SIM_SCGC5_Port_Type;

void SIM_SCGC5_PortClockGateControl(PORT_Type* PORTx, SIM_SCGC5_Port_Type clock);

#endif /* SIM_H_ */
