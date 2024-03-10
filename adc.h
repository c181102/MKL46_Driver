/*
 * adc.h
 *
 *  Created on: Mar 6, 2024
 *      Author: DELL
 */

#ifndef ADC_H_
#define ADC_H_

#include "MKL46Z4.h"

typedef enum
{
	ADC_DIFF_DISABLE,
	ADC_DIFF_ENABLE,
} ADC_Mode_Type;

typedef enum
{
	ADC_MODE_DIFF0_8BIT_DIFF1_9BIT,
	ADC_MODE_DIFF0_12BIT_DIFF1_13BIT,
	ADC_MODE_DIFF0_10BIT_DIFF1_11BIT,
	ADC_MODE_DIFF0_16BIT_DIFF1_16BIT,
} ADC_Resolution_Type;

typedef enum
{
	ADC_REFSEL_EXTERNAL,
	ADC_REFSEL_ALTERNATE,
} ADC_Voltage_Reference_Type;

typedef enum
{
	ADC_ADTRG_SOFTWARE,
	ADC_ADTRG_HARDWARE,
} ADC_Conversion_Trigger_Type;

typedef enum
{
	ADC_ADCO_DISABLE,
	ADC_ADCO_ENABLE,
} ADC_Continuous_Conversion_Type;

typedef enum
{
	ADC_AIEN_DISABLE,
	ADC_AIEN_ENABLE,
} ADC_Interrupt_Enable_Type;

typedef enum
{
	ADC_ADICLK_BUSCLOCK,
	ADC_ADICLK_HALFBUCLOCK,
	ADC_ADICLK_ALTCLK,
	ADC_ADICLK_ADACK,
} ADC_Input_Clock_Type;

typedef struct
{
	PORT_Type* PORT;
	uint8_t pin;
	uint8_t channel;
} ADC_Pin_Config_Type;

typedef struct
{
	ADC_Pin_Config_Type				pin_config[2];
	ADC_Input_Clock_Type			clock;
	ADC_Mode_Type 					mode;
	ADC_Resolution_Type				resolution;
	ADC_Voltage_Reference_Type		Vref;
	ADC_Conversion_Trigger_Type		trigger;
	ADC_Continuous_Conversion_Type	continuous;
	ADC_Interrupt_Enable_Type		interrupt;
} ADC_Config_Type;

void ADC_SetDefaultConfig(ADC_Type* ADC, ADC_Config_Type* config);
void ADC_Init(ADC_Type* ADC, ADC_Config_Type* config);
void ADC_SetDiffMode(ADC_Type* ADC, ADC_Mode_Type mode);
void ADC_SetResolution(ADC_Type* ADC, ADC_Resolution_Type resolution);
void ADC_SetReferenceVolatge(ADC_Type* ADC, ADC_Voltage_Reference_Type voltage);
void ADC_SelectTrigger(ADC_Type* ADC,  ADC_Conversion_Trigger_Type trigger);
void ADC_SelectContinuousSingleEnded(ADC_Type* ADC, ADC_Continuous_Conversion_Type continuous);
void ADC_EnableInterrupt(ADC_Type* ADC);
void ADC_DisableInterrupt(ADC_Type* ADC);

int32_t ADC_Read(ADC_Type* ADC, uint8_t channel);

#endif /* ADC_H_ */
