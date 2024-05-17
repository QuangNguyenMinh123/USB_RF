#include "USB_Types.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LOBYTE(x)         	((uint8_t)((x) & 0x00FFU))
#define HIBYTE(x)			((uint8_t)(((x) & 0xFF00U) >> 8U))
#define USBD_VID     		1155
#define USBD_LANGID_STRING     1033
#define USBD_MANUFACTURER_STRING     "STMicroelectronics"
#define USBD_PID_FS     22336
#define USBD_PRODUCT_STRING_FS     "STM32 Virtual ComPort"
#define USBD_CONFIGURATION_STRING_FS     "CDC Config"
#define USBD_INTERFACE_STRING_FS     "CDC Interface"
#define  USB_DESC_TYPE_ENDPOINT                         0x05U
#define CDC_IN_EP                                   0x81U  /* EP1 for data IN */
#define CDC_OUT_EP                                  0x01U  /* EP1 for data OUT */
#define CDC_CMD_EP                                  0x82U  /* EP2 for CDC commands */
#define CDC_CMD_PACKET_SIZE							8
#define CDC_FS_BINTERVAL                          	0x10U
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */

#define USB_CONFIGURATION_DESCRIPTOR_TYPE 		0x02
#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC		67
#define USB_INTERFACE_DESCRIPTOR_TYPE			0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE			0x05
#define VIRTUAL_COM_PORT_INT_SIZE				8
#define VIRTUAL_COM_PORT_DATA_SIZE				64
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID		4
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR		38
#define	VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT		50
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL		12
#define USB_STRING_DESCRIPTOR_TYPE				0x03
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t Virtual_Com_Port_DeviceDescriptor[] =
 {
  	0x12,                       /*bLength */
  	0x01,       				/*bDescriptorType*/
  	0x00,                       /*bcdUSB */
  	0x02,
 	0x02,                       /*bDeviceClass*/
 	0x02,                       /*bDeviceSubClass*/
  	0x00,                       /*bDeviceProtocol*/
  	0x40,           			/*bMaxPacketSize*/
  	LOBYTE(USBD_VID),           /*idVendor*/
  	HIBYTE(USBD_VID),           /*idVendor*/
  	LOBYTE(USBD_PID_FS),        /*idProduct*/
  	HIBYTE(USBD_PID_FS),        /*idProduct*/
  	0x00,                       /*bcdDevice rel. 2.00*/
  	0x02,
  	1,           		/*Index of manufacturer  string*/
  	2,       			/*Index of product string*/
  	3,        			/*Index of serial number string*/
 	1  					/*bNumConfigurations*/
 };

uint8_t USB_DeviceDescriptor[] =
{
	0x12,                       /*bLength */
  	0x01,       				/*bDescriptorType*/
  	0x00,                       /*bcdUSB */
  	0x02,
 	0x02,                       /*bDeviceClass*/
 	0x02,                       /*bDeviceSubClass*/
  	0x00,                       /*bDeviceProtocol*/
  	0x40,           			/*bMaxPacketSize*/
  	LOBYTE(USBD_VID),           /*idVendor*/
  	HIBYTE(USBD_VID),           /*idVendor*/
  	LOBYTE(USBD_PID_FS),        /*idProduct*/
  	HIBYTE(USBD_PID_FS),        /*idProduct*/
  	0x00,                       /*bcdDevice rel. 2.00*/
  	0x02,
  	1,           		/*Index of manufacturer  string*/
  	2,       			/*Index of product string*/
  	3,        			/*Index of serial number string*/
 	1  					/*bNumConfigurations*/
 };

