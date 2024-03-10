/*
 * port.c
 *
 *  Created on: Feb 18, 2024
 *      Author: ADMIN
 */

#include "port.h"
#include "clock.h"
#include <stddef.h>
#include <assert.h>

#define IS_PORT(PORT)		((PORT) == PORTA ||	\
							 (PORT) == PORTB ||	\
							 (PORT) == PORTC ||	\
							 (PORT) == PORTD ||	\
							 (PORT) == PORTE  ) ? 1 : 0

#define IS_PIN(PIN)			((PIN) >= 0 && (PIN) < 31) ? 1 : 0

#define IS_PULL_TYPE(TYPE) (((TYPE) == PORT_PULL_DISABLE) 	|| \
							((TYPE) == PORT_PULL_DOWN) 		|| \
							((TYPE) == PORT_PULL_UP)) ? 1 : 0

#define IS_MUX_TYPE(TYPE)  (((TYPE) >= PORT_MUX_ANALOG) && \
							((TYPE) <= PORT_MUX_ALT7)) ? 1 : 0

#define IS_IRQC_TYPE(TYPE) (((TYPE) == PORT_IRQC_DISABLE) 		|| \
							((TYPE) == PORT_IRQC_DMA_RISING)	|| \
							((TYPE) == PORT_IRQC_DMA_FALLING) 	|| \
							((TYPE) == PORT_IRQC_BOTH_EDGE) 	|| \
							((TYPE) == PORT_IRQC_INT_LOW) 		|| \
							((TYPE) == PORT_IRQC_INT_RISING) 	|| \
							((TYPE) == PORT_IRQC_INT_FALLING) 	|| \
							((TYPE) == PORT_IRQC_INT_BOTH_EDGE) || \
							((TYPE) == PORT_IRQC_INT_HIGH)) ? 1 : 0


static PORT_Callback PortCDCallback = NULL;
static PORT_Callback PortACallback = NULL;

void PORT_Init(PORT_Type* PORTx, uint8_t pin, PORT_Config_Type* config)
{
	assert(IS_PORT(PORTx));
	assert(IS_PIN(pin));
	assert(IS_PULL_TYPE(config->pull));
	assert(IS_MUX_TYPE(config->mux));

	CLOCK_PortEnable(PORTx);
	PORTx->PCR[pin] &= ~(PORT_PCR_MUX_MASK 	|
			 	 	 	 PORT_PCR_PS_MASK 	|
						 PORT_PCR_PE_MASK 	|
						 PORT_PCR_MUX_MASK 	|
						 PORT_PCR_IRQC_MASK);

	if (config->mux != PORT_MUX_ANALOG)
	{
		PORTx->PCR[pin] |= PORT_PCR_MUX(config->mux);

		PORTx->PCR[pin] |= PORT_PCR_PE(config->pull >> 0x1);
		PORTx->PCR[pin] |= PORT_PCR_PS(config->pull & 0x1);

		assert(IS_IRQC_TYPE(config->irqc));
		PORTx->PCR[pin] |= PORT_PCR_IRQC(config->irqc);

		if (config->irqc != PORT_IRQC_DISABLE)
		{
			PORTx->PCR[pin] |= PORT_PCR_IRQC(config->irqc);

			assert(config->pCallback);

			if (PORTC == PORTx || PORTD == PORTx)
			{
				PortCDCallback = config->pCallback;
				NVIC_EnableIRQ(PORTC_PORTD_IRQn);
			}

			if (PORTC == PORTx || PORTD == PORTx)
			{
				PortACallback = config->pCallback;
				NVIC_EnableIRQ(PORTA_IRQn);
			}
		}
	}
}

void PORTC_PORTD_IRQHandler()
{
	uint8_t idx;

	if (CLOCK_CheckPortEnable(PORTC) != 0)
	{
		for (idx = 0; idx < 32; idx++)
		{
			if ((PORTC->ISFR & (1U << idx)) != 0U)
			{
				PORTC->ISFR = (1U << idx);
				PortCDCallback(idx);
			}
		}
	}

	if (CLOCK_CheckPortEnable(PORTD) != 0)
	{
		for (idx = 0; idx < 32; idx++)
		{
			if ((PORTD->ISFR & (1U << idx)) != 0U)
			{
				PORTD->ISFR = (1U << idx);
				PortCDCallback(idx);
			}
		}
	}
}

void PORTA_IRQHandler(void)
{
	uint8_t idx;

	for (idx = 0; idx < 32; idx++)
	{
		if ((PORTA->ISFR & (1U << idx)) != 0U)
		{
			PORTA->ISFR = (1U << idx);
			PortACallback(idx);
		}
	}
}

