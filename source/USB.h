#ifndef _USB_H
#define _USB_H
#include "stm32f10x.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define OUT_TOKEN		0b0001
#define	IN_TOKEN		0b1001
#define	SOF_TOKEN		0b0101
#define	SETUP_TOKEN		0b1101
#define	DATA0			0b0011
#define	DATA1			0b1011
#define	DATA2			0b0111
#define	MDATA			0b1111
#define	ACK				0b0010
#define	NACK			0b1010
#define	STALL			0b1110
#define	NYET			0b0110
#define	PREAMBLE		0b1100
#define	ERR				0b1100
#define	SPLIT			0b1000
#define	PING			0b0100
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
typedef enum
{
	IN,
	OUT
} USB_DataDirectionType;

typedef enum
{
	LOW_SPEED,
	HIGH_SPEED
} USB_Speed;

typedef enum
{
	abc
} USB_DeviceClasses;

typedef enum
{
	CONTROL,
	ISOCHRONOUS,
	BULK,
	INTERRUPT
} USB_TransferType;

typedef enum
{
	XYZ,
	ABC
} USB_RequestType;

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

typedef struct
{
	int bLength;
	int bDescriptorType;
	int wTotalLength;
	int bNumInterfaces;
	int bConfigurationValue;
	int iConfiguration;
	int bMaxPower;
} USB_ConfigurationDescriptor;

typedef struct
{
	int bLength;
	int bDescriptorType;
	int bInterfaceNumber;
	int bAlternateSetting;
	int bNumEndpoints;
	int bInterfaceClass;
	int bInterfaceProtocol;
	int iInterface;
} USB_InterfaceDescriptor;

typedef struct
{
	int bLength;
	int bDescriptorType;
	int bEndpointAddress;
	int wMaxPacketSize;
	int bInterval;
} USB_EndpointDescriptor;
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
extern USB_DataDirectionType USB_DataDirection;
extern bool USB_InitSuccess;
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
void USB_Init(void);
void USB_EnableInterrupt(void);
bool USB_CheckData(void);
void USB_Send2Host(void);
#endif  /* _USB_H */