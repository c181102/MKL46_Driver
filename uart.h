/*
 * uart.h
 *
 *  Created on: Mar 9, 2024
 *      Author: DELL
 */

#ifndef UART_H_
#define UART_H_

#include "MKL46Z4.h"
#include "port.h"
#include "stdbool.h"

typedef void(*UART0_Callback)(uint8_t);

typedef enum
{
	UART_SUCCESS,
	UART_TIMEOUT,
} UART_Status_Type;

typedef enum
{
	UART_DATA_8BIT,
	UART_DATA_9BIT,
	UART_DATA_10BIT,
} UART_Data_Length_Type;

typedef enum
{
	UART_PARITY_NONE = 0U,
	UART_PARITY_EVEN = 2U,
	UART_PARITY_ODD = 3U,
} UART_Parity_Type;

typedef enum
{
	UART_SBNS_1BIT,
	UART_SBNS_2BIT,
} UART_Stop_Bit_Type;

typedef enum
{
	UART_LSB_FIRST,
	UART_MSB_FIRST,
} UART_MSB_LSB_Type;

typedef struct
{
	PORT_Type* PORT;
	uint8_t pin;
} UART_Pin_Config_Type;

typedef struct
{
	UART_Pin_Config_Type	tx;
	UART_Pin_Config_Type 	rx;
	bool 					txEnable;
	bool					rxEnable;
	uint32_t				baudrate;
	UART_Data_Length_Type	dataLength;
	UART_Stop_Bit_Type		stopBit;
	UART_Parity_Type		parity;
	UART_MSB_LSB_Type		MsbLsb;
	UART0_Callback			RxCallback;
} UART_Config_Type;

void UART0_Init(UART_Config_Type* config);
UART_Status_Type UART0_SendChar(uint8_t ch, uint32_t timeout);
UART_Status_Type UART0_SendString(uint8_t* str, uint8_t len, uint32_t timeout);
UART_Status_Type UART0_ReceiveCharBlocking(uint8_t* rxChar, uint32_t timeout);
void UART0_ReceiveCharNonBlocking();

#endif /* UART_H_ */
