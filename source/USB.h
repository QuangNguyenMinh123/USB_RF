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

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
void USB_Init(void);
void USB_Send2Host(void);
#endif  /* _USB_H */