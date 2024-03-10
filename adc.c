/*
 * adc.c
 *
 *  Created on: Mar 6, 2024
 *      Author: DELL
 */

#include "adc.h"
#include "clock.h"
#include "port.h"
#include "assert.h"

void ADC_SetDefaultConfig(ADC_Type* ADC, ADC_Config_Type* config)
{
	assert(config);

	config->clock = ADC_ADICLK_BUSCLOCK;
	config->mode = ADC_DIFF_DISABLE;
	config->resolution = ADC_MODE_DIFF0_8BIT_DIFF1_9BIT;
	config->Vref = ADC_REFSEL_EXTERNAL;
	config->trigger = ADC_ADTRG_SOFTWARE;
	config->continuous = ADC_ADCO_DISABLE;
	config->interrupt = ADC_AIEN_DISABLE;
}

void ADC_Init(ADC_Type* ADC, ADC_Config_Type* config)
{
	assert(config);

	uint32_t temp;

	CLOCK_ADCEnable(ADC);

	PORT_Config_Type port_conf = {
			.mux = PORT_MUX_ALT2,
			.pull = PORT_PULL_DISABLE,
	};

	PORT_Init(config->pin_config[0].PORT, config->pin_config[0].pin, &port_conf);

	/* MUX configuration */
	temp = config->pin_config[0].PORT->PCR[config->pin_config[0].pin];
	temp = (temp & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(PORT_MUX_ANALOG);
	config->pin_config[0].PORT->PCR[config->pin_config[0].pin] = temp;

	if (config->clock != ADC_ADICLK_BUSCLOCK)
	{
		temp = (ADC->CFG1 & ~ADC_CFG1_ADICLK_MASK) | ADC_CFG1_ADICLK(config->clock);
		ADC->CFG1 = temp;
	}

	if (config->mode != ADC_DIFF_DISABLE)
	{
		ADC->SC1[0] |= ADC_SC1_DIFF(ADC_DIFF_ENABLE);
		PORT_Init(config->pin_config[1].PORT, config->pin_config[1].pin, &port_conf);

		temp = config->pin_config[1].PORT->PCR[config->pin_config[1].pin];
		temp = (temp & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(PORT_MUX_ANALOG);
		config->pin_config[1].PORT->PCR[config->pin_config[1].pin] = temp;
	}

	if (config->resolution != ADC_MODE_DIFF0_8BIT_DIFF1_9BIT)
	{
		temp = (ADC->CFG1 & ~ADC_CFG1_MODE_MASK) | ADC_CFG1_MODE(config->resolution);
		ADC->CFG1 = temp;
	}

	if (config->Vref != ADC_REFSEL_EXTERNAL)
	{
		ADC->SC2 &= ~ADC_SC2_REFSEL_MASK;
		ADC->SC2 |= ADC_SC2_REFSEL(config->Vref);
	}

	if (config->trigger != ADC_ADTRG_SOFTWARE)
	{
		ADC->SC2 |= ADC_SC2_ADTRG(ADC_ADTRG_HARDWARE);
	}

	if (config->continuous != ADC_ADCO_DISABLE)
	{
		ADC->SC2 |= ADC_SC2_ADTRG(ADC_ADCO_ENABLE);
	}

	if (config->interrupt != ADC_AIEN_DISABLE)
	{
		ADC->SC1[0] |= ADC_SC1_AIEN(ADC_ADCO_ENABLE);
	}
}

void ADC_SetDiffMode(ADC_Type* ADC, ADC_Mode_Type mode)
{
	ADC->SC1[0] |= ADC_SC1_DIFF(ADC_DIFF_ENABLE);
}

void ADC_SetResolution(ADC_Type* ADC, ADC_Resolution_Type resolution)
{
	uint32_t temp = (ADC->CFG1 & ~ADC_CFG1_MODE_MASK) | ADC_CFG1_MODE(resolution);
	ADC->CFG1 = temp;
}

void ADC_SetReferenceVolatge(ADC_Type* ADC, ADC_Voltage_Reference_Type voltage)
{
	ADC->SC2 &= ~ADC_SC2_REFSEL_MASK;
	ADC->SC2 |= ADC_SC2_REFSEL(voltage);
}

void ADC_SelectTrigger(ADC_Type* ADC,  ADC_Conversion_Trigger_Type trigger)
{
	ADC->SC2 |= ADC_SC2_ADTRG(trigger);
}

void ADC_SelectContinuousSingleEnded(ADC_Type* ADC, ADC_Continuous_Conversion_Type continuous)
{
	ADC->SC2 |= ADC_SC2_ADTRG(continuous);
}

void ADC_EnableInterrupt(ADC_Type* ADC)
{
	ADC->SC1[0] |= ADC_SC1_AIEN(ADC_ADCO_ENABLE);
}

void ADC_DisableInterrupt(ADC_Type* ADC)
{
	ADC->SC1[0] &= ~ADC_SC1_AIEN_MASK;
}

int32_t ADC_Read(ADC_Type* ADC, uint8_t channel)
{
	ADC->SC1[0] = (ADC->SC1[0] & ~ADC_SC1_ADCH_MASK) | ADC_SC1_ADCH(channel);	// read modify write
	while((ADC->SC1[0] & ADC_SC1_COCO_MASK) == 0U);

	return (ADC->R[0]);
}
