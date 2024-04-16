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
USB_HW_Buffer* ptr = (USB_HW_Buffer*)USB_MEM_BASE;
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
	/* Led initialization */
	GPIO_SetOutPut(PB10, General_Push_Pull);
	GPIO_PINLow(PB10);
	/* nRF24L01 initialization */
	nRF24L01_Init();
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