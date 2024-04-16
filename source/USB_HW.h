#ifndef _USB_HW_H
#define _USB_HW_H
#include "stm32f10x.h"
#include "USB.h"
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
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
__inline void USB_HW_SetRXDataToggle(int EndpointIdx, USB_PIDType RXData)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_DTOG_RX_POS);
	USB->EPR[EndpointIdx] |= (Dummy ^ ((RXData >> 2) << USB_EPR_DTOG_RX_POS) );
}

__inline void USB_HW_SetRXStatus(int EndpointIdx, USB_TX_RX_StatusType RXStatus)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_STAT_RX_MSK);
	USB->EPR[EndpointIdx] |= (Dummy ^ (RXStatus << USB_EPR_STAT_RX_POS));
}

__inline void USB_HW_SetTXDataToggle(int EndpointIdx, USB_PIDType TXData)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_DTOG_TX_POS);
	USB->EPR[EndpointIdx] |= (Dummy ^ ((TXData >> 2) << USB_EPR_DTOG_TX_POS) );
}

__inline void USB_HW_SetTXStatus(int EndpointIdx, USB_TX_RX_StatusType TXStatus)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_STAT_TX_MSK);
	USB->EPR[EndpointIdx] |= (Dummy ^ (TXStatus << USB_EPR_STAT_TX_POS));
}
#endif  /* _USB_HW_H */