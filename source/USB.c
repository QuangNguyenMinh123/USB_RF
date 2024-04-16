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
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
static void USB_EndpointInit(int EndpointIdx)
{
	
}

static void USB_EnableInterrupt(void)
{
	USB->CNTR |= USB_CNTR_CTRM;
	USB->CNTR |= USB_CNTR_PMAOVRM;
	USB->CNTR |= USB_CNTR_ERRM;
	USB->CNTR |= USB_CNTR_WKUPM;
	USB->CNTR |= USB_CNTR_SUSPM;
	USB->CNTR |= USB_CNTR_RESETM;
	USB->CNTR |= USB_CNTR_SOFM;
	USB->CNTR |= USB_CNTR_ESOFM;
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
	while (micros() - timer < 1);
	USB->CNTR &= ~USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 1);
	/* Enable USB device */
	USB->DADDR |= USB_DADDR_EF;
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
}

void USB_Send2Host(void)
{

}

void USBWakeUp_IRQHandler(void)
{

}

void USB_HP_CAN1_TX_IRQHandler(void)
{

}

void USB_LP_CAN1_RX0_IRQHandler(void)
{

}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/