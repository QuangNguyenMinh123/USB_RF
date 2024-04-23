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

#define HOST_IN				0b00000
#define HOST_OUT			0b10000

#define ERROR_IRQ_OCCURED	(USB->ISTR & USB_ISTR_ERR)
#define WKUP_IRQ_OCCURED	(USB->ISTR & USB_ISTR_WKUP)
#define SUSP_IRQ_OCCURED	(USB->ISTR & USB_ISTR_SUSP)
#define RESET_IRQ_OCCURED	(USB->ISTR & USB_ISTR_RESET)
#define SOF_IRQ_OCCURED		(USB->ISTR & USB_ISTR_SOF)
#define CTR_IRQ_OCCURED		(USB->ISTR & USB_ISTR_CTR)

#define USB_DEVICE_DESCRIPTOR_TYPE 		1

#define CURRENT_EP_ID		USB->ISTR & 0xF

#define DATA_DIRECTION		USB->ISTR & USB_ISTR_DIR
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void USB_Reset(void);
static void USB_EnableInterrupt(void);
static void USB_EndpointReset(int EndpointIdx);
static void USB_ClearPMA(void);
static void USB_GetRequest(void);
static uint16_t USB_ByteSwap(uint16_t Data);
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
volatile uint16_t Dummy = 0;
volatile uint16_t checkCRT = 0;

volatile uint16_t Direction = 0;
volatile uint8_t EndpointIndex = 0;
const uint8_t Virtual_Com_Port_DeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,
    0x02,   /* bcdUSB = 2.00 */
    0x02,   /* bDeviceClass: CDC */
    0x02,   /* bDeviceSubClass */
    0x02,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 */
    0x83,
    0x04,   /* idVendor = 0x0483 */
    0x40,
    0x57,   /* idProduct = 0x7540 */
    0x00,
    0x02,   /* bcdDevice = 2.00 */
    1,              /* Index of string descriptor describing manufacturer */
    2,              /* Index of string descriptor describing product */
    3,              /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };
static uint32_t *ptr_Data;
DEVICE_INFO pInformation;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
static void USB_ClearPMA(void)
{
	uint32_t *ptr = (uint32_t *)USB_MEM_BASE;
	while (ptr <= (uint32_t *) (USB_MEM_BASE + 1024))
	{
		*ptr = 0;
		*ptr ++;
	}
}

static void USB_Reset(void)
{
	int timer;
	/* Reset USB peripheral */
	USB->CNTR |= USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 2);
	USB->CNTR &= ~USB_CNTR_PDWN;
	timer = micros();
	while (micros() - timer < 2);
	USB->CNTR &= ~USB_CNTR_FRES;
	timer = micros();
	while (micros() - timer < 2);
}

static void USB_EndpointReset(int EndpointIdx)
{
	USB_HW_SetEPRxStatus(EndpointIdx, DISABLED);
	USB_HW_SetEPTxStatus(EndpointIdx, DISABLED);
	USB_HW_SetEPRxDataToggle(EndpointIdx, 0);
	USB_HW_SetEPTxDataToggle(EndpointIdx, 0);
	USB->EPR[EndpointIdx] = 0;
}

static void USB_EnableInterrupt(void)
{
	USB->CNTR |= USB_CNTR_RESETM  | USB_CNTR_CTRM;
	USB->CNTR |= USB_CNTR_SUSPM;
	NVIC_EnableIRQ((IRQn_Type)USBWakeUp_IRQ);
	NVIC_EnableIRQ((IRQn_Type)USB_HP_CAN1_TX_IRQ);
	NVIC_EnableIRQ((IRQn_Type)USB_LP_CAN1_RX0_IRQ);
	NVIC_SetPriority ((IRQn_Type)USB_HP_CAN1_TX_IRQn, (1UL << 10) - 1UL);
	NVIC_SetPriority ((IRQn_Type)USB_LP_CAN1_RX0_IRQn, (1UL << 11) - 1UL);
	NVIC_SetPriority ((IRQn_Type)USBWakeUp_IRQn, (1UL << 12) - 1UL);
}

