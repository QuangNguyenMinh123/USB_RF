#ifndef _USB_H
#define _USB_H
#include "stm32f10x.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
typedef enum
{
	SEND,
	RECEIVE
} DataDirectionType;

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
extern DataDirectionType USB_DataDirection;
extern bool USB_InitSuccess;
/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
void USB_Init(void);
void USB_EnableInterrupt(void);
#endif  /* _USB_H */