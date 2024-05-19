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
uint8_t TxData[] = "ABCDEFGHIKLMNOPQRSTVXYZ123456789";
volatile uint8_t irq= 0;
int test = 0;
bool DataAvailable = FALSE;
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
	/* GPIO initialization */
	GPIO_SetInPut(NRF24L01_IRQ_PIN, Input_doubleing);
	GPIO_SetOutPut(GREEN_LED, General_Push_Pull);
	GPIO_SetOutPut(PC13, General_Push_Pull);
	GPIO_PinHigh(GREEN_LED);
	GPIO_SetOutPut(USB_ENABLE_PIN, General_Push_Pull);
	GPIO_PinLow(USB_ENABLE_PIN);
	GPIO_SetInterrupt(PA3, FALLING_EDGE);
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
	nRF24L01_TxMode(Address, 0);
#else
	nRF24L01_RxMode(Address, 0);
#endif
	while (1)
	{
#if (IS_TX == TRUE)
		nRF24L01_Transmit(TxData);
#else
		test = nRF24L01_Read1Byte(STATUS_REG);
		if ((test & 0b1110) != 0b1110)
		{
			nRF24L01_Disable();
			nRF24L01_Write1Byte(STATUS_REG, STATUS_RX_DR);
			nRF24L01_ReceiveData(RxData);
			while ((test & FIFO_STATUS_RX_FULL)
				   == FIFO_STATUS_RX_FULL)
			{
				test = nRF24L01_Read1Byte(FIFO_STATUS_REG);
				nRF24L01_ReceiveData(&RxData[PACKET_SIZE]);
			}
			nRF24L01_Command(FLUSH_RX);
			nRF24L01_RxMode(Address, 0);
			GPIO_PinToggle(GREEN_LED);
			GPIO_PinToggle(PC13);
			USB_SendString(1, RxData, PACKET_SIZE);
		}
#endif
	}
}


void EXTI3_IRQHandler(void)
{
	EXTI->PR |= BIT_3;
	irq = nRF24L01_Read1Byte(STATUS_REG);
	nRF24L01_Disable();
#if (IS_TX == TRUE)
	if ((irq & STATUS_MAX_RT) == STATUS_MAX_RT)
	{
		nRF24L01_Write1Byte(STATUS_REG, STATUS_MAX_RT);
		nRF24L01_Enable();
	}
	else if ((irq & STATUS_TX_DS) == STATUS_TX_DS)
	{
		nRF24L01_TxMode(Address, 0);
		GPIO_PinToggle(PC13);
		GPIO_PinToggle(GREEN_LED);
	}
#else
	if ((irq & STATUS_RX_DR) == STATUS_RX_DR)
	{

	}
#endif
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/