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

#define MAX_EP0_PACKET_SIZE						64

#define BAUD1(x)			(x & 0xFF) >> 0
#define BAUD2(x)			(x & 0xFF00) >> 8
#define BAUD3(x)			(x & 0xFF0000) >> 16
#define BAUD4(x)			(x & 0xFF000000) >> 24

#define StringDescriptorSize 	4
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void USB_EndpointReset(int EndpointIdx);
static uint16_t USB_ByteSwap(uint16_t Data);
static void USB_IRQ_SetCDCBaudrate(void);
typedef struct {
	uint8_t *ptr;
	uint8_t size;
} StringDescriptorType;
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint16_t SaveEPRStatus = 0;
uint16_t SaveISTR = 0;
uint16_t EndpointIndex = 0;
uint16_t Direction = 0;
USB_TX_RX_StatusType SaveRx;
USB_TX_RX_StatusType SaveTx;
USB_EnumerationType EnumerationStatus = UNCONNECTED;
uint16_t saveAdr = 0;
USB_RequestType Request;
USB_SpecificRequestType SpecificRequest;
USB_PacketInforType PacketInfo;
int Baudrate = 9600;
int Stopbit = 0;
bool ParityType = 0;
bool DataBits =  7;
StringDescriptorType StringDescriptor[StringDescriptorSize] =
{
	{Virtual_Com_Port_StringLangID, 4},
	{Virtual_Com_Port_StringVendor, 38},
	{Virtual_Com_Port_StringProduct, 50},
	{Virtual_Com_Port_StringSerial, 12}
};
int StringDescriptorIdx = StringDescriptorSize - 1;
uint8_t CDC_LineCoding[7] =
{
	0x00,			/* dwDTERate */
	0x00,
	0x00,
	0x00,
	0x00,			/* bCharFormat */
	0x00,			/* bParityType */
	0x00			/* bDataBits */
};
USB_Request PreRequest = RESERVED1;

int Dummy = 0;
int cnt = 0;
int ctr = 0;
int dum = 0;
int reset = 0;
int wk = 0;
int sof = 0;
int err = 0;
int sus = 0;
int set_addr = 0;
int get_configDes = 0;
int get_device_des = 0;
int get_ful_des = 0;
int data_out = 0;
int host_in = 0;
int setup = 0;
int stop = 0;
int k = 0;
int getStatus = 0;
int clearFeature = 0;
int setFeature = 0;
int get_interface = 0;
int getFullDes = 0;
int string_des = 0;
int pid_in = 0;
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

static void USB_IRQ_SetCDCBaudrate(void)
{
	CDC_LineCoding[0] = BAUD1(Baudrate);
	CDC_LineCoding[1] = BAUD2(Baudrate);
	CDC_LineCoding[2] = BAUD3(Baudrate);
	CDC_LineCoding[3] = BAUD4(Baudrate);
	if (Stopbit == 0)
		CDC_LineCoding[4] = 0;
	else if (Stopbit == 2)
		CDC_LineCoding[4] = 2;
	CDC_LineCoding[5] = ParityType;
	CDC_LineCoding[6] = DataBits;
}
static void USB_EndpointReset(int EndpointIdx)
{
	USB_HW_SetEPRxStatus(EndpointIdx, DISABLED);
	USB_HW_SetEPTxStatus(EndpointIdx, DISABLED);
	USB_HW_SetEPRxDataToggle(EndpointIdx, 0);
	USB_HW_SetEPTxDataToggle(EndpointIdx, 0);
	USB->EPR[EndpointIdx] = 0;
}

void USB_GetRequest(void) {
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
	SpecificRequest.Request = (USB_Request)Request.bRequest;
	SpecificRequest.DescriptorType = (USB_DescriptorType)Request.wValue;
}

static void USB_IRQ_ZeroLengthPacket(int EndpointIdx)
{
	USB_HW_SetEPTxCount(EndpointIdx, 0);
	USB_HW_SetEPTxStatus(EndpointIdx, VALID);
}
/***********************************************************************************************************************
 * Globbal function
 **********************************************************************************************************************/
