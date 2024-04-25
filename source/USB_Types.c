#include "USB_Types.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint8_t Virtual_Com_Port_DeviceDescriptor[] =
  {
    0x12,   /* bLength */
    0x01,    /* bDescriptorType */
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
    0x10,
    0x01,   /* bcdDevice = 1.1 */
    1,      /* Index of string descriptor describing manufacturer */
    2,      /* Index of string descriptor describing product */
    3,      /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

USB_DeviceDescriptorType USB_DeviceDescriptor =
{
	0
};

USB_PacketInforType SendPacketInfo =
{
	0
};


/*******************************************************************************
 * Code
 ******************************************************************************/

/*******************************************************************************
 * EOF
 ******************************************************************************/