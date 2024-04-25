#ifndef _USB_H
#define _USB_H
#include "stm32f10x.h"
#include "USB_Types.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define DATA0				0
#define DATA1				1
#define DATA2				2
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
extern const uint8_t Virtual_Com_Port_DeviceDescriptor[];
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
void USB_Init(void);
void USB_PrepareBuffer(uint32_t * ptr);
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
#endif  /* _USB_H */