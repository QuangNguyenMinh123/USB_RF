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


/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void USB_EndpointReset(int EndpointIdx);
static uint16_t USB_ByteSwap(uint16_t Data);
static void USB_IRQ_SendDescriptor(void);
static void USB_IRQ_SendConfiguration(void);
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
uint16_t SaveEPRStatus = 0;
uint16_t EndpointIndex = 0;
uint16_t Direction = 0;
USB_TX_RX_StatusType SaveRx;
USB_TX_RX_StatusType SaveTx;
USB_EnumerationType EnumerationStatus = UNCONNECTED;
uint16_t saveAdr = 0;
USB_RequestType Request;
USB_SpecificRequestType SpecificRequest;
uint32_t *ptr_Data;

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
int check[20][8] = {0};
int k = 0;
int getStatus = 0;
int clearFeature = 0;
int setFeature = 0;

DEVICE_INFO pInformation;
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

void copy()
{
	int j = 0;
	uint16_t *ptr = (uint16_t *)0x40006080;
	for (;j<8;j++)
	{
		check[k][j] = *ptr;
		ptr++;
	}
	k++;
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
	SpecificRequest.Request = Request.bRequest;
}

static void USB_IRQ_SendDeviceDescriptor(void)
{
	USB_HW_SetupData(END_POINT_0, (uint8_t *)Virtual_Com_Port_DeviceDescriptor, GET_DESCIPTOR_SIZE);
}
/***********************************************************************************************************************
 * Globbal function
 **********************************************************************************************************************/
void USB_IRQ_PrepareBuffer(uint32_t * ptr)
{
	ptr_Data = ptr;
}


void clearSetupRequest(void)
{
	uint32_t *ptr = (uint32_t *)0x40006080;
	int j = 0;
	for (;j<4;j++)
	{
		*ptr = 0;
		ptr++;
	}
}

void USB_IRQ_ZeroLengthPacket(void)
{
	USB_HW_SetEPTxCount(0,0);
	USB_HW_SetEPTxStatus(0, VALID);
}

void USB_IRQ_CTR_PID_SETUP_Process(uint8_t EndpointIndex)
{
	setup++;
	copy();
	if (EnumerationStatus == ADDRESSING)
	{
		while ((USB->DADDR & 0b1111111) == 0)
		{
			USB_HW_SetDeviceAddr(saveAdr);
		}
		EnumerationStatus = ADDRESSED;
		USB_HW_SetEPRxDataToggle(END_POINT_0, 0);
		USB_HW_SetEPTxDataToggle(END_POINT_0, 0);
	}

	if (Request.wLength == 0)
	{
		/* Request is SET_CONFIGURATION, SET_ADDRESS, SET_FEATURE, CLEAR_FEATURE */
		if (SpecificRequest.RequestClass == STANDARD)
		{
			if (SpecificRequest.Receiver == DEVICE)
			{
				if (SpecificRequest.Request == SET_CONFIGURATION)
    			{

    			}
				else if (SpecificRequest.Request == SET_ADDRESS)
    			{
					saveAdr = USB_ByteSwap(Request.wValue);
					USB_HW_SetEPRxCount(END_POINT_0, 64);
					USB_HW_SetEPRxStatus(END_POINT_0, VALID);
					USB_IRQ_ZeroLengthPacket();
					EnumerationStatus = ADDRESSING;
					clearSetupRequest();
    			}
				else if (SpecificRequest.Request == SET_FEATURE)
    			{

    			}
				else if (SpecificRequest.Request == CLEAR_FEATURE)
    			{

    			}
			}
			else if (SpecificRequest.Receiver == INTERFACE)
			{
				if (SpecificRequest.Request == SET_INTERFACE)
    			{

    			}
			}
			else if (SpecificRequest.Receiver == ENDPOINT)
			{
				if (SpecificRequest.Request == CLEAR_FEATURE)
    			{

    			}
				else if (SpecificRequest.Request == SET_FEATURE)
    			{

    			}
			}
			else
			{

			}
		}
	}
	else
	{
		if (SpecificRequest.Request == GET_DESCRIPTOR)
		{
			if (SpecificRequest.RequestClass == STANDARD
				&& SpecificRequest.Receiver == DEVICE)
			{
				if (EnumerationStatus != ADDRESSED)
				{
					USB_IRQ_SendDeviceDescriptor();
					USB_HW_SetEPTxStatus(END_POINT_0, VALID);
					USB_HW_SetEPRxStatus(END_POINT_0, VALID);
					EnumerationStatus = DEVICE_DESCRIPTOR;
					get_device_des++;
					clearSetupRequest();
				}
				else if (EnumerationStatus == ADDRESSED)
				{
					EnumerationStatus = FULL_DESCRIPTOR;
					USB_HW_SetEPRxDataToggle(END_POINT_0, 0);
					USB_HW_SetEPTxDataToggle(END_POINT_0, 0);
					USB_HW_SetEPTxCount(0, 18);
					get_ful_des++;
				}

			}
		}
		else if (SpecificRequest.Request == GET_STATUS)
		{

		}
		else if (SpecificRequest.Request == GET_CONFIGURATION)
		{

		}
		else if (SpecificRequest.Request == GET_INTERFACE)
		{

		}
	}
}

