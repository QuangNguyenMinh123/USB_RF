#include "USB.h"
#include "GPIO.h"
#include "USB_HW.h"
#include "CLOCK.h"
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

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
volatile int RX_IRQ = 0;
volatile int TX_IRQ = 0;
volatile int wk = 0;
volatile int sof = 0;
volatile int err = 0;
volatile int sus = 0;
volatile int reset = 0;
volatile int ctr = 0;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
static void USB_EndpointInit(int EndpointIdx)
{
	USB->EPR[EndpointIdx] |= EndpointIdx;
}

static void USB_EnableInterrupt(void)
{
	USB->CNTR |= USB_CNTR_RESETM;
	USB->CNTR |= USB_CNTR_SUSPM;
	USB->CNTR |= USB_CNTR_CTRM;
	
	NVIC_EnableIRQ(USBWakeUp_IRQ);
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQ);
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQ);
	NVIC_SetPriority ((IRQn_Type)USB_HP_CAN1_TX_IRQn, (1UL << 10) - 1UL);
	NVIC_SetPriority ((IRQn_Type)USB_LP_CAN1_RX0_IRQn, (1UL << 11) - 1UL);
	NVIC_SetPriority ((IRQn_Type)USBWakeUp_IRQn, (1UL << 12) - 1UL);

}

void USB_Init(void)
{
	int timer;
	/* Disable USB */
	RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
	/* Reset USB peripheral */
	RCC->APB1RSTR |= RCC_APB1RSTR_USBRST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USBRST;
	/* PLL clock is divided by 1.5 -> 72/1.5 = 48MHz */
	RCC->CFGR &= ~RCC_CFGR_USBPRE;
	/* Enable USB clock */
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	/* Reset USB peripheral */
	USB->CNTR |= USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 5);
	USB->CNTR &= ~USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 5);
	USB->CNTR &= ~USB_CNTR_FRES;
	/* Endpoint Initialization */
	USB_EndpointInit(END_POINT_0);
	USB_EndpointInit(END_POINT_1);
	USB_EndpointInit(END_POINT_2);
	USB_EndpointInit(END_POINT_3);
	USB_EndpointInit(END_POINT_4);
	USB_EndpointInit(END_POINT_5);
	USB_EndpointInit(END_POINT_6);
	USB_EndpointInit(END_POINT_7);
	/* Enable IRQ */
	USB_EnableInterrupt();
	/* Endpoint */ 
	/*
	USB_EndpointEncodingType EpControl = CONTROL;
	USB_TX_RX_StatusType Status = VALID;
	USB->EPR[0] |= EpControl << 9; 			
	USB->EPR[0] |= BIT_8;
	USB_HW_SetRXStatus(0, VALID);
	USB_MEM->EP_BUFFER[0].COUNT_RX = 10;
	*/
	
}

void USB_Send2Host(void)
{

}

void USBWakeUp_IRQHandler(void)
{
	
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	TX_IRQ = 2;
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	/* First reset */
	if (RESET_IRQ_OCCURED)
	{
		reset++;
		USB->ISTR &= ~USB_ISTR_RESET;
		/* USB-> EPR[0] = 0x3220;*/
		USB_HW_SetRXStatus(0,VALID);
		USB-> EPR[0] |= BIT_9;
		USB_HW_SetTXStatus(0,STALL);
		/* PMA */
		USB_MEM->EP_BUFFER[0].ADDR_RX = 64;
		USB_MEM->EP_BUFFER[0].ADDR_TX = 64;
		USB_MEM->EP_BUFFER[0].COUNT_RX = 64;
		USB_MEM->EP_BUFFER[0].COUNT_TX = 64;
		/* Enable USB device */
		USB->DADDR |= USB_DADDR_EF;
	}
	/* Suspension */
	if (SUSP_IRQ_OCCURED)
	{
		sus ++;
		USB->CNTR |= USB_CNTR_WKUPM;
		USB->ISTR &= ~USB_ISTR_SUSP;
	}
	/* Correct reception */
	if (CTR_IRQ_OCCURED)
	{
		ctr ++;
	}
	/*Handling errors */
	if (ERROR_IRQ_OCCURED)
	{
		err ++;
		USB->ISTR &= ~USB_ISTR_ERR;
	}
	
	if (WKUP_IRQ_OCCURED)
	{
		wk ++;
		USB->ISTR &= ~USB_ISTR_WKUP;
		
	}
	if (SOF_IRQ_OCCURED)
	{
		sof ++;
	}
	
	
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/