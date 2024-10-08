#include "USB.h"
#include "GPIO.h"
#include "USB_HW.h"
#include "CLOCK.h"
#include "USB_IRQ.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define END_POINT_0         0
#define END_POINT_1         1
#define END_POINT_2         2
#define END_POINT_3         3
#define END_POINT_4         4
#define END_POINT_5         5
#define END_POINT_6         6
#define END_POINT_7         7

#define ERROR_IRQ_OCCURED	(USB->ISTR & USB_ISTR_ERR)
#define WKUP_IRQ_OCCURED	(USB->ISTR & USB_ISTR_WKUP)
#define SUSP_IRQ_OCCURED	(USB->ISTR & USB_ISTR_SUSP)
#define RESET_IRQ_OCCURED	(USB->ISTR & USB_ISTR_RESET)
#define SOF_IRQ_OCCURED		(USB->ISTR & USB_ISTR_SOF)
#define CTR_IRQ_OCCURED		(USB->ISTR & USB_ISTR_CTR)
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void USB_Reset(void);
static void USB_EnableInterrupt(void);
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
volatile int TX_IRQ = 0;
/***********************************************************************************************************************
 * Static function
 **********************************************************************************************************************/
static void USB_Reset(void)
{
	int timer;
	/* Reset USB peripheral */
	USB->CNTR |= USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 2);
	USB->CNTR &= ~USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 2);
	USB->CNTR &= ~USB_CNTR_FRES;
	timer = micros();
	while (micros() - timer < 2);
}

static void USB_EnableInterrupt(void)
{
	USB->CNTR |= USB_CNTR_RESETM  | USB_CNTR_CTRM;
	USB->CNTR |= USB_CNTR_SUSPM;
	NVIC_EnableIRQ((IRQn_Type)USB_HP_CAN1_TX_IRQ);
	NVIC_EnableIRQ((IRQn_Type)USB_LP_CAN1_RX0_IRQ);
	NVIC_SetPriority ((IRQn_Type)USB_HP_CAN1_TX_IRQn, (1UL << 10) - 1UL);
	NVIC_SetPriority ((IRQn_Type)USB_LP_CAN1_RX0_IRQn, (1UL << 11) - 1UL);
}
/***********************************************************************************************************************
 * Globbal function
 **********************************************************************************************************************/
void USB_Init(void)
{
	/* Enable Port A */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	/* Disable USB */
	RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
	/* Reset USB peripheral */
	RCC->APB1RSTR |= RCC_APB1RSTR_USBRST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USBRST;
	/* PLL clock is divided by 1.5 -> 72/1.5 = 48MHz */
	RCC->CFGR &= ~RCC_CFGR_USBPRE;
	/* Enable USB clock */
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	USB_HW_ClearPMA();
	USB_Reset();
	/* BTABLE reset */
	USB_HW_ResetBTABLE();
	/* Enable IRQ */
	USB_EnableInterrupt();
}

void USB_SendString(uint8_t EndpointIndex, uint8_t *Data, uint16_t size)
{
	USB_HW_SetupData(EndpointIndex, Data, size);
	USB_HW_SetEPRxStatus(EndpointIndex, VALID);
}

/***********************************************************************************************************************
 * Interrupt Handler
 **********************************************************************************************************************/
void USBWakeUp_IRQHandler(void)
{

}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	TX_IRQ ++;
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	/* First reset */
	if (RESET_IRQ_OCCURED)
	{
		USB->ISTR &= ~USB_ISTR_RESET;
		USB_IRQ_Reset();
	}
	/* Suspension */
	if (SUSP_IRQ_OCCURED)
	{
		USB->CNTR |= USB_CNTR_WKUPM;
		USB->ISTR &= ~USB_ISTR_SUSP;
		USB_IRQ_Suspend();
	}
	/* Correct reception */
	if (CTR_IRQ_OCCURED)
	{
		USB_IRQ_CorrecTransfer();
	}
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/