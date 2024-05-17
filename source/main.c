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
#define IS_TX							TRUE
#define GREEN_LED						PB10
#define USB_ENABLE_PIN					PB5

#define STATUS_MAX_RT					0b10000
#define STATUS_TX_DS					0b100000
#define STATUS_RX_DR					0b1000000
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
uint8_t check[100] = 0;
uint8_t after[100] = 0;
int i= 0;
int test = 0;
int dem = 0;
int max = 0;
int tx = 0;
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
	GPIO_PinHigh(GREEN_LED);
	GPIO_SetOutPut(USB_ENABLE_PIN, General_Push_Pull);
	GPIO_PinLow(USB_ENABLE_PIN);
	GPIO_SetOutPut(PA2, General_Push_Pull);
	GPIO_PinLow(PA2);
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
	test = nRF24L01_Read1Byte(STATUS_REG);
	while (1)
	{
		timer = micros();
#if (IS_TX == TRUE)
		//nRF24L01_Command(FLUSH_TX);
		nRF24L01_Transmit(TxData);

		GPIO_PinToggle(GREEN_LED);
		dem++;

#else
		if (nRF24L01_DataAvailable(1) == 1)
		{
			nRF24L01_ReceiveData(RxData);
			USB_SendString(1, RxData, strlen((char *)RxData));
			GPIO_PinToggle(GREEN_LED);
		}
#endif
		while (nRF24L01_Read1Byte(OBSERVE_TX_REG) < 3)
		{
			test = 0;
		}
		if (GPIO_ReadPIN(NRF24L01_IRQ_PIN) == 0)
		{
			check[i] = nRF24L01_Read1Byte(STATUS_REG);
			test = check [i];
			if ((test & STATUS_MAX_RT) == STATUS_MAX_RT)
			{
				nRF24L01_Write1Byte(STATUS_REG, STATUS_MAX_RT);
				after[i] = nRF24L01_Read1Byte(STATUS_REG);
				GPIO_PinToggle(PA2);
				nRF24L01_TxMode(Address, 0);
				nRF24L01_Disable();
				max ++;
			}
			if ((test & STATUS_TX_DS) == STATUS_TX_DS)
			{
				nRF24L01_Write1Byte(STATUS_REG, 0x10);
				nRF24L01_Read1Byte(STATUS_REG);
				nRF24L01_TxMode(Address, 0);
				tx ++;
				test = 0;
			}
			if ((test & STATUS_RX_DR) == STATUS_RX_DR)
			{
				test = 0;
			}
			i++;
		}
		while (micros() - timer < 1000000);
	}
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/