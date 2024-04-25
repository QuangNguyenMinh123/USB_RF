#include "USB_IRQ.h"
#include "stm32f10x.h"
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

#define HOST_IN				0b00000
#define HOST_OUT			0b10000

#define CURRENT_EP_ID		USB->ISTR & 0xF

#define DATA_DIRECTION		USB->ISTR & USB_ISTR_DIR

#define GET_DESCIPTOR_SIZE			0x12
#define GET_CONFIGURATION_SIZE		0x09
#define GET_CONFIGURATION_SIZE		0x08
#define GET_INTERFACE_SIZE			0x09
#define GET_ENDPOINT_SIZE			0x07

#define __bmRequestType_DIRECTION_POS			7
#define __bmRequestType_DIRECTION_MSK			(0b1 << __bmRequestType_DIRECTION_POS)
#define __bmRequestType_DIRECTION_IN			0b10000000
#define __bmRequestType_DIRECTION_OUT			0b00000000

#define __bmRequestType_REQUEST_TYPE_POS		5
#define __bmRequestType_REQUEST_TYPE_MSK		(0b11 << __bmRequestType_REQUEST_TYPE_POS)
#define __bmRequestType_REQUEST_TYPE_STANDARD	0b00000000
#define __bmRequestType_REQUEST_TYPE_CLASS		0b00000000
#define __bmRequestType_REQUEST_TYPE_VENDOR		0b01000000

#define __bmRequestType_RECIPIENT_POS			0
#define __bmRequestType_RECIPIENT_MSK			(0b1111 << __bmRequestType_RECIPIENT_POS)
#define __bmRequestType_RECIPIENT_DEVICE		0b00000000
#define __bmRequestType_RECIPIENT_INTERFACE		0b00000001
#define __bmRequestType_RECIPIENT_ENDPOINT		0b00000010
#define __bmRequestType_RECIPIENT_OTHER			0b00000011


/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void USB_EndpointReset(int EndpointIdx);
static uint16_t USB_ByteSwap(uint16_t Data);
static void USB_GetRequest(void);
static void USB_IRQ_SendDescriptor(void);
static void USB_IRQ_SendConfiguration(void);
static void USB_IRQ_ACK(void);
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint16_t Dummy = 0;
int ctr = 0;
int dum = 0;
int dum1 = 0;
uint16_t Direction = 0;
int reset = 0;
uint16_t EndpointIndex = 0;
int wk = 0;
int sof = 0;
int err = 0;
int sus = 0;
int setup = 0;

DEVICE_INFO pInformation;
uint32_t *ptr_Data;
USB_RequestType Request;
USB_SpecificRequestType SpecificRequest;
/***********************************************************************************************************************
 * Static function
 **********************************************************************************************************************/
static uint16_t USB_ByteSwap(uint16_t Data)
{
  uint8_t Temp;
  uint16_t Ret;
  Temp = (uint8_t)(Data & 0xff);
  Ret =  (Data >> 8) | ((uint16_t)Temp << 8);
  return (Ret);
}

static void USB_EndpointReset(int EndpointIdx)
{
	USB_HW_SetEPRxStatus(EndpointIdx, DISABLED);
	USB_HW_SetEPTxStatus(EndpointIdx, DISABLED);
	USB_HW_SetEPRxDataToggle(EndpointIdx, 0);
	USB_HW_SetEPTxDataToggle(EndpointIdx, 0);
	USB->EPR[EndpointIdx] = 0;
}

static void USB_GetRequest(void) {
	uint16_t *ptr_Adr16 = (uint16_t *) (USB_MEM_BASE + (uint8_t *)(USB_MEM->EP_BUFFER[END_POINT_0].ADDR_RX * 2));
	uint8_t * ptr_Adr8 = USB_MEM_BASE + (uint8_t *)(USB_MEM->EP_BUFFER[END_POINT_0].ADDR_RX * 2);
	Request.bmRequestType = *ptr_Adr8++; 			/* bmRequestType */
    Request.bRequest = *ptr_Adr8++; 				/* bRequest */
    ptr_Adr16 += 2;  								/* word not accessed because of 32 bits addressing */
    Request.wValue = USB_ByteSwap(*ptr_Adr16); 		/* wValue */
    ptr_Adr16 += 2;  								/* word not accessed because of 32 bits addressing */
    Request.wIndex  = USB_ByteSwap(*ptr_Adr16); 	/* wIndex */
    ptr_Adr16 += 2;  								/* word not accessed because of 32 bits addressing */
    Request.wLength = *ptr_Adr16; 					/* wLength */
	if ((Request.bmRequestType & __bmRequestType_DIRECTION_MSK) == __bmRequestType_DIRECTION_IN)
		SpecificRequest.Dir = DIR_IN;
	else
		SpecificRequest.Dir = DIR_OUT;

	if ((Request.bmRequestType & __bmRequestType_REQUEST_TYPE_MSK) == __bmRequestType_REQUEST_TYPE_STANDARD)
		SpecificRequest.RequestClass = STANDARD;
	else if ((Request.bmRequestType & __bmRequestType_REQUEST_TYPE_MSK) == __bmRequestType_REQUEST_TYPE_CLASS)
		SpecificRequest.RequestClass = CLASS;
	else
		SpecificRequest.RequestClass = VENDOR;

	if ((Request.bmRequestType & __bmRequestType_RECIPIENT_MSK) == __bmRequestType_RECIPIENT_DEVICE)
		SpecificRequest.Receiver = DEVICE;
	else if ((Request.bmRequestType & __bmRequestType_RECIPIENT_MSK) == __bmRequestType_RECIPIENT_INTERFACE)
		SpecificRequest.Receiver = INTERFACE;
	else if ((Request.bmRequestType & __bmRequestType_RECIPIENT_MSK) == __bmRequestType_RECIPIENT_ENDPOINT)
		SpecificRequest.Receiver = ENDPOINT;
	else
		SpecificRequest.Receiver = OTHER;

	SpecificRequest.EndpointTarget = Request.wIndex & 0b1111;
}

