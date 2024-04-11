#include "USB.h"
#include "GPIO.h"
#include "USB_HW.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
USB_DataDirectionType USB_DataDirection;
bool USB_InitSuccess = FALSE;
static unsigned char USB_DeviceAddress = 0;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
static bool USB_IsIdle(void)
{

}

static bool USB_IsJState(void)
{

}

static bool USB_IsKState(void)
{

}

void USB_Init(void)
{
	/* Disable USB */
	RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
	/* Reset USB peripheral */
	RCC->APB1RSTR |= RCC_APB1RSTR_USBRST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USBRST;
	/* PLL clock is divided by 1.5 -> 72/1.5 = 48MHz */
	RCC->CFGR &= ~RCC_CFGR_USBPRE;
	/* Enable USB clock */
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	/*  */
	USB_InitSuccess = TRUE;
	
}

void USB_EnableInterrupt(void)
{

}

bool USB_CheckData(void)
{
	return TRUE;
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