#include "GPIO.h"
#include "SPI.h"
#include "USB.h"
#include "FLASH.h"
#include "system_stm32f10x.h"
#include "nRF24L01.h"
#include "TIMER.h"
#include "CLOCK.h"
#include "USB_HW.h"
#include "USB_IRQ.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define MS								3120U
#define STRING							"qwertyuioplkjhgfdsazxcvbnm1234567890\n"
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint32_t timer = 0;
uint16_t data[100];
uint16_t *ptr = &data[1];
uint8_t i = 1;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
void main()
{
	/* Clock initialization */
	CLOCK_Init(8);
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
	SPI1_Init();
	/* USB initialization */
	USB_Init();
	/* Led initialization */
	GPIO_SetOutPut(PB10, General_Push_Pull);
	GPIO_PinLow(PB10);
	/* nRF24L01 initialization */
	nRF24L01_Init();
	timer = micros();
	SPI1_Send1Byte(0, 0b00000010);
	while (1)
	{
		delay(100*MS);
		SPI1_Send1Byte(i, i);
		delay(10);
		*ptr = SPI1_Read1Byte(i, ptr);
		ptr++;
		i++;
		if (i == 16)
		{
			while (1)
			{}
		}
	}
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/