static uint16_t USB_ByteSwap(uint16_t Data)
{
  uint8_t Temp;
  uint16_t Ret;
  Temp = (uint8_t)(Data & 0xff);
  Ret =  (Data >> 8) | ((uint16_t)Temp << 8);
  return (Ret);
}

static void USB_GetRequest(void) {
	union
  	{
    	uint8_t* b;
    	uint16_t* w;
  	} pBuf;
  	uint16_t offset = 1;
  	pBuf.b = USB_MEM_BASE + (uint8_t *)(USB_MEM->EP_BUFFER[0].ADDR_RX * 2); /* *2 for 32 bits addr */
	pInformation.USBbmRequestType = *pBuf.b++; 		/* bmRequestType */
    pInformation.USBbRequest = *pBuf.b++; 			/* bRequest */
    pBuf.w += offset;  								/* word not accessed because of 32 bits addressing */
    pInformation.USBwValues = USB_ByteSwap(*pBuf.w++); 	/* wValue */
    pBuf.w += offset;  								/* word not accessed because of 32 bits addressing */
    pInformation.USBwIndexs  = USB_ByteSwap(*pBuf.w++); /* wIndex */
    pBuf.w += offset;  								/* word not accessed because of 32 bits addressing */
    pInformation.USBwLengths = *pBuf.w; 			/* wLength */
}


void USB_Init(void)
{
	/* Enable Port A */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	/* Disable USB */
	RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
	/* Reset USB peripheral */
	RCC->APB1RSTR |= RCC_APB1RSTR_USBRST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_USBRST;
	/* PLL clock is divided by 1.5 -> 72/1.5 = 48MHz */
	RCC->CFGR &= ~RCC_CFGR_USBPRE;
	/* Enable USB clock */
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	USB_ClearPMA();
	USB_Reset();
	/* BTABLE reset */
	USB_HW_ResetBTABLE();
	/* Enable IRQ */
	USB_EnableInterrupt();

	USB_HW_SetupData(END_POINT_0, (uint8_t *)Virtual_Com_Port_DeviceDescriptor,
					 sizeof(Virtual_Com_Port_DeviceDescriptor));
}

void USB_PrepareBuffer(uint32_t * ptr)
{
	ptr_Data = ptr;
}

/***********************************************************************************************************************
 * Interrupt Handler
 **********************************************************************************************************************/
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
		USB_HW_SetEPRxStatus(END_POINT_0, NACK);
		USB_HW_SetEPTxStatus(END_POINT_0, NACK);
		USB_HW_SetEPType(END_POINT_0, CONTROL);
		USB_HW_SetEPTxDataToggle(END_POINT_0, 1);
		USB_HW_SetEPRxDataToggle(END_POINT_0, 0);

		USB_HW_SetPacketSize(END_POINT_0, 18);
		/* PMA */
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
		EndpointIndex = CURRENT_EP_ID;
		/* Save endpoint status */
		Dummy = USB->EPR[EndpointIndex];
		/* If setup is occuring */
		if (USB_HW_IsSetupPacket(EndpointIndex))
		{
			Direction = DATA_DIRECTION;
			if (Direction == HOST_OUT)  /* Device get the data */
			{
				/* Retrieve data from bufer */
				USB_HW_GetData(EndpointIndex, ptr_Data, 4);
				/* Clear CTR interupt flag */
				USB_HW_ClearEP_CTR_RX(EndpointIndex);
				/* Restore old state */
				USB_GetRequest();

				USB_HW_SetEPType(EndpointIndex, (USB_EndpointEncodingType) ((Dummy & USB_EPR_EP_TYPE_MSK ) >> USB_EPR_EP_TYPE_POS));
				USB_HW_SetEPKind(EndpointIndex, (uint8_t) (Dummy & USB_EPR_EP_KIND_MSK ) >> USB_EPR_EP_KIND_POS);
				USB_HW_SetEPEnpointAddress(EndpointIndex, (Dummy & 0xf ));

				USB_HW_SetEPTxStatus(END_POINT_0, VALID);
				ctr = 0;
			}
			else
			{
				/* Clear CTR interupt flag */
				USB_HW_ClearEP_CTR_RX(EndpointIndex);
			}

			/* Prepare an ACK to host */

		}
		else	/* Normal data transfering */
		{

		}

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