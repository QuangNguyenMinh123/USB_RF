#include "CLOCK.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define SYSTICK_ARR_VALUE				9000000U
#define NVIC_PRIO_BITS		          	4U
#define MS								3120U
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
static uint32_t ui32micros = 0;
/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
static void delay(uint32_t delayTime) {
	uint32_t ui32Cnt = 0U;
	for (; ui32Cnt< delayTime; ui32Cnt++) {
		__asm("nop");                                                                                                                               	}
}

void CLOCK_Init(int crystalFreq)
{
	/* HSI selected as system clock */
	RCC->CFGR &= ~RCC_CFGR_SW;										/* RCC_CR->SW */
	delay(5*MS);
	/* Disble PLL  */
	RCC->CR &= ~RCC_CR_PLLON;
	while ( (RCC->CR & RCC_CR_PLLRDY) != 0);
	delay(MS);
	/* OSC IN divide by 2 */
	if (crystalFreq == 16)
	{
		RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
		RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_Div2;
	}
	else if (crystalFreq == 8)
	{
		RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
	}
	delay(MS);
	/* ADC prescaler div 6 */
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
	delay(MS);
	/* APB2 Prescaler = 1 -> 72 Mhz */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	delay(MS);
	/* APB1 Prescaler = 2 -> 36 Mhz*/
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	delay(MS);
	/* Choose PLL entry clock source as HSE */
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;
	delay(MS);
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
	delay(MS);
	/* PLL selected as system clock */
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);		/* Wait until HW is switch to PLL */
	delay(MS);
}

void CLOCK_SystickInit(void) {
	/* https://developer.arm.com/documentation/dui0552/a/cortex-m3-peripherals/system-timer--systick/systick-control-and-status-register */
	ui32micros=0U;
	/* Program the reload value */
	/* SysTick Reload Value Register */
	/* 8997620 */
	SysTick->LOAD |= (SYSTICK_ARR_VALUE - 1); /* Minus 1 because counter count down to 0 */
	SysTick->VAL   = 0UL;

	NVIC_SetPriority ((IRQn_Type)SysTick_IRQ, (1UL << NVIC_PRIO_BITS) - 1UL);

	/* SysTick Control and Status Register */
	/* bit 0: enable; bit 1: IRQ enable, systick clock = 72M / 8 = 9M */
	SysTick->CTRL = 0;
	SysTick->CTRL |= BIT_0 | BIT_1;
}

uint32_t micros(void) {
	return ( ui32micros + ((SYSTICK_ARR_VALUE - SysTick->VAL) / 9) );
}

/**************************SYSTICK HANDLER*************************************/
void SysTick_Handler(void) {
	ui32micros += (SYSTICK_ARR_VALUE / 9) + 1;
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/