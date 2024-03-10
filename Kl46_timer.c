/*INCLUDES-------------------------------------------------------------------*/
#include <assert.h>
#include "KL46_timer.h"
/*VARIABLES------------------------------------------------------------------*/
static volatile uint16_t current_time = 0;
#ifndef NULL
#define NULL ((void *)0)
#endif
/*Static Prototypes----------------------------------------------------------*/
static void Systick_Timer_Enable(void);
static void Systick_Timer_Disable(void);
/*DEFINITIONS-----------------------------------------------------------------*/
void Systick_Timer_Init(void) {
    /*Update core clock*/
    SystemCoreClockUpdate();
    /*Set the load value - Tick every 1ms*/
    SysTick->LOAD = (SystemCoreClock / SYSTICk_FREQUENCY) - 1;
    /*Reset current value*/
    SysTick->VAL = 0;
    /*Config clock source*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;   /*Core clock*/
    /*Enable exception request*/
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void Systick_Timer_Delay(uint16_t time_ms) {
    current_time = 0;
    /*Start timer*/
    Systick_Timer_Enable();
    while (current_time < time_ms) {}
    Systick_Timer_Disable();
}

static void Systick_Timer_Enable(void) {
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

static void Systick_Timer_Disable(void) {
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}

void SysTick_Handler(void) {
    /*Increase current time every ONE milisecond*/
    current_time ++;

}