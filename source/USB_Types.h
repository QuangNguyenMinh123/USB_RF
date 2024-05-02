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
	SYNCH_FRAME = 12,
	SET_LINE_CODING	= 0x20,
	GET_LINE_CODING	= 0x21,
	SET_CONTROL_LINE_STATE	= 0x22,
} USB_Request;
/*---------------------- Enumeration state ----------------------*/
typedef enum
{
	UNCONNECTED,
	INIT_DEVICE_DESCRIPTOR,
	ADDRESSING,
	ADDRESSED,
	SENDING_FULL_DESCRIPTOR,
	DONE_FULL_DESCRIPTOR,
	SENDING_STRING_DESCRIPTOR,
	DONE_STRING_DESCRIPTOR,
	SETTING_CONFIGURATION,
	DONE_SETTING_CONFIGURATION,
} USB_EnumerationType;
/*---------------------- Definition of BwValue ----------------------*/
typedef enum _DESCRIPTOR_TYPE
{
  DEVICE_DESCRIPTOR = 1,
  CONFIG_DESCRIPTOR = 2,
  STRING_DESCRIPTOR = 3,
  INTERFACE_DESCRIPTOR = 4,
  ENDPOINT_DESCRIPTOR = 5,
  DEVICE_QUALIFIER_DESCRIPTOR = 6,
  DEVICE_BOS_DESCRIPTOR = 0xF
} USB_DescriptorType;
/*----------------------  Packet Information ----------------------*/
typedef struct
{
  int PacketSize;		/* unit: number of bytes to be sent */
  int RemainSize;		/* remaining bytes to finish */
} USB_PacketInforType;
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
	USB_DescriptorType DescriptorType;
	uint8_t EndpointTarget;
} USB_SpecificRequestType;
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
extern uint8_t Virtual_Com_Port_DeviceDescriptor[];
extern uint8_t USB_DeviceDescriptor[];
extern uint8_t Virtual_Com_Port_ConfigDescriptor[];
extern uint8_t Virtual_Com_Port_StringLangID[];
extern uint8_t Virtual_Com_Port_StringVendor[];
extern uint8_t Virtual_Com_Port_StringProduct[];
extern uint8_t Virtual_Com_Port_StringSerial[];
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
#endif  /* _USB_TYPES_H */