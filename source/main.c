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
#include "string.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define MS								3120U
#define IS_TX							FALSE
#define GREEN_LED						PB10
#define USB_ENABLE_PIN					PB5
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint32_t timer = 0;
uint8_t Address[5];
uint8_t RxData[100];
uint8_t TxData[] = "Hello World\n";
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
	GPIO_SetOutPut(GREEN_LED, General_Push_Pull);
	GPIO_PinLow(GREEN_LED);
	GPIO_SetOutPut(USB_ENABLE_PIN, General_Push_Pull);
	GPIO_PinLow(USB_ENABLE_PIN);
	/* nRF24L01 initialization */
	nRF24L01_Init();
#if (IS_TX == TRUE)
	Address[0] = 0xEE;
#else
	Address[0] = 0xEE;
#endif
	Address[1] = 0xDD;
	Address[2] = 0xCC;
	Address[3] = 0xBB;
	Address[4] = 0xAA;

#if (IS_TX == TRUE)
	nRF24L01_TxMode(Address, 10);
#else
	nRF24L01_RxMode(Address, 10);
#endif

	while (1)
	{
		timer = micros();
#if (IS_TX == TRUE)
	if (nRF24L01_Transmit(TxData) == 1)
	{
		PIO_PinToggle(GREEN_LED);
	}
#else
	if (nRF24L01_DataAvailable(1) == TRUE)
	{
		nRF24L01_ReceiveData(RxData);
		USB_SendString(1, RxData, strlen((char *)RxData));
	}
#endif
	}
	USB_SendString(1, "1234", 4);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/