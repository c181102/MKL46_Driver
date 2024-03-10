/*
 * pit.h
 *
 *  Created on: Feb 21, 2024
 *      Author: DELL
 */

#ifndef PIT_H_
#define PIT_H_

#include <MKL46Z4.h>

typedef enum
{
	PIT_FRZ_RUN,
	PIT_FRZ_STOP,
} PIT_Freeze_Type;

typedef enum
{
	PIT_CHN_NOT_CHAINED,
	PIT_CHN_CHAINED,
} PIT_Chain_Type;

typedef enum
{
	PIT_TIE_DISABLE,
	PIT_TIE_ENABLE,
} PIT_TimerEnable_Type;

typedef enum
{
	PIT_CHANNEL_0,
	PIT_CHANNEL_1,
} PIT_Channel_Type;

typedef void(*PIT_Callback)(PIT_Channel_Type);

typedef struct
{
	PIT_Freeze_Type			freeze;
	PIT_Chain_Type			chain;
	PIT_TimerEnable_Type	tie;
	PIT_Callback			pCallback;
	uint32_t 				timeout;
} PIT_Config_Type;

/*!
 * @brief Initializes a PIT channel with the given configuration.
 *
 * @param channel The PIT channel to initialize.
 * @param config Pointer to the configuration structure containing the desired settings for the timer.
 */
void PIT_Init(PIT_Channel_Type channel, PIT_Config_Type* config);

/*!
 * @brief Changes the reload value of a specified PIT channel.
 *
 * @param channel The PIT channel to modify the reload value for.
 * @param config Pointer to the configuration structure, which contains new settings to be applied.
 * @param time The new reload value to be set for the timer.
 */
void PIT_ChangeReload(PIT_Channel_Type channel, PIT_Config_Type* config, uint32_t time);

/*!
 * @brief Starts the timer for a specified PIT channel.
 *
 * @param channel The PIT channel to start the timer for.
 */
void PIT_StartTimer(PIT_Channel_Type channel);

/*!
 * @brief Stops the timer for a specified PIT channel.
 *
 * @param channel The PIT channel to stop the timer for.
 */
void PIT_StopTimer(PIT_Channel_Type channel);

#endif /* PIT_H_ */
