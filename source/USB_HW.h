#ifndef _USB_HW_H
#define _USB_HW_H
#include "USB_Types.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define BTABLE_REG  			((__IO unsigned *)(USB_BASE + 0x50))
#define _GetBTABLE() 			((uint16_t) *BTABLE_REG)

#define _pEPTxAddr(bEpNum) 		((uint32_t *)((_GetBTABLE() + bEpNum*8  ) * 2 + USB_MEM_BASE))
#define _pEPTxCount(bEpNum) 	((uint32_t *)((_GetBTABLE() + bEpNum*8 + 2) * 2 + USB_MEM_BASE))
#define _pEPRxAddr(bEpNum) 		((uint32_t *)((_GetBTABLE() + bEpNum*8 + 4) * 2 + USB_MEM_BASE))
#define _pEPRxCount(bEpNum) 	((uint32_t *)((_GetBTABLE() + bEpNum*8 + 6) * 2 + USB_MEM_BASE))

/* rx/tx buffer base address */
/* EP0  */
#define ENDP0_RXADDR        (0x40)			/* actual address: 0x80 */
#define ENDP0_TXADDR        (0x90)
/* EP1  */
#define ENDP1_RXADDR        (0xE0)			/* actual address: 0x280 */
#define ENDP1_TXADDR        (0x110)			/* actual address: 0x380 */
/* EP2  */
#define ENDP2_RXADDR        (0x140)
#define ENDP2_TXADDR        (0x170)
/* EP3  */
#define ENDP3_RXADDR        (0x180)
#define ENDP3_TXADDR        (0)

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

#define USB_EPR_NUM_BLOCK_POS			(10)

#define USB_EPR_BLSIZE_POS				(15)
#define USB_EPR_BLSIZE_MSK				(0b1 << USB_EPR_BLSIZE_POS)

#define USB_EPR_RX_MSK					(0b1000111110001111)
#define USB_EPR_TX_MSK					(0b1000111110001111)

#define USB_EPR_EP_TYPE_POS				(9)
#define USB_EPR_EP_TYPE_MSK				(0b11 << USB_EPR_EP_TYPE_POS)
#define USB_EPR_EP_TYPE_BULK			(0b00 << USB_EPR_EP_TYPE_POS)
#define USB_EPR_EP_TYPE_CONTROL			(0b01 << USB_EPR_EP_TYPE_POS)
#define USB_EPR_EP_TYPE_ISO				(0b10 << USB_EPR_EP_TYPE_POS)
#define USB_EPR_EP_TYPE_INTERRUPT		(0b11 << USB_EPR_EP_TYPE_POS)

#define USB_EPR_SETUP_POS				(11)
#define USB_EPR_SETUP_MSK				(0b1 << USB_EPR_SETUP_POS)

#define USB_EPR_CTR_RX_POS				(15)
#define USB_EPR_CTR_RX_MSK				(0b1 << USB_EPR_CTR_RX_POS)

#define USB_EPR_CTR_TX_POS				(7)
#define USB_EPR_CTR_TX_MSK				(0b1 << USB_EPR_CTR_TX_POS)

#define USB_EPR_EP_KIND_POS				(8)
#define USB_EPR_EP_KIND_MSK				(0b1 << USB_EPR_EP_KIND_POS)

#define DBL_BUF							0
#define STATUS_OUT						1

/* EndPoint Register MASK (no toggle fields) */

#define EP_CTR_RX      (0x8000) /* EndPoint Correct TRansfer RX */
#define EP_DTOG_RX     (0x4000) /* EndPoint Data TOGGLE RX */
#define EPRX_STAT      (0x3000) /* EndPoint RX STATus bit field */
#define EP_SETUP       (0x0800) /* EndPoint SETUP */
#define EP_T_FIELD     (0x0600) /* EndPoint TYPE */
#define EP_KIND        (0x0100) /* EndPoint KIND */
#define EP_CTR_TX      (0x0080) /* EndPoint Correct TRansfer TX */
#define EP_DTOG_TX     (0x0040) /* EndPoint Data TOGGLE TX */
#define EPTX_STAT      (0x0030) /* EndPoint TX STATus bit field */
#define EPADDR_FIELD   (0x000F) /* EndPoint ADDRess FIELD */

#define EP_TX_DIS      (0x0000) /* EndPoint TX DISabled */
#define EP_TX_NAK      (0x0020) /* EndPoint TX NAKed */
#define EP_TX_VALID    (0x0030) /* EndPoint TX VALID */
/* EndPoint REGister MASK (no toggle fields) */
#define EPREG_MASK     (EP_CTR_RX|EP_SETUP|EP_T_FIELD|EP_KIND|EP_CTR_TX|EPADDR_FIELD)


/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
void USB_HW_ResetBTABLE();

void USB_HW_ClearPMA(void);

void USB_HW_SetEPRxDataToggle(uint8_t EndpointIdx, int RXData);

void USB_HW_SetEPRxStatus(uint8_t EndpointIdx, USB_TX_RX_StatusType RXStatus);

void USB_HW_SetEPTxDataToggle(uint8_t EndpointIdx, int TXData);

void USB_HW_SetEPTxStatus(uint8_t EndpointIdx, USB_TX_RX_StatusType TXStatus);

void USB_HW_SetMaxPacketSize(uint8_t EndpointIdx, int Size);

void USB_HW_SetDeviceAddr(int Address);

void USB_HW_ToggleDTOG_RX(uint8_t EndpointIdx);

void USB_HW_ToggleDTOG_TX(uint8_t EndpointIdx);

void USB_HW_SetupData(uint8_t EnpointIdx, uint8_t *SourceData, int Size);

void USB_HW_GetData(uint8_t EnpointIdx, uint32_t *DestData, int Size);

void USB_HW_SetEPRxAddr(uint8_t EndpointIdx, uint16_t Addr);

void USB_HW_SetEPTxAddr(uint8_t EndpointIdx, uint16_t Addr);

void USB_HW_SetPacketSize(uint8_t EndpointIdx, int Size);

void USB_HW_SetEPType(uint8_t EnpointIdx, USB_EndpointEncodingType Type);

void USB_HW_ClearEP_CTR_RX(uint8_t EnpointIdx);

void USB_HW_ClearEP_CTR_TX(uint8_t EnpointIdx);

void USB_HW_SetEPKind(uint8_t EnpointIdx, uint8_t Kind);

void USB_HW_SetEPEnpointAddress(uint8_t EnpointIdx, uint8_t Address);

bool USB_HW_IsSetupPacket(uint8_t EnpointIdx);

void USB_HW_SetEPRxCount(uint8_t EnpointIdx, uint16_t Value);

void USB_HW_SetEPTxCount(uint8_t EnpointIdx, uint16_t Value);
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
#endif  /* _USB_HW_H */