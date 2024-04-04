#include "TIMER.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
static TIM_TypeDef* TIMER_SourceMillis = 0;
static TIM_TypeDef* TIMER_SourceMicros = 0;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
static void delay(uint32_t delayTime) {
	uint32_t ui32Cnt = 0U;
	for (; ui32Cnt< delayTime; ui32Cnt++) {
		__asm("nop");                                                                                                                               	}
}

void TIMER_Init(TIM_TypeDef* TIMx, TIMER_CounterMode Mode, uint32_t Counter)
{
	/* Timer auto reload register content is equal to "Counter" value */
	switch (*(uint32_t*)&TIMx)
	{
		case TIM1_BASE:
			RCC->APB2RSTR |= RCC_APB2RSTR_TIM1RST;
			delay(10);
			RCC->APB2RSTR &= ~RCC_APB2RSTR_TIM1RST;
			RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
			break;
		case TIM2_BASE:
			RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;	/* Timer reset */
			delay(10);
			RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;		/* Provide Clock for timer */
			break;
		case TIM3_BASE:
			RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;	/* Timer reset */
			delay(10);
			RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
			break;
		case TIM4_BASE:
			RCC->APB1RSTR |= RCC_APB1RSTR_TIM4RST;	/* Timer reset */
			delay(10);
			RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM4RST;
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
			break;
	}
}

void TIMER_Chaining(void)
{
	/* RM0008, page 399 */
	/* Configure Timer 1 in master mode so that it outputs a periodic trigger signal on each
	update event UEV. If you write MMS=010 in the TIM1_CR2 register, a rising edge is
	output on TRGO1 each time an update event is generated */
	TIMER_SourceMicros->CR1 &= ~TIM_CR1_CEN;			/* Disable timer */
	TIMER_SourceMillis->CR1 &= ~TIM_CR1_CEN;			/* Disable timer */
	
	TIMER_SourceMicros->CR1 |= TIM_CR1_URS;			/* Only generate IRQ in case of overflow */
	TIMER_SourceMicros->CR1 &= ~TIM_CR1_UDIS;
	TIMER_SourceMicros->CR2 &= ~(0b111 << 4);
	TIMER_SourceMicros->CR2 |= (0b010 << 4);		
	/* To connect the TRGO1 output of Timer 1 to Timer 2, Timer 2 must be configured in
	slave mode using ITR0 as internal trigger. You select this through the TS bits in the
	TIM2_SMCR register (writing TS=000) */
	TIMER_SourceMillis->SMCR &= ~(0b111 << 4);
	TIMER_SourceMillis->SMCR |= (0b000 << 4);
	/* Then you put the slave mode controller in external clock mode 1 (write SMS=111 in the
	TIM2_SMCR register). This causes Timer 2 to be clocked by the rising edge of the
	periodic Timer 1 trigger signal (which correspond to the timer 1 counter overflow) */
	TIMER_SourceMillis->SMCR &= ~(0b111 << 0);
	TIMER_SourceMillis->SMCR |= (0b111 << 0);
	/* Finally both timers must be enabled by setting their respective CEN bits (TIMx_CR1
	register). */
	TIMER_SourceMillis->PSC = 0;
	TIMER_SourceMillis->ARR = 0xFFFF;
	TIMER_SourceMicros->PSC = 35;
	TIMER_SourceMicros->ARR = 50000;
	
	TIMER_SourceMicros->CR1 |= TIM_CR1_CEN;
	TIMER_SourceMillis->CR1 |= TIM_CR1_CEN;
}

void TIMER_SetCLockSourceMillis(TIM_TypeDef* Timer)
{
	TIMER_SourceMillis = Timer;
}

void TIMER_SetCLockSourceMicros(TIM_TypeDef* Timer)
{
	TIMER_SourceMicros = Timer;
}

void TIMER_EnableInterrupt(TIM_TypeDef* Timer)
{
	
}

uint32_t TIMER_Milis(void)
{
	uint32_t a = TIMER_SourceMillis->CNT;
	uint32_t b = TIMER_SourceMicros->CNT / 1000;
	return  a + b;
}

uint32_t TIMER_Micros(void)
{
	uint32_t a = TIMER_SourceMillis->CNT * 1000;
	uint32_t b = TIMER_SourceMicros->CNT;
	return  a + b;
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/