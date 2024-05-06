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

#define SPI_COMMAND_READ(Reg)				(0b00000000 | (Reg & 0b00011111))
#define SPI_COMMAND_WRITE(Reg)				(0b00100000 | (Reg & 0b00011111))
#define SPI_COMMAND_READ_RX_PAYLOAD			0b01100001
#define SPI_COMMAND_WRITE_TX_PAYLOAD		0b10100000
#define SPI_COMMAND_FLUSH_TX				0b11100001
#define SPI_COMMAND_FLUSH_RX				0b11100010
#define SPI_COMMAND_REUSE_TX_PL				0b11100011
#define SPI_COMMAND_R_RX_PL_WID				0b01100000

#define GPIOB_CRL0_MODE_OUTPUT_2MHZ			(0b10)
#define GPIOB_CRL0_MODE_OUTPUT_10MHZ		(0b01)
#define GPIOB_CRL0_MODE_OUTPUT_50MHZ		(0b11)
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
uint16_t DummySpi = 0;
/***********************************************************************************************************************
 * Static function
 **********************************************************************************************************************/
__inline static void SPI1_Start(void)
{
	GPIO_PinLow(PB0);
}

__inline static void SPI1_Stop(void)
{
	GPIO_PinHigh(PB0);
}
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
	/* PB0 GPIO_NSS: General_Open_Drain */
	GPIO_SetOutPut(PB0, General_Open_Drain);
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
	SPI1->CR1 |= SPI_CR1_BR_DIV_8;			/* 72 / 16 = 4MHz */
 	/* 2. Select the CPOL and CPHA bits to define one of the four relationships between the
 	data transfer and the serial clock/ */
	SPI1->CR1 &= ~SPI_CR1_CPOL;			/* CPOL = 0, clock idle low */
	SPI1->CR1 &= ~SPI_CR1_CPHA;			/* CPHA = 0 */
	/* 3. Set the DFF bit to define 8- or 16-bit data frame format */
	SPI1->CR1 |= SPI_CR1_DFF;			/* 16 bit data frame */
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
}

void SPI1_InterruptInit(void)
{

}

void SPI1_Send1Byte(uint16_t Register, uint8_t Data)
{
	GPIOB->BSRR |= GPIO_BSRR_BR0;
	SPI1->DR = SPI_COMMAND_WRITE(Register) << 8 | Data;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
	DummySpi = SPI1->DR;
	DummySpi = SPI1->SR;
	GPIOB->BSRR |= GPIO_BSRR_BS0;
}

uint16_t SPI1_Read1Byte(uint16_t Register, uint16_t* Des)
{
	uint16_t data = 0;
	GPIOB->BSRR |= GPIO_BSRR_BR0;
	SPI1->DR = SPI_COMMAND_READ(Register) << 8;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}
	while ((SPI1->SR & SPI_SR_RXNE) == 0) {}  				/* wait while buffer is empty */
	data = (SPI1->DR);
	GPIOB->BSRR |= GPIO_BSRR_BS0;
	return data;
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/