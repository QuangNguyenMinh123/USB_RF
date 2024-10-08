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
/* Pin connection:
	Bluepill				NRF24L01
	GND					->	GND
	PB10: optional LED 	->	No connection
	PA3					-> 	IRQ
	PA5					-> 	SCK
	PA6					->	MISO
	PA7					->	MOSI
	PB0					->	CSN
	PB1					->	CE
 */
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define MS								3120U
/* Change IS_TX definition to FALSE/TRUE to set device as PTX or PRX
	IS_TX == FALSE -> device is receiver
	IS_RX == TRUE  -> device is transmitter
*/
#define IS_TX							FALSE
#define GREEN_LED						PB10
#define USB_ENABLE_PIN					PB5
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint8_t Address[5];
int RF_CH = 0;
#if (IS_TX == TRUE)
uint32_t timer = 0;
uint8_t TxData[] = "ABCDEFGHIKLMNOPQRSTVXYZ123456789";
#else
uint8_t RxData[100];
int dummy = 0;
bool DataAvailable = FALSE;
#endif
volatile uint8_t irq= 0;
int success = 0;
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
	Address[0] = 0xEE;
	Address[1] = 0xDD;
	Address[2] = 0xCC;
	Address[3] = 0xBB;
	Address[4] = 0xAA;
	nRF24L01_Init();
#if (IS_TX == TRUE)
		RF_CH = 46;
		nRF24L01_TxMode(Address, RF_CH);
#else
		RF_CH = 50;
		nRF24L01_RxMode(Address, RF_CH);
#endif
	while (1)
	{
#if (IS_TX == TRUE)
		timer = micros();
		nRF24L01_Transmit(TxData);
		while (micros() - timer < 1000000);
#else
		dummy = nRF24L01_Read1Byte(STATUS_REG);
		if ((dummy & 0b1110) != 0b1110)
		{
			success++;
			nRF24L01_Disable();
			nRF24L01_Write1Byte(STATUS_REG, STATUS_RX_DR);
			nRF24L01_ReceiveData(RxData);
			while ((dummy & FIFO_STATUS_RX_FULL)
				   == FIFO_STATUS_RX_FULL)
			{
				dummy = nRF24L01_Read1Byte(FIFO_STATUS_REG);
				nRF24L01_ReceiveData(&RxData[PACKET_SIZE]);
			}
			nRF24L01_Command(FLUSH_RX);
			nRF24L01_RxMode(Address, RF_CH);
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
#if (IS_TX == TRUE)
	irq = nRF24L01_Read1Byte(STATUS_REG);
	nRF24L01_Disable();
	if ((irq & STATUS_MAX_RT) == STATUS_MAX_RT)
	{
		nRF24L01_Write1Byte(STATUS_REG, STATUS_MAX_RT);
		nRF24L01_Enable();
	}
	else if ((irq & STATUS_TX_DS) == STATUS_TX_DS)
	{
		nRF24L01_TxMode(Address, RF_CH);
		GPIO_PinToggle(PC13);
		GPIO_PinToggle(GREEN_LED);
	}
#else
	if ((irq & STATUS_RX_DR) == STATUS_RX_DR)
	{
		irq = 0;
	}
#endif
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/