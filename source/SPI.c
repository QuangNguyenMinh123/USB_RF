#include "SPI.h"
#include "CLOCK.h"
#include "GPIO.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define MS								3120U
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
/*
PA5 SPI1_SCK
PA6 SPI1_MISO
PA7 SPI1_MOSI
PB0 GPIO_NSS
*/
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/
void SPI_Init(void)
{
	/* GPIO setting */
	/* PA5 SPI1_SCK: Master = Alternate function push-pull */
	GPIO_SetOutPut(PA5, Alternate_Push_Pull);
	/* PA7 SPI1_MOSI: Full duplex / master = Alternate function push-pull */
	GPIO_SetOutPut(PA7, Alternate_Push_Pull);
	/* PA6 SPI1_MISO: Full duplex / slave = Alternate function push-pull */
	GPIO_SetOutPut(PA6, Alternate_Push_Pull);
	/* PB0 GPIO_NSS: General_Open_Drain */
	GPIO_SetOutPut(PB0, General_Open_Drain);
	/* Remap = 0 */
	AFIO->MAPR &= ~ AFIO_MAPR_SPI1_REMAP;
	/* Reset SPI1 */
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	delay(1*MS);
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	/* Endable SPI1 Clock */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	/* Prior to changing the CPOL/CPHA bits the SPI must be disabled by resetting the SPE bit */\

	/* Master and slave must be programmed with the same timing mode */
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/