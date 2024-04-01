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
static uint32_t RisingTimeTIM2;
static uint32_t RisingTimeTIM3;
GPIO_PulseWidth_Type GPIO_PulseWidth;
/*******************************************************************************
 * Code
 ******************************************************************************/
/* GPIO Output Module */
void GPIO_SetOutPut(IO_PIN PIN, OUTPUT_MODE mode) {
	IO_PIN ui32Temp = PIN % 16;
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
	IO_PIN ui32Temp = PIN % 16 ;
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
	IO_PIN ui32Temp = PIN % 16;
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
	IO_PIN ui32Temp = PIN % 16;
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
	IO_PIN ui32Temp = PIN % 16;
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
	IO_PIN ui32Temp = PIN % 16;

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

uint32_t GPIO_ReadAnalog(ADC_TypeDef *ADCx) {
	ADCx->CR2 |= (1<<20);
	while ( (ADCx->SR  & (1<<1)) == 0 ) {}
	return (uint32_t) ADCx->DR;
}

/* GPIO PWM Module */
void GPIO_SetPWM(IO_PIN PIN) {
	if ( (PIN > IO_B9) || (PIN < IO_B6) )
	{
		return;
	}
	GPIO_SetOutPut(PIN, Alternate_Push_Pull);
	/* Timer 4 setting */
	if ( (RCC->APB1ENR & BIT_2) == 0) {
		RCC->APB1ENR |= BIT_2;
	}
	if ( (TIM4->CR1 & (BIT_0 | BIT_7 )) == 0) {
		TIM4->CR1 |= BIT_0 | BIT_7;
		TIM4->CR2 = 0;
		TIM4->SMCR = 0;
		TIM4->DIER = 0;
		TIM4->EGR = 0;
		TIM4->CCMR1 = 0;
		TIM4->CCMR2 = 0;
		TIM4->DCR = 0;
	}
	if ((PIN - IO_B6) < 2) {
		TIM4->CCMR1 |= (6<<(4+ (PIN-IO_B6) * 8)) ;
		TIM4->CCMR1 |=  (1<<(3+ (PIN-IO_B6) * 8)) ;
	} else {
		TIM4->CCMR2 |= (6<<(4+ (PIN-IO_B6 -2) * 8)) | (1<<(3+ (PIN-IO_B6-2) * 8));
	}
	if ( (TIM4->CCER & (1 << ((PIN-IO_B6)*4)) ) == 0 ) {
		TIM4->CCER |= (1 << ((PIN-IO_B6)*4));
	}
	if (TIM4->PSC == 0) {
		TIM4->PSC = 71;
		TIM4->ARR = 5000;
	}
}

__INLINE void GPIO_B6_PWM(uint16_t PWMValue) {
	TIM4->CCR1 = PWMValue;
}

__INLINE void GPIO_B7_PWM(uint16_t PWMValue) {
	TIM4->CCR2 = PWMValue;
}

__INLINE void GPIO_B8_PWM(uint16_t PWMValue) {
	TIM4->CCR3 = PWMValue;
}

__INLINE void GPIO_B9_PWM(uint16_t PWMValue) {
	TIM4->CCR4 = PWMValue;
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
	NVIC_EnableIRQ( (IRQType)IRQ_Idx );
}

/* GPIO Pulse width measurement */
void GPIO_SetPWMMeasurement(void) {
	RisingTimeTIM2 = 0;
	RisingTimeTIM3 = 0;
	GPIO_PulseWidth.Roll = 0;
	GPIO_PulseWidth.Aux1 = 0;
	GPIO_PulseWidth.Yaw = 0;
	GPIO_PulseWidth.Throttle = 0;
	GPIO_PulseWidth.Pitch = 0;
	GPIO_PulseWidth.Aux2 = 0;
	GPIO_SetInPut(IO_A1,Input_doubleing);
	GPIO_SetInPut(IO_A2,Input_doubleing);
	GPIO_SetInPut(IO_A3,Input_doubleing);
	GPIO_SetInPut(IO_A6,Input_doubleing);
	GPIO_SetInPut(IO_A7,Input_doubleing);
	/* Timer 2 setting */
	RCC->APB1ENR |= BIT_0;
	TIM2->CR1 |= BIT_0;
	TIM2->CR2 = 0;
	TIM2->SMCR = 0;
	TIM2->DIER = BIT_2 | BIT_3 | BIT_4;
	TIM2->EGR = 0;
	TIM2->CCMR1 = 257;
	TIM2->CCMR2 = 257;
	TIM2->CCER = BIT_4 | (3<<8) | (3<<12);
	TIM2->PSC = 71;
	TIM2->ARR = 0xFFFF;
	TIM2->DCR = 0;
	NVIC_EnableIRQ(TIM2_IRQ);

	/* Timer 3 setting */
	RCC->APB1ENR |= BIT_1;
	TIM3->CR1 |= BIT_0;
	TIM3->CR2 = 0;
	TIM3->SMCR = 0;
	TIM3->DIER = BIT_1 | BIT_2;
	TIM3->EGR = 0;
	TIM3->CCMR1 = 257;
	TIM3->CCMR2 = 257;
	TIM3->CCER = (3<<0) | (3<<4);
	TIM3->PSC = 71;
	TIM3->ARR = 0xFFFF;
	TIM3->DCR = 0;
	NVIC_EnableIRQ(TIM3_IRQ);
}

/* Return pulse width all channel */
__inline GPIO_PulseWidth_Type* GPIO_GetPWM(void) {
	return &GPIO_PulseWidth;
}

/* Timer 2 interrupt handler */
void TIM2_IRQHandler(void) {
	if (GPIOA->IDR & BIT_1) {
		/* Capture rising time */
		RisingTimeTIM2 = TIM2->CCR2;
		/* Detect falling edge */
		TIM2->CCER |= BIT_5;
		TIM2->SR = 0;
		return;
	}
	if ((GPIOA->IDR & BIT_1) == 0) {
		GPIO_PulseWidth.Roll = (TIM2->CCR2 - RisingTimeTIM2) & 0xfff - 1;
		/* Detect rising edge */
		TIM2->CCER &= ~BIT_5;
	}
	if ((GPIOA->IDR & BIT_2) == 0 ) {
		GPIO_PulseWidth.Pitch = (TIM2->CCR3 - RisingTimeTIM2) & 0xfff - 1;
	}
	if ((GPIOA->IDR & BIT_3) == 0 ) {
		GPIO_PulseWidth.Throttle = (TIM2->CCR4 - RisingTimeTIM2) & 0xfff - 1;
	}
	TIM2->SR = 0;
}

/* Timer 3 interrupt handler */
void TIM3_IRQHandler(void) {
	if (GPIOA->IDR & BIT_6) {
		/* Capture rising time */
		RisingTimeTIM3 = TIM3->CCR1;
		/* Detect falling edge */
		TIM3->CCER |= BIT_1;
		TIM3->SR = 0;
		return;
	}
	if ((GPIOA->IDR & BIT_6) == 0 ) {
		GPIO_PulseWidth.Yaw = (TIM3->CCR1 - RisingTimeTIM3) & 0xfff - 1;
		/* Detect rising edge */
		TIM3->CCER &= ~BIT_1;
	}
	if ((GPIOA->IDR & BIT_7) == 0 ) {
		GPIO_PulseWidth.Aux1 = (TIM3->CCR2 - RisingTimeTIM3) & 0xfff - 1;
	}
	TIM3->SR = 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
