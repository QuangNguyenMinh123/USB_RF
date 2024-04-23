#include "USB_Types.h"
#include "USB_HW.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
__inline void USB_HW_ResetBTABLE()
{
	USB->BTABLE = 0;
}

__inline void USB_HW_SetEPRxDataToggle(uint8_t EndpointIdx, int RXData)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_DTOG_RX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_RX_MSK;
	USB->EPR[EndpointIdx] = Dummy ^ (RXData << USB_EPR_DTOG_RX_POS) + Msk;
}

__inline void USB_HW_SetEPRxStatus(uint8_t EndpointIdx, USB_TX_RX_StatusType RXStatus)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_STAT_RX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_RX_MSK;
	USB->EPR[EndpointIdx] = (Dummy ^ (RXStatus << USB_EPR_STAT_RX_POS)) + Msk;
}

__inline void USB_HW_SetEPTxDataToggle(uint8_t EndpointIdx, int TXData)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_DTOG_TX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_TX_MSK;
	USB->EPR[EndpointIdx] = Dummy ^ (TXData << USB_EPR_DTOG_TX_POS) + Msk;
}

__inline void USB_HW_SetEPTxStatus(uint8_t EndpointIdx, USB_TX_RX_StatusType TXStatus)
{
	uint16_t Dummy = (USB->EPR[EndpointIdx] & USB_EPR_STAT_TX_MSK);
	uint16_t Msk = USB->EPR[EndpointIdx] & USB_EPR_RX_MSK;
	USB->EPR[EndpointIdx] = (Dummy ^ (TXStatus << USB_EPR_STAT_TX_POS)) + Msk;
}

__inline void USB_HW_ToggleDTOG_RX(uint8_t EndpointIdx)
{
	USB->EPR[EndpointIdx] |= USB_EPR_DTOG_RX_MSK;
}

__inline void USB_HW_ToggleDTOG_TX(uint8_t EndpointIdx)
{
	USB->EPR[EndpointIdx] |= USB_EPR_DTOG_TX_MSK;
}

__inline void USB_HW_SetEPTxAddr(uint8_t EndpointIdx, uint16_t Addr)
{
	USB_MEM->EP_BUFFER[EndpointIdx].ADDR_TX = Addr;
}

__inline void USB_HW_SetEPRxAddr(uint8_t EndpointIdx, uint16_t Addr)
{
	USB_MEM->EP_BUFFER[EndpointIdx].ADDR_RX = Addr;
}

void USB_HW_SetPacketSize(uint8_t EndpointIdx, int Size)
{
	USB_MEM->EP_BUFFER[EndpointIdx].COUNT_RX = 0;
	if (Size < 32)
	{
		USB_MEM->EP_BUFFER[EndpointIdx].COUNT_RX |= (Size / 2) << USB_EPR_NUM_BLOCK_POS | Size;
	}
	else
	{

	}
}

__inline void USB_HW_SetDeviceAddr(int Address)
{
	USB->DADDR |= Address;
}

__inline void USB_HW_SetupData(uint8_t EnpointIdx, uint8_t *SourceData, int Size)
{
	int i = 0;
	uint16_t *ptr_Adr = (uint16_t *) (USB_MEM->EP_BUFFER[EnpointIdx].ADDR_TX + USB_MEM_BASE);
	uint8_t * ptr_Data = SourceData;
	for (i = 0; i < Size; i++)
	{
		*ptr_Adr = *ptr_Data;
		ptr_Data++;
		ptr_Adr+=2;
	}
	USB_MEM->EP_BUFFER[EnpointIdx].COUNT_TX = Size;
}

__inline void USB_HW_GetData(uint8_t EnpointIdx, uint32_t *DestData, int Size)
{
	int i = 0;
	uint32_t *ptr_Adr = (uint32_t *) (USB_MEM->EP_BUFFER[EnpointIdx].ADDR_RX * 2 + USB_MEM_BASE);
	uint32_t * ptr_Data = DestData;
	for (i = 0; i < Size; i++)
	{
		*ptr_Data = *ptr_Adr;
		ptr_Data++;
		ptr_Adr++;
	}
}

__inline void USB_HW_SetEPType(uint8_t EnpointIdx, USB_EndpointEncodingType Type)
{
	uint16_t Msk = USB->EPR[EnpointIdx] & USB_EPR_TX_MSK;
	USB->EPR[EnpointIdx] =  Msk | (Type << USB_EPR_EP_TYPE_POS);
}

__inline void USB_HW_ClearEP_CTR_RX(uint8_t EnpointIdx)
{
	USB->EPR[EnpointIdx] = 0b0000;
}

__inline void USB_HW_SetEPKind(uint8_t EnpointIdx, uint8_t Kind)
{
	uint16_t Msk = USB->EPR[EnpointIdx] & USB_EPR_TX_MSK;
	USB->EPR[EnpointIdx] =  Msk | (Kind << USB_EPR_EP_KIND_POS);
}

__inline void USB_HW_SetEPEnpointAddress(uint8_t EnpointIdx, uint8_t Address)
{
	uint16_t Msk = USB->EPR[EnpointIdx] & USB_EPR_TX_MSK;
	USB->EPR[EnpointIdx] =  Msk | (Address);
}

__inline bool USB_HW_IsSetupPacket(uint8_t EnpointIdx)
{
	uint16_t Msk = USB->EPR[EnpointIdx] & USB_EPR_SETUP_MSK;
	bool Ret = (Msk == USB_EPR_SETUP_MSK);
	return Ret;
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/