void USB_IRQ_CTR_PID_SETUP_Process(uint8_t EndpointIndex)
{
	/* No data stage */
	if (Request.wLength == 0)
	{
		/* Request is SET_CONFIGURATION, SET_ADDRESS, SET_FEATURE, CLEAR_FEATURE */
		if (SpecificRequest.RequestClass == STANDARD)
		{
			if (SpecificRequest.Receiver == DEVICE)
			{
				if (SpecificRequest.Request == SET_CONFIGURATION)
    			{
					EnumerationStatus = SETTING_CONFIGURATION;
					USB_IRQ_ZeroLengthPacket(EndpointIndex);
					USB_HW_SetEPRxStatus(EndpointIndex, VALID);
    			}
				else if (SpecificRequest.Request == SET_ADDRESS)
				{
					saveAdr = USB_ByteSwap(Request.wValue);
					USB_HW_SetEPRxCount(EndpointIndex, 64);
					USB_HW_SetEPRxStatus(EndpointIndex, VALID);
					USB_IRQ_ZeroLengthPacket(EndpointIndex);
					EnumerationStatus = ADDRESSING;
    			}
				else if (SpecificRequest.Request == CLEAR_FEATURE)
    			{
					PreRequest = CLEAR_FEATURE;
    			}
			}
			else if (SpecificRequest.Receiver == INTERFACE)
			{
				if (SpecificRequest.Request == SET_FEATURE)
    			{
					PreRequest = SET_FEATURE;
    			}
			}
			else if (SpecificRequest.Receiver == ENDPOINT)
			{
				if (SpecificRequest.Request == CLEAR_FEATURE)
    			{
					Dummy = USB_ByteSwap(Request.wIndex) & 0b1111;
					USB_IRQ_ZeroLengthPacket(EndpointIndex);
					USB_HW_SetEPRxStatus(Dummy, VALID);
					Baudrate = 0;
					ParityType = 0;
					DataBits = 7;
					ParityType = 0;
					USB_IRQ_SetCDCBaudrate();
					PreRequest = CLEAR_FEATURE;
    			}
				else if (SpecificRequest.Request == SET_FEATURE)
    			{
					PreRequest = SET_FEATURE;
    			}
			}
		}
		else if (SpecificRequest.RequestClass == VENDOR)
		{
			if (SpecificRequest.Receiver == DEVICE)
			{
			}
			else if (SpecificRequest.Receiver == INTERFACE)
			{
				if (SpecificRequest.Request == SET_CONTROL_LINE_STATE)
				{
					USB_IRQ_ZeroLengthPacket(EndpointIndex);
					USB_HW_SetEPRxStatus(EndpointIndex, VALID);
					PreRequest = SET_CONTROL_LINE_STATE;
				}
				else if (SpecificRequest.Request == SET_LINE_CODING)
				{
					USB_HW_SetupData(END_POINT_0, CDC_LineCoding, 7);
					USB_HW_SetEPRxStatus(EndpointIndex, VALID);
					PreRequest = SET_LINE_CODING;
				}
			}
			else if (SpecificRequest.Receiver == ENDPOINT)
			{

			}
		}
	}
	/* Data stage */
	else
	{
		if (SpecificRequest.Request == GET_DESCRIPTOR)
		{
			if (SpecificRequest.RequestClass == STANDARD
				&& SpecificRequest.Receiver == DEVICE)
			{
				if (SpecificRequest.DescriptorType == DEVICE_DESCRIPTOR)
				{
					if (EnumerationStatus != ADDRESSED)
					{
						USB_HW_SetupData(END_POINT_0,
								(uint8_t *)Virtual_Com_Port_DeviceDescriptor, GET_DESCIPTOR_SIZE);
						USB_HW_SetEPTxStatus(EndpointIndex, VALID);
						USB_HW_SetEPRxStatus(EndpointIndex, VALID);
						EnumerationStatus = INIT_DEVICE_DESCRIPTOR;
					}
					else if (EnumerationStatus == ADDRESSED)
					{
						USB_HW_SetupData(EndpointIndex, USB_DeviceDescriptor, GET_DESCIPTOR_SIZE);
						USB_HW_SetEPRxStatus(EndpointIndex, VALID);
					}
				}
				else if (SpecificRequest.DescriptorType == CONFIG_DESCRIPTOR)
				{
					if (Request.wLength == 0xff || Request.wLength == 0x43 || Request.wLength == 0x109)
					{
						PacketInfo.PacketSize = 67;			/* 67 = Virtual_Com_Port_ConfigDescriptor size */
						PacketInfo.RemainSize = PacketInfo.PacketSize;
						USB_HW_SetupData(EndpointIndex, Virtual_Com_Port_ConfigDescriptor,
								PacketInfo.PacketSize);
						PacketInfo.RemainSize -= MAX_EP0_PACKET_SIZE;
					}
					else if (Request.wLength == 0x09)
					{
						USB_HW_SetupData(EndpointIndex, Virtual_Com_Port_ConfigDescriptor,
								Request.wLength);
					}
					USB_HW_SetEPRxStatus(EndpointIndex, VALID);
					EnumerationStatus = SENDING_FULL_DESCRIPTOR;
				}
				else if (SpecificRequest.DescriptorType == STRING_DESCRIPTOR)
				{
					Dummy = USB_ByteSwap(Request.wValue) & 0b1111;
					USB_HW_SetupData(EndpointIndex,
									StringDescriptor[Dummy].ptr,
									StringDescriptor[Dummy].size);
					USB_HW_SetEPRxStatus(EndpointIndex, VALID);
					EnumerationStatus = SENDING_STRING_DESCRIPTOR;
				}
				else if (SpecificRequest.DescriptorType == INTERFACE_DESCRIPTOR)
				{
					stop = 0;
				}
				else if (SpecificRequest.DescriptorType == ENDPOINT_DESCRIPTOR)
				{

				}
				else if (SpecificRequest.DescriptorType == DEVICE_QUALIFIER_DESCRIPTOR)
				{
					USB_HW_SetEPTxStatus(EndpointIndex, STALL);
				}
			}
		}
		else if (SpecificRequest.Request == GET_STATUS)
		{
			getStatus ++;
		}
		else if (SpecificRequest.Request == GET_CONFIGURATION)
		{
			get_configDes ++;
		}
		else if (SpecificRequest.Request == GET_INTERFACE)
		{
			get_interface ++;
		}
		else if (SpecificRequest.Request == GET_LINE_CODING)
		{
			USB_IRQ_SetCDCBaudrate();
			USB_HW_SetupData(END_POINT_0, CDC_LineCoding, 7);
			USB_HW_SetEPRxStatus(EndpointIndex, VALID);
			PreRequest = GET_LINE_CODING;
		}
		else if (SpecificRequest.Request == SET_LINE_CODING)
		{
			USB_HW_SetEPRxStatus(EndpointIndex, VALID);
			PreRequest = SET_LINE_CODING;
		}
	}
}

