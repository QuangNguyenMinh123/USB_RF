#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f10x.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
typedef enum {
	PORTA = 0,
	PORTB = 16,
	PORTC = 32,
	PA0	= PORTA + 0,
	PA1	= PORTA + 1,
	PA2	= PORTA + 2,
	PA3	= PORTA + 3,
	PA4	= PORTA + 4,
	PA5	= PORTA + 5,
	PA6	= PORTA + 6,
	PA7	= PORTA + 7,
	PA8	= PORTA + 8,
	PA9	= PORTA + 9,
	PA10 = PORTA + 10,
	PA11 = PORTA + 11,
	PA12 = PORTA + 12,
	PA15 = PORTA + 15,
	PB0	= PORTB + 0,
	PB1	= PORTB + 1,
	PB5	= PORTB + 5,
	PB6	= PORTB + 6,
	PB7	= PORTB + 7,
	PB8	= PORTB + 8,
	PB9	= PORTB + 9,
	PB10 = PORTB + 10,
	PB11 = PORTB + 11,
	PB12 = PORTB + 12,
	PB13 = PORTB + 13,
	PB14 = PORTB + 14,
	PB15 = PORTB + 15,
	PC13 = PORTC + 13,
	PC14 = PORTC + 14,
	PC15 = PORTC + 15
} IO_PIN;

typedef enum {
	General_Push_Pull = 0,
	General_Open_Drain = 1,
	Alternate_Push_Pull = 2,
	Alternate_Open_Drain = 3
} OUTPUT_MODE;

typedef enum {
	Input_Pullup = 0,
	Input_Pulldown = 1,
	Input_doubleing = 2
} INPUT_MODE;

typedef enum {
	ADC_PORTA = 0,
	ADC_A0	= ADC_PORTA + 0,
	ADC_A1	= ADC_PORTA + 1,
	ADC_A2	= ADC_PORTA + 2,
	ADC_A3	= ADC_PORTA + 3,
	ADC_A4	= ADC_PORTA + 4,
	ADC_A5	= ADC_PORTA + 5,
	ADC_A6	= ADC_PORTA + 6,
	ADC_A7	= ADC_PORTA + 7
} ADC_PIN;

typedef enum {
	FALLING_EDGE = 0,
	RISING_EDGE = 1,
	BOTH_EDGES = 2
} EDGE;
/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
/* GPIO Output Module */
void GPIO_SetOutPut(IO_PIN PIN, OUTPUT_MODE mode);
void GPIO_PINLow(IO_PIN PIN);
void GPIO_PINHigh(IO_PIN PIN);
void GPIO_PINToggle(IO_PIN PIN);
/* GPIO Input Module */
void GPIO_SetInPut(IO_PIN PIN, INPUT_MODE MODE);
uint32_t GPIO_ReadPIN(IO_PIN PIN);
/* GPIO Analog Module */
void GPIO_SetAnalog(ADC_PIN PIN);
uint32_t GPIO_ReadAnalog(ADC_TypeDef *ADCx);
/* GPIO external IRQ Module */
void GPIO_SetInterrupt(IO_PIN PIN,EDGE EDGE_STATE);
#endif   /* _GPIO_H */
