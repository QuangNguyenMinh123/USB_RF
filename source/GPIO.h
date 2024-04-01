#ifndef _GPIO_H
#define _GPIO_H
#include "stm32f10x.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************ADC CONTROLLER*********************************/

#define ADC_CONVERSION_COMPLETED			ADC1->SR  & (1<<1)
#define SERVO_START							TIM4->CNT = 4999
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

typedef enum {
	IO_PORTA = 0,
	IO_PORTB = 16,
	IO_PORTC = 32,
	IO_A0	= IO_PORTA + 0,
	IO_A1	= IO_PORTA + 1,
	IO_A2	= IO_PORTA + 2,
	IO_A3	= IO_PORTA + 3,
	IO_A4	= IO_PORTA + 4,
	IO_A5	= IO_PORTA + 5,
	IO_A6	= IO_PORTA + 6,
	IO_A7	= IO_PORTA + 7,
	IO_A8	= IO_PORTA + 8,
	IO_A9	= IO_PORTA + 9,
	IO_A10	= IO_PORTA + 10,
	IO_A11	= IO_PORTA + 11,
	IO_A12	= IO_PORTA + 12,
	IO_A15	= IO_PORTA + 15,
	IO_B0	= IO_PORTB + 0,
	IO_B1	= IO_PORTB + 1,
	IO_B5	= IO_PORTB + 5,
	IO_B6	= IO_PORTB + 6,
	IO_B7	= IO_PORTB + 7,
	IO_B8	= IO_PORTB + 8,
	IO_B9	= IO_PORTB + 9,
	IO_B10	= IO_PORTB + 10,
	IO_B11	= IO_PORTB + 11,
	IO_B12	= IO_PORTB + 12,
	IO_B13	= IO_PORTB + 13,
	IO_B14	= IO_PORTB + 14,
	IO_B15	= IO_PORTB + 15,
	IO_C13	= IO_PORTC + 13,
	IO_C14	= IO_PORTC + 14,
	IO_C15 = IO_PORTC + 15
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

typedef struct {
  uint16_t Throttle;
  uint16_t Yaw;
  uint16_t Pitch;
  uint16_t Roll;
  uint16_t Aux1;
  uint16_t Aux2;
} GPIO_PulseWidth_Type;
/*******************************************************************************
 * Global variables
 ******************************************************************************/
extern GPIO_PulseWidth_Type GPIO_PulseWidth;
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
