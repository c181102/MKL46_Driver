/*
 * uart.c
 *
 *  Created on: Mar 9, 2024
 *      Author: DELL
 */

#include "uart.h"
#include "assert.h"
#include "port.h"
#include "clock.h"
#include "stddef.h"

static UART0_Callback RxCompleteCallback = NULL;
static UART0_Callback TxCompleteCallback = NULL;
static volatile uint8_t rxData;

static inline void UART0_RxEnable(bool enable)
{
	if (enable)
	{
		UART0->C2 |= UART0_C2_RE(1U);
	}
	else
	{
		UART0->C2 &= ~UART0_C2_RE_MASK;
	}
}

static inline void UART0_TxEnable(bool enable)
{
	if (enable)
	{
		UART0->C2 |= UART0_C2_TE(1U);
	}
	else
	{
		UART0->C2 &= ~UART0_C2_TE_MASK;
	}
}

void UART0_Init(UART_Config_Type* config)
{
	CLOCK_UART0Enable();

	PORT_Config_Type port_conf = {
			.pull = PORT_PULL_UP,
	};

	if (config->txEnable)
	{
		if (config->tx.PORT == PORTE && config->tx.pin == 20U)
		{
			port_conf.mux = PORT_MUX_ALT4;
		}
		else if (config->tx.PORT == PORTA && config->tx.pin == 2U)
		{
			port_conf.mux = PORT_MUX_ALT2;
		}
		else if (config->tx.PORT == PORTA && config->tx.pin == 14U)
		{
			port_conf.mux = PORT_MUX_ALT3;
		}
		else if (config->tx.PORT == PORTB && config->tx.pin == 17U)
		{
			port_conf.mux = PORT_MUX_ALT3;
		}
		else if (config->tx.PORT == PORTD && config->tx.pin == 7U)
		{
			port_conf.mux = PORT_MUX_ALT3;
		}

		PORT_Init(config->tx.PORT, config->tx.pin, &port_conf);
	}

	if (config->rxEnable)
	{
		if (config->rx.PORT == PORTE && config->rx.pin == 21U)
		{
			port_conf.mux = PORT_MUX_ALT4;
		}
		else if (config->rx.PORT == PORTA && config->rx.pin == 1U)
		{
			port_conf.mux = PORT_MUX_ALT2;
		}
		else if (config->rx.PORT == PORTA && config->rx.pin == 15U)
		{
			port_conf.mux = PORT_MUX_ALT3;
		}
		else if (config->rx.PORT == PORTB && config->rx.pin == 16U)
		{
			port_conf.mux = PORT_MUX_ALT3;
		}
		else if (config->rx.PORT == PORTD && config->rx.pin == 6U)
		{
			port_conf.mux = PORT_MUX_ALT3;
		}

		PORT_Init(config->rx.PORT, config->rx.pin, &port_conf);
	}

	UART0_RxEnable(false);
	UART0_TxEnable(false);

	MCG->C2 |= MCG_C2_IRCS(1U);				// fast
	MCG->SC |= MCG_SC_FCRDIV(1U);			// 4M / 2 = 2M
	MCG->C1 |= MCG_C1_IRCLKEN(1U);			// MCGIRCLK active
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(3U);	// MCGIRCLK - Fast clk

	uint16_t sbr = 2000000/(float)(config->baudrate)/4;

	/* SBR */
	UART0->BDL = sbr & 0xFF;
	UART0->BDH = (sbr & 0x1FFFU) >> 8U;

	/* OSR */
	UART0->C4 = (UART0->C4 & ~UART0_C4_OSR_MASK)| UART0_C4_OSR(3U);
	UART0->C5 |= UART0_C5_BOTHEDGE(1U); // both edge

	if (config->dataLength != UART_DATA_10BIT)
	{
		UART0->C4 &= ~UART0_C4_M10_MASK;
		UART0->C1 &= ~UART0_C1_M_MASK;
		UART0->C1 |= UART0_C1_M(config->dataLength);
	}
	else
	{
		UART0->C4 |= UART0_C4_M10(1U);
	}

	if (config->parity != UART_PARITY_NONE)
	{
		UART0->C1 |= UART0_C1_PE(1U);
		UART0->C1 |= UART0_C1_PT(config->parity & 1U);
	}

	if (config->stopBit != UART_SBNS_1BIT)
	{
		UART0->BDH |= UART_BDH_SBNS(1U);
	}

	if (config->MsbLsb != UART_LSB_FIRST)
	{
		UART0->S2 |= UART0_S2_MSBF(1U);
	}

	RxCompleteCallback = config->RxCallback;
}

UART_Status_Type UART0_SendChar(uint8_t ch, uint32_t timeout)
{
	UART0_TxEnable(true);

	UART_Status_Type status = UART_TIMEOUT;

	if (timeout == 0)
	{
		while((UART0->S1 & UART0_S1_TDRE_MASK) == 0);
		UART0->D = ch;
		while((UART0->S1 & UART0_S1_TC_MASK) == 0);
		status = UART_SUCCESS;
	}
	else
	{
		while((UART0->S1 & UART0_S1_TDRE_MASK) == 0U && --timeout);
		if ((UART0->S1 & UART0_S1_TDRE_MASK) != 0U)
		{
			UART0->D = ch;
			while((UART0->S1 & UART0_S1_TC_MASK) == 0U && --timeout);
			if ((UART0->S1 & UART0_S1_TC_MASK) == 0U)
			{
				status = UART_SUCCESS;
			}
		}
	}

	UART0_TxEnable(false);

	return status;
}

UART_Status_Type UART0_SendString(uint8_t* str, uint8_t length, uint32_t timeout)
{
	uint8_t i = 0;
	UART_Status_Type status = UART_TIMEOUT;
	for (i = 0; i < length; i++)
	{
		status = UART0_SendChar(str[i], timeout);
		if (status != UART_SUCCESS)
		{
			break;
		}
	}

	return status;
}

UART_Status_Type UART0_ReceiveCharBlocking(uint8_t* rxChar, uint32_t timeout)
{
	UART_Status_Type status = UART_TIMEOUT;

	UART0_RxEnable(true);

	while((UART0->S1 & UART0_S1_RDRF_MASK) == 0U && --timeout);
	if ((UART0->S1 & UART0_S1_RDRF_MASK) == 0U)
	{
		*rxChar = UART0->D;
		status = UART_SUCCESS;
	}

	UART0_RxEnable(false);

	return status;
}

void UART0_ReceiveCharNonBlocking()
{
	UART0->C2 |= UART0_C2_RIE(1U);
	NVIC_EnableIRQ(UART0_IRQn);

	UART0_RxEnable(true);
}

void UART0_IRQHandler()
{
	if ((UART0->S1 & UART0_S1_RDRF_MASK) != 0U)
	{
		rxData = UART0->D;
		if (RxCompleteCallback)
		{
			RxCompleteCallback(rxData);
		}
	}
}