void USB_IRQ_CTR_PID_OUT_Process(uint8_t EndpointIndex)
{
	if (PreRequest == SET_LINE_CODING)
	{
		Baudrate = ((Request.wValue & 0x00ff) << 24);
		Baudrate |= ((Request.wValue & 0xff00) << 8);
		Baudrate |= (Request.bRequest<< 8);
		Baudrate |= Request.bmRequestType;
		Stopbit = Request.wIndex & 0xff;
		ParityType = Request.wIndex & 0xff00;
		DataBits = Request.wLength & 0xf;
		USB_IRQ_SetCDCBaudrate();
		USB_IRQ_ZeroLengthPacket(EndpointIndex);
		USB_HW_SetEPRxStatus(EndpointIndex, VALID);
	}
	else if (PreRequest == SET_CONTROL_LINE_STATE)
	{
		stop = 0;
	}
}

void USB_IRQ_CTR_PID_IN_Process(uint8_t EndpointIndex)
{
	if (EnumerationStatus == ADDRESSING)
	{
		while ((USB->DADDR & 0b1111111) == 0)
		{
			USB_HW_SetDeviceAddr(saveAdr);
		}
		EnumerationStatus = ADDRESSED;
	}
	else if (EnumerationStatus == SENDING_FULL_DESCRIPTOR)
	{
		if (PacketInfo.RemainSize > 0)
		{
			USB_HW_SetupData(EndpointIndex,
			&Virtual_Com_Port_ConfigDescriptor[PacketInfo.PacketSize - PacketInfo.RemainSize],
				PacketInfo.RemainSize);
			USB_HW_SetEPRxStatus(EndpointIndex, VALID);
			PacketInfo.RemainSize -= MAX_EP0_PACKET_SIZE;
		}
		else
		{
			USB_IRQ_ZeroLengthPacket(EndpointIndex);
			/* Start sending string descriptor */
			USB_HW_SetEPRxStatus(EndpointIndex, VALID);
			EnumerationStatus = DONE_FULL_DESCRIPTOR;
		}
	}
	else if (EnumerationStatus == SETTING_CONFIGURATION)
	{
		EnumerationStatus = DONE_SETTING_CONFIGURATION;
		USB_HW_SetEPTxStatus(EndpointIndex, VALID);
		USB_HW_SetEPRxStatus(EndpointIndex, VALID);
	}

}
/***********************************************************************************************************************
 * Interrupt Handler
 **********************************************************************************************************************/
