#ifndef _USB_TYPES_H
#define _USB_TYPES_H
#include "stm32f10x.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Register Address */
#define USB_BASE             	(APB1PERIPH_BASE + 0x5c00)
#define USB_MEM_BASE	     	(APB1PERIPH_BASE + 0x6000)
#define USB_MEM_SIZE	     	(512)
#define USB 					((volatile USB_TypeDef *) USB_BASE)
#define USB_MEM					((volatile USB_BufferType *) USB_MEM_BASE)
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
/*---------------------- Register definition ----------------------*/
/*---------------------- USB common register ----------------------*/
typedef struct
{
  __IO uint32_t EPR[8];
  uint32_t  RESERVED[8];
  __IO uint32_t CNTR;
  __IO uint32_t ISTR;
  __IO uint32_t FNR;
  __IO uint32_t DADDR;
  __IO uint32_t BTABLE;
} USB_TypeDef;
/*---------------------- USB hardware buffer register ----------------------*/
typedef struct {
  __IO uint32_t ADDR_TX;
  __IO uint32_t COUNT_TX;
  __IO uint32_t ADDR_RX;
  __IO uint32_t COUNT_RX;
} USB_HW_Buffer;
/*---------------------- USB buffer register ----------------------*/
typedef struct {
  USB_HW_Buffer EP_BUFFER[8];
} USB_BufferType;
/*---------------------- Endpoint type encoding ----------------------*/
typedef enum
{
	BULK		= 0b00,
	CONTROL		= 0b01,
	ISOCHRONOUS	= 0b10,
	INTERRUPT	= 0b11
} USB_EndpointEncodingType;
/*---------------------- Transmission/ Reception status encoding ----------------------*/
typedef enum
{
	DISABLED	= 0b00,
	STALL		= 0b01,
	NACK		= 0b10,
	VALID		= 0b11
} USB_TX_RX_StatusType;
/*---------------------- Packet ID encoding ----------------------*/
typedef enum
{
	/* Token */
	PID_OUT 		= 0b0001,
	PID_IN 			= 0b1001,
	PID_PID_SOF 	= 0b0101,
	PID_SETUP 		= 0b1101,
	/* Data */
	PID_DATA0 		= 0b0011,
	PID_DATA1 		= 0b1011,
	PID_DATA2 		= 0b0111,
	PID_PID_MDATA 	= 0b1111,
	/* Handshake */
	PID_ACK 		= 0b0010,
	PID_NACK 		= 0b1010,
	PID_STALL 		= 0b1110,
	PID_PID_NYET	= 0b0110,
	/* Special */
	PID_PRE 		= 0b1100,
	PID_ERR 		= 0b1100,
	PID_SPLIT 		= 0b1000,
	PID_PING 		= 0b0100
} USB_PIDType;
/*---------------------- Device Descriptor ----------------------*/
typedef struct
{
	int bLength;
	int bDescriptorType;
	int bcdUSB;
	int bDeviceClass;
	int bDeviceSubClass;
	int bDeviceProtocol;
	int bMaxPacketSize0;
	int idVendor;
	int idProduct;
	int bcdDevice;
	int iManufacturer;
	int iProduct;
	int iSerialNumber;
	int bNumConfigurations;
} USB_DeviceDescriptor;
/*---------------------- Configuration Descriptor ----------------------*/
typedef struct
{
	int bLength;
	int bDescriptorType;
	int wTotalLength;
	int bNumInterfaces;
	int bConfigurationValue;
	int iConfiguration;
	int bmAttributes;
	int bMaxPower;
} USB_ConfigurationDescriptor;
/*---------------------- Interface Descriptor ----------------------*/
typedef struct
{
	int bLength;
	int bDescriptorType;
	int bInterfaceNumber;
	int bAlternateSetting;
	int bNumEndpoints;
	int bInterfaceClass;
	int bInterfaceSubClass;
	int bInterfaceProtocol;
	int iInterface;
} USB_InterfaceDescriptor;
/*---------------------- Endpoint Descriptor ----------------------*/
typedef struct
{
	int bLength;
	int bDescriptorType;
	int bEndpointAddress;
	int bmAttributes;
	int wMaxPacketSize;
	int bInterval;
} USB_EndpointDescriptor;
/*---------------------- HID Descriptor ----------------------*/
typedef struct
{
	int bLength;
	int bDescriptorType;
	int bcdHID;
	int bCountryCode;
	int bNumDescriptors;
	int wDescriptorLength;
} USB_HIDDescriptor;
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
#endif  /* _USB_TYPES_H */