uint8_t Virtual_Com_Port_ConfigDescriptor[] =
{
    /*Configuration Descriptor*/
 	0x09,   /* bLength: Configuration Descriptor size */
  	USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
  	VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,                /* wTotalLength:no of returned bytes */
  	0x00,
  	0x02,   /* bNumInterfaces: 2 interface */
  	0x01,   /* bConfigurationValue: Configuration value */
  	0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  	0xC0,   /* bmAttributes: self powered */
  	0xF9,   /* MaxPower 0 mA */

  	/*---------------------------------------------------------------------------*/

  	/*Interface Descriptor */
  	0x09,   /* bLength: Interface Descriptor size */
  	0x04,  	/* bDescriptorType: Interface */
  	/* Interface descriptor type */
  	0x00,   /* bInterfaceNumber: Number of Interface */
  	0x00,   /* bAlternateSetting: Alternate setting */
  	0x01,   /* bNumEndpoints: One endpoints used */
  	0x02,   /* bInterfaceClass: Communication Interface Class */
  	0x02,   /* bInterfaceSubClass: Abstract Control Model */
  	0x01,   /* bInterfaceProtocol: Common AT commands */
  	0x00,   /* iInterface: */

  	/*Header Functional Descriptor*/
  	0x05,   /* bLength: Endpoint Descriptor size */
  	0x24,   /* bDescriptorType: CS_INTERFACE */
  	0x00,   /* bDescriptorSubtype: Header Func Desc */
  	0x10,   /* bcdCDC: spec release number */
  	0x01,

  	/*Call Management Functional Descriptor*/
  	0x05,   /* bFunctionLength */
  	0x24,   /* bDescriptorType: CS_INTERFACE */
  	0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  	0x00,   /* bmCapabilities: D0+D1 */
  	0x01,   /* bDataInterface: 1 */

  	/*ACM Functional Descriptor*/
  	0x04,   /* bFunctionLength */
  	0x24,   /* bDescriptorType: CS_INTERFACE */
  	0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  	0x02,   /* bmCapabilities */

  	/*Union Functional Descriptor*/
  	0x05,   /* bFunctionLength */
  	0x24,   /* bDescriptorType: CS_INTERFACE */
  	0x06,   /* bDescriptorSubtype: Union func desc */
  	0x00,   /* bMasterInterface: Communication class interface */
  	0x01,   /* bSlaveInterface0: Data Class Interface */

  	/*Endpoint 2 Descriptor*/
  	0x07,                           /* bLength: Endpoint Descriptor size */
 	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  	CDC_CMD_EP,                     /* bEndpointAddress */
  	0x03,                           /* bmAttributes: Interrupt */
  	LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
  	HIBYTE(CDC_CMD_PACKET_SIZE),
  	CDC_FS_BINTERVAL,                           /* bInterval: */
  	/*---------------------------------------------------------------------------*/

  	/*Data class interface descriptor*/
  	0x09,   					/* bLength: Endpoint Descriptor size */
  	0x04,  	/* bDescriptorType: */
  	0x01,   /* bInterfaceNumber: Number of Interface */
  	0x00,   /* bAlternateSetting: Alternate setting */
  	0x02,   /* bNumEndpoints: Two endpoints used */
	0x0A,   /* bInterfaceClass: CDC */
	0x00,   /* bInterfaceSubClass: */
  	0x00,   /* bInterfaceProtocol: */
  	0x00,   /* iInterface: */

  	/*Endpoint OUT Descriptor*/
  	0x07,   /* bLength: Endpoint Descriptor size */
  	USB_DESC_TYPE_ENDPOINT,      			/* bDescriptorType: Endpoint */
  	CDC_OUT_EP,                        		/* bEndpointAddress */
  	0x02,                              		/* bmAttributes: Bulk */
  	LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  	/* wMaxPacketSize: */
  	HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  	0x00,                              		/* bInterval: ignore for Bulk transfer */

  	/*Endpoint IN Descriptor*/
  	0x07,   								/* bLength: Endpoint Descriptor size */
  	USB_DESC_TYPE_ENDPOINT,      			/* bDescriptorType: Endpoint */
 	CDC_IN_EP,                         		/* bEndpointAddress */
  	0x02,                              		/* bmAttributes: Bulk */
  	LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  	/* wMaxPacketSize: */
  	HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  	0x00                               		/* bInterval: ignore for Bulk transfer */
};

/* USB String Descriptors */
uint8_t Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

uint8_t Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

uint8_t Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
{
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "STM32 Virtual COM Port" */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'V', 0, 'i', 0,
    'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0,
    'M', 0, ' ', 0, 'P', 0, 'o', 0, 'r', 0, 't', 0, ' ', 0, ' ', 0
};

uint8_t Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
 {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
 };

/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * EOF
 ******************************************************************************/