static void USB_IRQ_SendDescriptor(void)
{
	USB_HW_SetupData(END_POINT_0, (uint8_t *)Virtual_Com_Port_DeviceDescriptor, GET_DESCIPTOR_SIZE);
}

static void USB_IRQ_SendConfiguration(void)
{

}

static void USB_IRQ_ACK(void)
{

}

static void USB_IRQ_Recover(uint8_t EndpointIdx, uint16_t status)
{
	USB_HW_SetEPType(EndpointIdx, (USB_EndpointEncodingType) ((status & USB_EPR_EP_TYPE_MSK ) >> USB_EPR_EP_TYPE_POS));
	USB_HW_SetEPKind(EndpointIdx, (uint8_t) (status & USB_EPR_EP_KIND_MSK ) >> USB_EPR_EP_KIND_POS);
	USB_HW_SetEPEnpointAddress(EndpointIdx, (status & 0xf ));
}
/***********************************************************************************************************************
 * Globbal function
 **********************************************************************************************************************/
void USB_IRQ_PrepareBuffer(uint32_t * ptr)
{
	ptr_Data = ptr;
}

void USB_IRQ_CorrecTransfer(void)
{
	ctr ++;
	EndpointIndex = CURRENT_EP_ID;
	/* Save endpoint status */
	Dummy = USB->EPR[EndpointIndex];
	USB_GetRequest();
	if (EndpointIndex == END_POINT_0)
	{
		Direction = DATA_DIRECTION;
		/* If setup is occuring */
		if (Direction == HOST_OUT)  /* Device get the data */
		{
			/* DIR = 1 & CTR_RX  => SETUP or OUT int */
			if (USB_HW_IsSetupPacket(EndpointIndex))
			{
				/* Retrieve data from bufer */
				USB_HW_GetData(EndpointIndex, ptr_Data, 4);
				/* Clear CTR interupt flag */
				USB_HW_ClearEP_CTR_RX(EndpointIndex);
				/* Restore old state */
				USB_IRQ_SendDescriptor();
				USB_IRQ_Recover(EndpointIndex, Dummy);
				USB_HW_SetEPTxStatus(END_POINT_0, VALID);
				setup++;
			}
			else
			{
				dum ++;
			}
		}
		else
		{
			/* Clear CTR interupt flag */
			USB_HW_ClearEP_CTR_RX(EndpointIndex);
			dum ++;
		}
		/* Prepare an ACK to host */
	}
	else
	{
		dum ++;
	}
}

void USB_IRQ_BufferOverrun(void)
{

}

void USB_IRQ_Error(void)
{
	err++;
}

void USB_IRQ_Wakeup(void)
{
	wk ++;
}

void USB_IRQ_Suspend(void)
{
	sus ++;
}

void USB_IRQ_Reset(void)
{
	reset++;
	USB_EndpointReset(END_POINT_0);
	USB_EndpointReset(END_POINT_1);
	USB_EndpointReset(END_POINT_2);
	USB_EndpointReset(END_POINT_3);
	USB_EndpointReset(END_POINT_4);
	USB_EndpointReset(END_POINT_5);
	USB_EndpointReset(END_POINT_6);
	USB_EndpointReset(END_POINT_7);
	USB_HW_SetDeviceAddr(0);
	/* Endpoint 0 init */
	USB->DADDR |= BIT_7;
	USB_HW_SetEPRxAddr(END_POINT_0, ENDP0_RXADDR);
	USB_HW_SetEPTxAddr(END_POINT_0, ENDP0_TXADDR);
	USB_HW_SetEPRxStatus(END_POINT_0, VALID);
	USB_HW_SetEPTxStatus(END_POINT_0, VALID);
	USB_HW_SetEPType(END_POINT_0, CONTROL);
	USB_HW_SetEPTxDataToggle(END_POINT_0, 1);
	USB_HW_SetEPRxDataToggle(END_POINT_0, 0);

	USB_HW_SetPacketSize(END_POINT_0, 18);
	/* PMA */
}

void USB_IRQ_StartOfFrame(void)
{
	sof ++;
}

void USB_IRQ_ExpectedStartOfFrame(void)
{

}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
