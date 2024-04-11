#include "GPIO.h"
#include "SPI.h"
#include "USB.h"
#include "FLASH.h"
#include "system_stm32f10x.h"
#include "nRF24L01.h"
#include "TIMER.h"
#include "CLOCK.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint32_t timer = 0;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
void main()
{
	/* Clock initialization */
	CLOCK_Init(16);
	CLOCK_SystickInit();
	SystemCoreClockUpdate();
	/* Timer initialization */
	TIMER_Init(TIM1, COUNTER_UP, 50000);
	TIMER_Init(TIM2, COUNTER_UP, 50000);
	TIMER_EnableInterrupt(TIM2);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIMER_SetCLockSourceMicros(TIM1);
	TIMER_SetCLockSourceMillis(TIM2);
	TIMER_Chaining();
	/* Peripheral initialization */
	/* SPI initialization */
	SPI_Init();
	/* USB initialization */
	USB_Init();
 	USB_EnableInterrupt();
	/* Led initialization */
	GPIO_SetOutPut(PB10, General_Push_Pull);
	GPIO_PINLow(PB10);
	/* nRF24L01 initialization */
	while (SPI_InitSuccess == FALSE);
	nRF24L01_Init();
	while (USB_InitSuccess == FALSE);
	timer = micros();
	while (1)
	{
		timer = micros();
		GPIO_PINToggle(PB10);
		while (micros() - timer <= 1000000);
	}
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/