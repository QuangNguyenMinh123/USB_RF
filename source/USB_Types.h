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
/*---------------------- DIRECTION ----------------------*/
typedef enum
{
	DIR_OUT 	= 0,
	DIR_IN 		= 1
} USB_DirectionType;
/*---------------------- REQUEST TYPE ----------------------*/
typedef enum
{
	STANDARD 	= 0,
	CLASS 		= 1,
	VENDOR
} USB_RequestSpecificType;
/*---------------------- REQUEST TYPE ----------------------*/
typedef enum
{
	DEVICE		= 0,
	INTERFACE	= 1,
	ENDPOINT	= 2,
	OTHER		= 3
} USB_RecipientType;
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
	PID_SOF 	= 0b0101,
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
/*---------------------- Standard Request ----------------------*/
typedef enum
{
  	GET_STATUS = 0,
  	CLEAR_FEATURE,
  	RESERVED1,
  	SET_FEATURE,
  	RESERVED2,
  	SET_ADDRESS,
  	GET_DESCRIPTOR,
  	SET_DESCRIPTOR,
  	GET_CONFIGURATION,
  	SET_CONFIGURATION,
  	GET_INTERFACE,
  	SET_INTERFACE,
  	TOTAL_sREQUEST,  /* Total number of Standard request */
	SYNCH_FRAME = 12
} USB_Request;
/*---------------------- Enumeration state ----------------------*/
typedef enum
{
	UNCONNECTED,
	DEVICE_DESCRIPTOR,
	ADDRESSING,
	ADDRESSED,
	FULL_DESCRIPTOR
} USB_EnumerationType;
/*---------------------- Transfer status ----------------------*/
typedef enum
{
	IN,
	RECEIVING,
	OUT,
	SENDING,
	HALTED
} USB_StatusType;
/*---------------------- Device Descriptor ----------------------*/
typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} USB_DeviceDescriptorType;
/*---------------------- Configuration Descriptor ----------------------*/
typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t MaxPower;
} USB_ConfigurationDescriptorType;
/*---------------------- Interface Descriptor ----------------------*/
typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} USB_InterfaceDescriptorType;
/*---------------------- Endpoint Descriptor ----------------------*/
typedef struct
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} USB_EndpointDescriptorType;
/*----------------------  Packet Information ----------------------*/
typedef struct
{
  uint16_t Size;
  uint16_t RemainSize;
} USB_PacketInforType;
/*---------------------- Host Request ----------------------*/
typedef struct _ENDPOINT_INFO
{
  uint16_t  Usb_wLength;
  uint16_t  Usb_wOffset;
  uint16_t  PacketSize;
  uint8_t   *(*CopyData)(uint16_t Length);
} ENDPOINT_INFO;
/*---------------------- Request Type ----------------------*/
typedef struct
{
  	uint16_t bmRequestType;
  	uint16_t bRequest;
  	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} USB_RequestType;
/*---------------------- Request specific Type ----------------------*/
typedef struct
{
  	USB_DirectionType Dir;
  	USB_RequestSpecificType RequestClass;
	USB_Request Request;
	USB_RecipientType Receiver;
	uint8_t EndpointTarget;
} USB_SpecificRequestType;

typedef struct _DEVICE_INFO
{
  uint8_t USBbmRequestType;       	/* bmRequestType */
  uint8_t USBbRequest;            	/* bRequest */
  uint16_t USBwValues;         		/* wValue */
  uint16_t USBwIndexs;         		/* wIndex */
  uint16_t USBwLengths;        		/* wLength */
  uint8_t ControlState;           	/* of type CONTROL_STATE */
  uint8_t Current_Feature;
  uint8_t Current_Configuration;   	/* Selected configuration */
  uint8_t Current_Interface;       	/* Selected interface of current configuration */
  uint8_t Current_AlternateSetting;	/* Selected Alternate Setting of current
                                     interface*/
  ENDPOINT_INFO Ctrl_Info;
}DEVICE_INFO;
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
extern const uint8_t Virtual_Com_Port_DeviceDescriptor[];
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
#endif  /* _USB_TYPES_H */