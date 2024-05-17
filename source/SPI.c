#include "SPI.h"
#include "CLOCK.h"
#include "GPIO.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define MS									3120U

#define SPI_CR1_BR_DIV_2					(0b000) << 3
#define SPI_CR1_BR_DIV_4					(0b001) << 3
#define SPI_CR1_BR_DIV_8					(0b010) << 3
#define SPI_CR1_BR_DIV_16					(0b011) << 3
#define SPI_CR1_BR_DIV_32					(0b100) << 3
#define SPI_CR1_BR_DIV_64					(0b101) << 3
#define SPI_CR1_BR_DIV_128					(0b110) << 3
#define SPI_CR1_BR_DIV_256					(0b111) << 3

#define GPIOB_CRL0_MODE_OUTPUT_2MHZ			(0b10)
#define GPIOB_CRL0_MODE_OUTPUT_10MHZ		(0b01)
#define GPIOB_CRL0_MODE_OUTPUT_50MHZ		(0b11)

#define DELAY								10
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
static uint16_t DummySpi = 0;
bool SPIInit_b = FALSE;
/***********************************************************************************************************************
 * Static function
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global function
 **********************************************************************************************************************/
void SPI1_Init(void)
{
	/* GPIO setting */
	/* PA5 SPI1_SCK: Master = Alternate function push-pull */
	GPIO_SetOutPut(PA5, Alternate_Push_Pull);
	/* PA7 SPI1_MOSI: Full duplex / master = Alternate function push-pull */
	GPIO_SetOutPut(PA7, Alternate_Push_Pull);
	/* PA6 SPI1_MISO: Full duplex / master = Input_Pullup */
	GPIOA->CRL |= (1<<26);    		// PA6 (MISO) Input mode (floating)
	/* PB0 GPIO_NSS: General_Push_Pull */
	GPIO_SetOutPut(PB0, General_Push_Pull);
	GPIOB->CRL &= ~0b11;
	GPIOB->CRL |= GPIOB_CRL0_MODE_OUTPUT_10MHZ;
	GPIO_PinHigh(PB0);
	/* Remap = 0 */
	AFIO->MAPR &= ~ AFIO_MAPR_SPI1_REMAP;
	/* Reset SPI1 */
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
	delay(1*MS);
	RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
	/* Endable SPI1 Clock */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	/* 1. Select the BR[2:0] bits to define the serial clock baud rate (see SPI_CR1 register). */
	/* SPI1 clock derived from APB2 = 72MHz */
	SPI1->CR1 |= SPI_CR1_BR_DIV_16;			/* 72 / 8 = 9MHz */
 	/* 2. Select the CPOL and CPHA bits to define one of the four relationships between the
 	data transfer and the serial clock/ */
	SPI1->CR1 &= ~SPI_CR1_CPOL;			/* CPOL = 0, clock idle low */
	SPI1->CR1 &= ~SPI_CR1_CPHA;			/* CPHA = 0 */
	/* 3. Set the DFF bit to define 8- or 16-bit data frame format */
	SPI1->CR1 &= ~SPI_CR1_DFF;			/* 8 bit data frame */
	/* 4. Configure the LSBFIRST bit in the SPI_CR1 register to define the frame format. */
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;		/* MSB first */
	/* 5. If the NSS pin is required in input mode, in hardware mode, connect the NSS pin to a
 	high-level signal during the complete byte transmit sequence. In NSS software mode,
 	set the SSM and SSI bits in the SPI_CR1 register. If the NSS pin is required in output
 	mode, the SSOE bit only should be set. */
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
	SPI1->CR2 |= SPI_CR2_SSOE;
	/* 6. The MSTR and SPE bits must be set (they remain set only if the NSS pin is connected
 	to a high-level signal). */
	SPI1->CR1 |= SPI_CR1_MSTR;			/* Master mode */
	SPI1->CR1 |= SPI_CR1_SPE;
	SPIInit_b = TRUE;
}


void SPI1_InterruptInit(void)
{

}

__inline void SPI1_Start(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR0;
}

__inline void SPI1_Stop(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS0;
}

void SPI1_Write1Byte(uint8_t Register, uint8_t Data)
{
	SPI1_Start();
	SPI1->DR = Register;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
	SPI1->DR = Data;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
	DummySpi = SPI1->DR;
	DummySpi = SPI1->SR;
	SPI1_Stop();
}

void SPI1_WriteMulBytes(uint8_t Register, uint8_t *Data, uint8_t Size)
{
	SPI1_Start();
	SPI1->DR = Register;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
	while (Size > 0)
	{
		SPI1->DR = *Data;
		while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
		Data++;
		Size --;
	}
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
	DummySpi = SPI1->DR;
	DummySpi = SPI1->SR;
	SPI1_Stop();
}

uint8_t SPI1_Read1Byte(uint8_t Register)
{
	uint8_t data = 0;
	SPI1_Start();
	SPI1->DR = Register;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	SPI1->DR = 0;
	while ((SPI1->SR & SPI_SR_RXNE) == 0) {}  				/* wait while buffer is empty */
	data = SPI1->DR;
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
	SPI1_Stop();
	return data;
}

void SPI1_ReadMulBytes(uint8_t Register, uint8_t *Des, uint8_t Size)
{
	SPI1_Start();
	SPI1->DR = Register;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	SPI1->DR = 0;
	while ((SPI1->SR & SPI_SR_RXNE) == 0) {}
	DummySpi = SPI1->DR;
	while (Size > 0)
	{
		SPI1->DR = 0;
		while ((SPI1->SR & SPI_SR_RXNE) == 0) {}  				/* wait while buffer is empty */
		while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
		*Des = SPI1->DR;
		Size--;
		Des++;
	}
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
	SPI1_Stop();
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/