void USB_IRQ_CorrecTransfer(void)
{
	EndpointIndex = CURRENT_EP_ID;
	/* Save endpoint status, istr register status */
	SaveISTR = USB->ISTR;
	SaveEPRStatus = USB->EPR[EndpointIndex];
	/* Retrieve data from bufer */
	USB_GetRequest();
	if (EndpointIndex == END_POINT_0)
	{
		Direction = SaveISTR & USB_ISTR_DIR;
		if (Direction == HOST_OUT)  /* Device gets the data */
		{
			/* DIR = 1 & CTR_RX  => SETUP or OUT int */
			if ((SaveEPRStatus & USB_EPR_SETUP_MSK) == USB_EPR_SETUP_MSK)
			{
				/* If host require PID SETUP */
				/* Clear CTR interupt flag */
				USB_HW_ClearEP_CTR_RX(EndpointIndex);
				USB_IRQ_CTR_PID_SETUP_Process(EndpointIndex);
			}
			else
			{
				/* Data is PID OUT */
				/* Clear CTR interupt flag */
				USB_HW_ClearEP_CTR_RX(EndpointIndex);
				USB_IRQ_CTR_PID_OUT_Process(EndpointIndex);
			}
		}
		else	/* HOST_IN, Host gets the data */
		{
			/* Clear CTR interupt flag */
			USB_HW_ClearEP_CTR_TX(EndpointIndex);
			USB_IRQ_CTR_PID_IN_Process(EndpointIndex);
		}
		/* Recover RX, TX status */
	}
	else		/* Normal data transaction */
	{
		/* If host send data to deveice */
		if ((SaveEPRStatus & USB_EPR_CTR_RX_MSK) == USB_EPR_CTR_RX_MSK)
		{
			USB_HW_ClearEP_CTR_RX(EndpointIndex);
			USB_IRQ_ZeroLengthPacket(EndpointIndex);
			USB_HW_SetEPRxStatus(EndpointIndex, VALID);
		}
		/* If device send data to host */
		if ((SaveEPRStatus & USB_EPR_CTR_TX_MSK) == USB_EPR_CTR_TX_MSK)
		{
			USB_HW_ClearEP_CTR_TX(EndpointIndex);
		}
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
	EnumerationStatus = UNCONNECTED;
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
	USB_HW_SetEPRxAddr(END_POINT_0, ENDP0_RXADDR);
	USB_HW_SetEPTxAddr(END_POINT_0, ENDP0_TXADDR);
	USB_HW_SetEPRxStatus(END_POINT_0, VALID);
	USB_HW_SetEPTxStatus(END_POINT_0, VALID);
	USB_HW_SetEPType(END_POINT_0, CONTROL);
	USB_HW_SetEPTxDataToggle(END_POINT_0, 1);
	USB_HW_SetEPRxDataToggle(END_POINT_0, 0);
	USB_HW_SetEPEnpointAddress(END_POINT_0, 0);
	/* Endpoint 1 init */
	USB_HW_SetEPRxAddr(END_POINT_1, ENDP1_RXADDR);
	USB_HW_SetEPTxAddr(END_POINT_1, ENDP1_TXADDR);
	USB_HW_SetEPRxStatus(END_POINT_1, VALID);
	USB_HW_SetEPTxStatus(END_POINT_1, VALID);
	USB_HW_SetEPType(END_POINT_1, INTERRUPT);
	USB_HW_SetEPKind(END_POINT_1, 1);
	USB_HW_SetEPTxDataToggle(END_POINT_1, 0);
	USB_HW_SetEPRxDataToggle(END_POINT_1, 0);
	USB_HW_SetEPEnpointAddress(END_POINT_1, 1);
	/* Endpoint 2 init */
	USB_HW_SetEPRxAddr(END_POINT_2, ENDP2_RXADDR);
	USB_HW_SetEPTxAddr(END_POINT_2, ENDP2_TXADDR);
	USB_HW_SetEPRxStatus(END_POINT_2, VALID);
	USB_HW_SetEPTxStatus(END_POINT_2, VALID);
	USB_HW_SetEPType(END_POINT_2, BULK);
	USB_HW_SetEPKind(END_POINT_2, 1);
	USB_HW_SetEPTxDataToggle(END_POINT_2, 0);
	USB_HW_SetEPRxDataToggle(END_POINT_2, 0);
	USB_HW_SetEPEnpointAddress(END_POINT_2, 2);
	/* PMA */
	USB_HW_SetEPRxCount(END_POINT_0, 64);
	USB_HW_SetEPRxCount(END_POINT_1, 64);
	USB_HW_SetEPRxCount(END_POINT_2, 64);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
