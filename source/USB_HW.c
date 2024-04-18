#include "USB_Types.h"
#include "USB_HW.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define USB_EPR_DTOG_RX_POS             (14)
#define USB_EPR_DTOG_RX_MSK             (0b1<<USB_EPR_DTOG_RX_POS)

#define USB_EPR_STAT_RX_POS				(12)
#define USB_EPR_STAT_RX_MSK             (0b11<<USB_EPR_STAT_RX_POS)
#define USB_EPR_STAT_RX_0_MSK           (0b1<<12)
#define USB_EPR_STAT_RX_1_MSK           (0b1<<13)

#define USB_EPR_DTOG_TX_POS				(6)
#define USB_EPR_DTOG_TX_MSK             (0b1<<USB_EPR_DTOG_TX_POS)

#define USB_EPR_STAT_TX_POS				(4)
#define USB_EPR_STAT_TX_MSK             (0b11<<USB_EPR_STAT_TX_POS)
#define USB_EPR_STAT_TX_0_MSK           (0b1<<4)
#define USB_EPR_STAT_TX_1_MSK           (0b1<<5)

#define USB_EPR_RX_MSK					(0b1000111110001111)
#define USB_EPR_TX_MSK					(0b1000111110001111)
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
__inline void USB_HW_SetRXDataToggle(int EndpointIdx, int RXData)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_DTOG_RX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_RX_MSK;
	USB->EPR[EndpointIdx] = Dummy ^ (RXData << USB_EPR_DTOG_RX_POS) + Msk;
}

__inline void USB_HW_SetRXStatus(int EndpointIdx, USB_TX_RX_StatusType RXStatus)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_STAT_RX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_RX_MSK;
	USB->EPR[EndpointIdx] = (Dummy ^ (RXStatus << USB_EPR_STAT_RX_POS)) + Msk;
}

__inline void USB_HW_SetTXDataToggle(int EndpointIdx, int TXData)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_DTOG_TX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_TX_MSK;
	USB->EPR[EndpointIdx] = Dummy ^ (TXData << USB_EPR_DTOG_TX_POS) + Msk;
}

__inline void USB_HW_SetTXStatus(int EndpointIdx, USB_TX_RX_StatusType TXStatus)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_STAT_TX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_RX_MSK;
	USB->EPR[EndpointIdx] = (Dummy ^ (TXStatus << USB_EPR_STAT_TX_POS)) + Msk;
}