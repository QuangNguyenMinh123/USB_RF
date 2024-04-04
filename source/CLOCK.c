#include "CLOCK.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
void CLOCK_Init(int crystalFreq)
{
	/* HSI selected as system clock */
	RCC->CFGR &= ~RCC_CFGR_SW;										/* RCC_CR->SW */
	while ((RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_HSI);		/* Wait until HW is switch to HSI */
	/* Disble PLL  */
	RCC->CR &= ~RCC_CR_PLLON;
	while ( (RCC->CR & RCC_CR_PLLRDY) != 0);
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
	/* ADC prescaler div 6 */
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV8;
	/* APB2 Prescaler = 1 -> 72 Mhz */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
	/* APB1 Prescaler = 2 -> 36 Mhz*/
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	/* Choose PLL entry clock source as HSE */
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
	/* PLL selected as system clock */
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);		/* Wait until HW is switch to PLL */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/