void USB_IRQ_CTR_PID_OUT_Process(uint8_t EndpointIndex)
{
	data_out++;
}

void USB_IRQ_CTR_PID_IN_Process(uint8_t EndpointIndex)
{
	host_in ++;
}
/***********************************************************************************************************************
 * Interrupt Handler
 **********************************************************************************************************************/
void USB_IRQ_CorrecTransfer(void)
{
	ctr ++;
	EndpointIndex = CURRENT_EP_ID;
	/* Save endpoint status */
	SaveEPRStatus = USB->EPR[EndpointIndex];
	/* Retrieve data from bufer */
	USB_GetRequest();
	if (Request.wLength > 0 && Request.wLength < 20)
		stop = 0;
	if (EndpointIndex == END_POINT_0)
	{
		SaveRx = (SaveEPRStatus & USB_EPR_STAT_RX_MSK) >> USB_EPR_STAT_RX_POS;
		SaveTx = (SaveEPRStatus & USB_EPR_STAT_TX_MSK) >> USB_EPR_STAT_TX_POS;
		Direction = DATA_DIRECTION;
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
		//USB_HW_SetEPTxStatus(EndpointIndex, SaveTx);
		//USB_HW_SetEPRxStatus(EndpointIndex, SaveRx);
	}
	else		/* Normal data transaction */
	{
		/* If host send data to deveice */
		if ((SaveEPRStatus & USB_EPR_CTR_RX_MSK) == USB_EPR_CTR_RX_MSK)
		{
			USB_HW_ClearEP_CTR_RX(EndpointIndex);
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
	USB_HW_SetEPEnpointAddress(END_POINT_0, 0);
	USB_HW_SetEPRxAddr(END_POINT_0, ENDP0_RXADDR);
	USB_HW_SetEPTxAddr(END_POINT_0, ENDP0_TXADDR);
	USB_HW_SetEPRxStatus(END_POINT_0, VALID);
	USB_HW_SetEPTxStatus(END_POINT_0, VALID);
	USB_HW_SetEPType(END_POINT_0, CONTROL);
	USB_HW_SetEPTxDataToggle(END_POINT_0, 1);
	USB_HW_SetEPRxDataToggle(END_POINT_0, 0);
	/* Endpoint 1 init */
	/*
	USB_HW_SetEPEnpointAddress(END_POINT_0, 0);
	USB_HW_SetEPRxAddr(END_POINT_1, ENDP1_RXADDR);
	USB_HW_SetEPTxAddr(END_POINT_1, ENDP1_TXADDR);
	USB_HW_SetEPRxStatus(END_POINT_1, VALID);
	USB_HW_SetEPTxStatus(END_POINT_1, VALID);
	USB_HW_SetEPType(END_POINT_1, CONTROL);
	USB_HW_SetEPKind(END_POINT_1, 1);
	USB_HW_SetEPTxDataToggle(END_POINT_1, 0);
	USB_HW_SetEPRxDataToggle(END_POINT_1, 0);
	*/
	/* PMA */
	USB_HW_SetPacketSize(END_POINT_0, 18);
	USB_HW_SetPacketSize(END_POINT_1, 18);

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
