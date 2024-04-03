#include "GPIO.h"
#include "SPI.h"
#include "USB.h"
#include "FLASH.h"
#include "system_stm32f10x.h"
#include "nRF24L01.h"
#include "TIMER.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define MS							3120U
#define SEC							3129597U
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void delay(uint32_t delayTime) {
	uint32_t ui32Cnt = 0U;
	for (; ui32Cnt< delayTime; ui32Cnt++) {
		__asm("nop");                                                                                                                               	}
}
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
void main()
{
	/*SystemInit();
	SystemCoreClockUpdate();
	*/
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
	while (1)
	{
		if (USB_DataDirection == SEND)
		{

		}
		else
		{

		}
		delay(10*MS);
		GPIO_PINHigh(PB10);
		delay(2000*MS);
		GPIO_PINLow(PB10);
		delay(2000*MS);
	}
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/