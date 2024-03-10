/*
 * pit.c
 *
 *  Created on: Feb 21, 2024
 *      Author: DELL
 */

#include "pit.h"
#include "clock.h"
#include <assert.h>
#include <stddef.h>

#define IS_PIT_FREEZE_TYPE(TYPE)    (((TYPE) == PIT_FRZ_RUN) || \
                                     ((TYPE) == PIT_FRZ_STOP)) ? 1 : 0

#define IS_PIT_CHAIN_TYPE(TYPE)     (((TYPE) == PIT_CHN_NOT_CHAINED) || \
                                     ((TYPE) == PIT_CHN_CHAINED)) ? 1 : 0

#define IS_PIT_TIE_TYPE(TYPE)       (((TYPE) == PIT_TIE_DISABLE) || \
                                     ((TYPE) == PIT_TIE_ENABLE)) ? 1 : 0

#define IS_PIT_CHANNEL_TYPE(TYPE)   (((TYPE) == PIT_CHANNEL_0) || \
                                     ((TYPE) == PIT_CHANNEL_1)) ? 1 : 0

static PIT_Callback PITCallback = NULL;

static uint32_t ReloadValueCalculate(uint32_t millisecond)
{
	uint32_t val = (CLOCK_GetBusClock() / 1000 * millisecond) - 1;
	return val;
}

void PIT_Init(PIT_Channel_Type channel, PIT_Config_Type* config)
{
	assert(IS_PIT_FREEZE_TYPE(config->freeze));
	assert(IS_PIT_CHAIN_TYPE(config->chain));
	assert(IS_PIT_TIE_TYPE(config->tie));
	assert(IS_PIT_CHANNEL_TYPE(channel));
	assert(config->timeout > 0);

	CLOCK_PITEnable();

	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_MDIS(config->freeze);

	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_CHN(config->chain);

	PIT->CHANNEL[channel].LDVAL = ReloadValueCalculate(config->timeout);

	if (config->tie != PIT_TIE_DISABLE)
	{
		PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE(config->tie);

		assert(config->pCallback);

		PITCallback = config->pCallback;
		NVIC_EnableIRQ(PIT_IRQn);
	}
}

void PIT_ChangeReload(PIT_Channel_Type channel, PIT_Config_Type* config, uint32_t time_ms)
{
	assert(IS_PIT_CHANNEL_TYPE(channel));
	assert(time_ms > 0);

	PIT_StopTimer(channel);

	config->timeout = time_ms;
	PIT->CHANNEL[channel].LDVAL = ReloadValueCalculate(config->timeout);

	PIT_StartTimer(channel);
}

void PIT_StartTimer(PIT_Channel_Type channel)
{
	assert(IS_PIT_CHANNEL_TYPE(channel));
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN(1U);
}

void PIT_StopTimer(PIT_Channel_Type channel)
{
	assert(IS_PIT_CHANNEL_TYPE(channel));

	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN(0U);
}

void PIT_IRQHandler()
{
	if (PIT->CHANNEL[PIT_CHANNEL_0].TFLG == 1U)
	{
		PITCallback(PIT_CHANNEL_0);
		PIT->CHANNEL[PIT_CHANNEL_0].TFLG = 1U;
	}

	if (PIT->CHANNEL[PIT_CHANNEL_1].TFLG == 1U)
	{
		PITCallback(PIT_CHANNEL_1);
		PIT->CHANNEL[PIT_CHANNEL_1].TFLG = 1U;
	}
}
