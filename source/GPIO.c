#include "GPIO.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static void delay(uint32_t delayTime) {
	uint32_t ui32Cnt = 0U;
	for (; ui32Cnt< delayTime; ui32Cnt++) {
		__asm("nop");                                                                                                                               	}
}
/*******************************************************************************
 * Code
 ******************************************************************************/
/* GPIO Output Module */
void GPIO_SetOutPut(IO_PIN PIN, OUTPUT_MODE mode) {
	uint32_t ui32Temp = (uint32_t) (PIN % 16);
	/* PORT A */
	if (PIN / 16 == 0) {
			/* Set Port A Clock */
		if ( (RCC->APB2ENR & (1<<2)) == 0 ) {
			RCC->APB2ENR |= 1<<2;
		}
			/* Config mode */
		if (ui32Temp <= 7 ) {
			GPIOA->CRL &= ~(15 << (ui32Temp * 4 ));
			if (mode == General_Push_Pull) {
				GPIOA->CRL |=  ( General_Push_Pull << (ui32Temp * 4 + 2));
				GPIOA->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == General_Open_Drain) {
				GPIOA->CRL |=  ( General_Open_Drain << (ui32Temp * 4 + 2));
				GPIOA->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Push_Pull) {
				GPIOA->CRL |=  ( Alternate_Push_Pull << (ui32Temp * 4 + 2));
				GPIOA->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Open_Drain) {
				GPIOA->CRL |=  ( Alternate_Open_Drain << (ui32Temp * 4 + 2));
				GPIOA->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
		} else
		{
			ui32Temp -= 8;
			GPIOA->CRH &= ~(15 << (ui32Temp * 4 ));
			if (mode == General_Push_Pull) {
				GPIOA->CRH |=  ( General_Push_Pull << (ui32Temp * 4 + 2));
				GPIOA->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == General_Open_Drain) {
				GPIOA->CRH |=  ( General_Open_Drain << (ui32Temp * 4 + 2));
				GPIOA->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Push_Pull) {
				GPIOA->CRH |=  ( Alternate_Push_Pull << (ui32Temp * 4 + 2));
				GPIOA->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Open_Drain) {
				GPIOA->CRH |=  ( Alternate_Open_Drain << (ui32Temp * 4 + 2));
				GPIOA->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
		}
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
			/* Set Port B Clock */
		if ( (RCC->APB2ENR & (1<<3)) == 0 ) {
			RCC->APB2ENR |= 1<<3;
		}
			/* Config mode */
		if (ui32Temp <= 7 ) {
			GPIOB->CRL &= ~(15 << (ui32Temp * 4 ));
			if (mode == General_Push_Pull) {
				GPIOB->CRL |=  ( General_Push_Pull << (ui32Temp * 4 + 2));
				GPIOB->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == General_Open_Drain) {
				GPIOB->CRL |=  ( General_Open_Drain << (ui32Temp * 4 + 2));
				GPIOB->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Push_Pull) {
				GPIOB->CRL |=  ( Alternate_Push_Pull << (ui32Temp * 4 + 2));
				GPIOB->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Open_Drain) {
				GPIOB->CRL |=  ( Alternate_Open_Drain << (ui32Temp * 4 + 2));
				GPIOB->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
		} else
		{
			ui32Temp -= 8;
			GPIOB->CRH &= ~(15 << (ui32Temp * 4 ));
			if (mode == General_Push_Pull) {
				GPIOB->CRH |=  ( General_Push_Pull << (ui32Temp * 4 + 2));
				GPIOB->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == General_Open_Drain) {
				GPIOB->CRH |=  ( General_Open_Drain << (ui32Temp * 4 + 2));
				GPIOB->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Push_Pull) {
				GPIOB->CRH |=  ( Alternate_Push_Pull << (ui32Temp * 4 + 2));
				GPIOB->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Open_Drain) {
				GPIOB->CRH |=  ( Alternate_Open_Drain << (ui32Temp * 4 + 2));
				GPIOB->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
		}
	}
	/* PORT C */
	else if (PIN / 16 == 2) {
			/* Set Port C Clock */
		if ( (RCC->APB2ENR & (1<<4)) == 0 ) {
			RCC->APB2ENR |= 1<<4;
		}
			/* Config mode */
		if (ui32Temp <= 7 ) {
			GPIOC->CRL &= ~(15 << (ui32Temp * 4 ));
			if (mode == General_Push_Pull) {
				GPIOC->CRL |=  ( General_Push_Pull << (ui32Temp * 4 + 2));
				GPIOC->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == General_Open_Drain) {
				GPIOC->CRL |=  ( General_Open_Drain << (ui32Temp * 4 + 2));
				GPIOC->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Push_Pull) {
				GPIOC->CRL |=  ( Alternate_Push_Pull << (ui32Temp * 4 + 2));
				GPIOC->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Open_Drain) {
				GPIOC->CRL |=  ( Alternate_Open_Drain << (ui32Temp * 4 + 2));
				GPIOC->CRL |=  ( 3 << (ui32Temp * 4 ));
			}
		} else
		{
			ui32Temp -= 8;
			GPIOC->CRH &= ~(15 << (ui32Temp * 4 ));
			if (mode == General_Push_Pull) {
				GPIOC->CRH |=  ( General_Push_Pull << (ui32Temp * 4 + 2));
				GPIOC->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == General_Open_Drain) {
				GPIOC->CRH |=  ( General_Open_Drain << (ui32Temp * 4 + 2));
				GPIOC->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Push_Pull) {
				GPIOC->CRH |=  ( Alternate_Push_Pull << (ui32Temp * 4 + 2));
				GPIOC->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
			else if (mode == Alternate_Open_Drain) {
				GPIOC->CRH |=  ( Alternate_Open_Drain << (ui32Temp * 4 + 2));
				GPIOC->CRH |=  ( 3 << (ui32Temp * 4 ));
			}
		}
	}
}

void GPIO_PINLow(IO_PIN PIN) {
	/* PORT A */
	uint32_t ui32Temp = (uint32_t) PIN % 16 ;
	if (PIN / 16 == 0) {
		GPIOA->BSRR |= ( 1 << ( ui32Temp + 16 ) );
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
		GPIOB->BSRR |= ( 1 << ( ui32Temp + 16 ) );
	}
	/* PORT C */
	else if (PIN / 16 == 2) {
		GPIOC->BSRR |= ( 1 << ( ui32Temp + 16 ) );
	}
}

void GPIO_PINHigh(IO_PIN PIN) {
	/* PORT A */
	uint32_t ui32Temp = (uint32_t) PIN % 16;
	if (PIN / 16 == 0) {
		GPIOA->BSRR |= (1<<ui32Temp);
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
		GPIOB->BSRR |= (1<<ui32Temp);
	}
	/* PORT C */
	else if (PIN / 16 == 2) {
		GPIOC->BSRR |= (1<<ui32Temp);
	}
}

void GPIO_PINToggle(IO_PIN PIN) {
	uint32_t Data = 0;
	/* PORT A */
	uint32_t ui32Temp = (uint32_t) PIN % 16;
	if (PIN / 16 == 0) {
		Data = GPIOA->ODR;
		if (Data & (1 << ui32Temp)) {
			GPIO_PINLow(PIN);
		}
		else {
			GPIO_PINHigh(PIN);
		}
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
		Data = GPIOB->ODR;
		if (Data & (1 << ui32Temp)) {
			GPIO_PINLow(PIN);
		}
		else {
			GPIO_PINHigh(PIN);
		}
	}
	/* PORT C */
	else if (PIN / 16 == 2) {
		Data = GPIOC->ODR;
		if (Data & (1 << ui32Temp)) {
			GPIO_PINLow(PIN);
		}
		else {
			GPIO_PINHigh(PIN);
		}
	}
}

/* GPIO Input Module */
void GPIO_SetInPut(IO_PIN PIN, INPUT_MODE MODE) {
	uint32_t ui32Temp = (uint32_t) PIN % 16;
	/* PORT A */
	if (PIN / 16 == 0) {
			/* Set Port A Clock */
		if ( (RCC->APB2ENR & (1<<2)) == 0 ) {
			RCC->APB2ENR |= 1<<2;
		}
		if (MODE == Input_Pullup) {
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOA->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOA->CRL |=  ( 2 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOA->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOA->CRH |=  ( 2 << ((ui32Temp-8) * 4 + 2));
			}
			GPIOA->ODR &= ~(1<<ui32Temp);
			GPIOA->ODR |= 1<<ui32Temp;
		} else if (MODE == Input_Pulldown)
		{
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOA->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOA->CRL |=  ( 2 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOA->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOA->CRH |=  ( 2 << ((ui32Temp-8) * 4 + 2));
			}
			GPIOA->ODR &= ~(1<<ui32Temp);
		}
		else if (MODE == Input_doubleing)
		{
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOA->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOA->CRL |=  ( 1 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOA->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOA->CRH |=  ( 1 << ((ui32Temp-8) * 4 + 2));
			}
		}
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
			/* Set Port B Clock */
		if ( (RCC->APB2ENR & (1<<3)) == 0 ) {
			RCC->APB2ENR |= 1<<3;
		}
		if (MODE == Input_Pullup) {
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOB->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOB->CRL |=  ( 2 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOB->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOB->CRH |=  ( 2 << ((ui32Temp-8) * 4 + 2));
			}
			GPIOB->ODR &= ~(1<<ui32Temp);
			GPIOB->ODR |= 1<<ui32Temp;
		} else if (MODE == Input_Pulldown)
		{
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOB->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOB->CRL |=  ( 2 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOB->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOB->CRH |=  ( 2 << ((ui32Temp-8) * 4 + 2));
			}
			GPIOB->ODR &= ~(1<<ui32Temp);
		}
		else if (MODE == Input_doubleing)
		{
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOB->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOB->CRL |=  ( 1 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOB->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOB->CRH |=  ( 1 << ((ui32Temp-8) * 4 + 2));
			}
		}
	}
	/* PORT C */
	else if (PIN / 16 == 2) {
			/* Set Port C Clock */
		if ( (RCC->APB2ENR & (1<<4)) == 0 ) {
			RCC->APB2ENR |= 1<<4;
		}
		if (MODE == Input_Pullup) {
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOC->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOC->CRL |=  ( 2 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOC->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOC->CRH |=  ( 2 << ((ui32Temp-8) * 4 + 2));
			}
			GPIOC->ODR &= ~(1<<ui32Temp);
			GPIOC->ODR |= 1<<ui32Temp;
		} else if (MODE == Input_Pulldown)
		{
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOC->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOC->CRL |=  ( 2 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOC->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOC->CRH |=  ( 2 << ((ui32Temp-8) * 4 + 2));
			}
			GPIOC->ODR &= ~(1<<ui32Temp);
		}
		else if (MODE == Input_doubleing)
		{
			if (ui32Temp <= 7 ) {
				/* MODE0:1 =00; CNF1:0 = 10  */
				GPIOC->CRL &= ~(15 << (ui32Temp * 4 ));
				GPIOC->CRL |=  ( 1 << (ui32Temp * 4 + 2));
			} else
			{
				GPIOC->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
				GPIOC->CRH |=  ( 1 << ((ui32Temp-8) * 4 + 2));
			}
		}
	}
}

uint32_t GPIO_ReadPIN(IO_PIN PIN) {
	uint32_t ui32Ret = 0U;
	/* PORT A */
	if (PIN / 16 == 0) {
		ui32Ret = GPIOA->IDR & ( 1 << (PIN % 16));
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
		ui32Ret = GPIOB->IDR & ( 1 << (PIN % 16));
	}
	/* PORT C */
	else if (PIN / 16 == 2) {
		ui32Ret = GPIOC->IDR & ( 1 << (PIN % 16));
	}
	return ui32Ret;
}
/* GPIO Analog Module */
void GPIO_SetAnalog(ADC_PIN PIN) {
	uint32_t ui32Temp = (uint32_t) PIN % 16;

	/* CLOCK enable */
	/* PORT A */
	if (PIN / 16 == 0) {
			/* Set Port A Clock */
		if ( (RCC->APB2ENR & (1<<2)) == 0 ) {
			RCC->APB2ENR |= 1<<2;
		}
		if (ui32Temp <= 7 ) {
			/* MODE0:1 =00; CNF1:0 = 10  */
			GPIOA->CRL &= ~(15 << (ui32Temp * 4 ));
		} else
		{
			GPIOA->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
		}
	}
	/* PORT B */
	else if (PIN / 16 == 1) {
			/* Set Port B Clock */
		if ( (RCC->APB2ENR & (1<<3)) == 0 ) {
			RCC->APB2ENR |= 1<<3;
		}
		if (ui32Temp <= 7 ) {
			GPIOB->CRL &= ~(15 << (ui32Temp * 4 ));
		} else
		{
			GPIOB->CRH &= ~(15 << ((ui32Temp-8) * 4 ));
		}
	}


	/* ADC1 setting */
	/* ADC prescaler RCC_ADCPRE = 6 */
	RCC->CFGR &= ~(3<<14);
	RCC->CFGR |= (2<<14);
	/* ADC1 clock enable ADC1EN */
	if ( (RCC->APB2ENR & (1<<9)) == 0) {
		RCC->APB2ENR |= (1<<9);
	}
	/* Continuous conversion
	if ( (ADC1->CR2 & (1<<1)) == 0) {
		ADC1->CR2 |= (1<<1);
	}   */
	/* ADC EXTSEL */
	ADC1->CR2 &= ~(7<<17);
	ADC1->CR2 |=  (7<<17);
	if (PIN / 16 == 0)
	{
		if ( (PIN % 16) <= 9)
		{
			ADC1->SMPR2 &= ~(7 << ((PIN % 16)*3));
			ADC1->SMPR2 |=  (7 << ((PIN % 16)*3));
			ADC1->SQR3  &= ~(7 << 0);
			ADC1->SQR3  |=  ((PIN % 16) << 0);
		}
		else {
			ADC1->SMPR1 &= ~(7<< (((PIN % 16) - 10) * 3));
			ADC1->SMPR1 |=  (7<< (((PIN % 16) - 10) * 3));
			ADC1->SQR3  &= ~(7 << 0);
			ADC1->SQR3  |=  ((PIN % 16) << 0);
		}
	}
	/* PORT B */
	else
	if (PIN / 16 == 1)
	{
		if ( (PIN % 16) <= 9)
		{
			ADC1->SMPR2 &= ~(7<< ((PIN % 16)*3));
			ADC1->SMPR2 |= (7<< ((PIN % 16)*3));
			ADC1->SQR3  &= ~(7 << 0);
			ADC1->SQR3  |=  ((PIN % 16) << 0);
		}
		else {
			ADC1->SMPR1 &= ~(7<< (((PIN % 16) - 10) * 3));
			ADC1->SMPR1 |=  (7<< (((PIN % 16) - 10) * 3));
			ADC1->SQR3  &= ~(7 << 0);
			ADC1->SQR3  |=  ((PIN % 16) << 0);
		}
	}
	/* set conversion trigger */
	ADC1->CR2 |= (7<<20);
	/* Start conversion ADCON */
	if ( (ADC1->CR2 & (1<<0)) == 0) {
		ADC1->CR2 |= (1<<0);
	}
	delay(10);
}

/* GPIO external IRQ Module */
void GPIO_SetInterrupt(IO_PIN PIN,EDGE EDGE_STATE ) {
	uint32_t IRQ_Idx = 0;
	if ( ((PIN % 16) <= 4) ) {
		IRQ_Idx += PIN % 16 + 6;
	} else
	if ( ((PIN % 16) > 4) && ((PIN % 16) < 10)) {
		IRQ_Idx = 23;
	} else
	{
		IRQ_Idx = 40;
	}
	/* Interrupt mask register */
	EXTI->IMR |= (1<<(PIN % 16));
	/* RISING EDGE */
	if (EDGE_STATE == RISING_EDGE) {
		EXTI->FTSR &= ~(1<<(PIN % 16));
		EXTI->RTSR |= (1<<(PIN % 16));
	} else
	/* FALLING EDGE */
	if (EDGE_STATE == FALLING_EDGE) {
		EXTI->RTSR &= ~(1<<(PIN % 16));
		EXTI->FTSR |= (1<<(PIN % 16));
	}
	/* BOTH EDGES */
	else {
		EXTI->RTSR |= (1<<(PIN % 16));
		EXTI->FTSR |= (1<<(PIN % 16));
	}
	/* IRQ handler */
	NVIC_EnableIRQ(IRQ_Idx);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
