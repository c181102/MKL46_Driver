/**
 * \file            timer.h
 * \author          Le Hung VIET (anhvietx15hd@gmail.com)
 * \brief           
 * \version         0.1
 * \date            2024-02-18
 */
/*Include guard--------------------------------------------------------------*/
#ifndef _MIDDLE_WARE_H_
#define _MIDDLE_WARE_H_
#ifdef __cplusplus
 extern "C" {
#endif
/*INCLUDES-------------------------------------------------------------------*/
#include <stdint.h>
#include "MKL46Z4.h"
/*Prototypes*/
#define SYSTICk_FREQUENCY               (1000U)

void Systick_Timer_Init(void);
void Systick_Timer_Delay(uint16_t time_ms);

#ifdef __cplusplus
}
#endif
#endif /*_MIDDLE_WARE_H_*/
/*EOF----------------------------------------------------------------------*/
