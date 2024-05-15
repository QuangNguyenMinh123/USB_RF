#include "nRF24L01.h"
#include "SPI.h"
#include "GPIO.h"
#include "CLOCK.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define NRF24L01_CE_PIN			PB1
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
static void nRF24L01_Write1Byte(uint8_t Register, uint8_t Data);
static void nRF24L01_WriteMulBytes(uint8_t Register, uint8_t *Data, uint8_t Size);
static uint8_t nRF24L01_Read1Byte(uint8_t Register);
static void nRF24L01_ReadMulBytes(uint8_t Register, uint8_t *Des, uint8_t Size);
static void nRF24L01_Enable(void);
static void nRF24L01_Disable(void);
static void TimerDelayUs(int time);
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
static int timer = 0;
/***********************************************************************************************************************
 * Static Function
 **********************************************************************************************************************/
static void nRF24L01_Write1Byte(uint8_t Register, uint8_t Data)
{
	SPI1_Write1Byte(nRF24_COMMAND_WRITE(Register), Data);
}

static void nRF24L01_WriteMulBytes(uint8_t Register, uint8_t *Data, uint8_t Size)
{
	SPI1_WriteMulBytes(nRF24_COMMAND_WRITE(Register), Data, Size);
}

static uint8_t nRF24L01_Read1Byte(uint8_t Register)
{
	return (SPI1_Read1Byte(nRF24_COMMAND_READ(Register)));
}

static void nRF24L01_ReadMulBytes(uint8_t Register, uint8_t *Des, uint8_t Size)
{
	SPI1_ReadMulBytes(nRF24_COMMAND_READ(Register), Des, Size);
}

static void nRF24L01_Enable(void)
{
	GPIO_PinHigh(NRF24L01_CE_PIN);
}

static void nRF24L01_Disable(void)
{
	GPIO_PinLow(NRF24L01_CE_PIN);
}

static void TimerDelayUs(int time)
{
	timer = micros();
	while (micros() - timer < time);
}

bool checkpointFail(uint8_t Register, uint8_t value)
{
	uint8_t Save = nRF24L01_Read1Byte(Register);
	if (Save != value)
		return 1;
	else
		return 0;
}
/***********************************************************************************************************************
 * Global Function
 **********************************************************************************************************************/
/* STATUS_REG co van de*/
void nRF24L01_Reset(uint8_t Register)
{
	uint8_t Buffer[5] = {0};
	uint8_t CheckpointFail = 0;
	if (Register == STATUS_REG)
	{
		nRF24L01_Write1Byte(STATUS_REG , 0x00);
	}
	else if (Register == FIFO_STATUS_REG)
	{
		nRF24L01_Write1Byte(FIFO_STATUS_REG, 0x11);
	}
	else if (Register == ALL_REGISTER)
	{
		nRF24L01_Write1Byte(CONFIG_REG, 0x08);
		CheckpointFail |= checkpointFail(CONFIG_REG, 0x08);
		nRF24L01_Write1Byte(EN_AA_REG, 0x3F);
		CheckpointFail |= checkpointFail(EN_AA_REG, 0x3F);
		nRF24L01_Write1Byte(EN_RXADDR_REG, 0x03);
		CheckpointFail |= checkpointFail(EN_RXADDR_REG, 0x03);
		nRF24L01_Write1Byte(SETUP_AW_REG, 0x03);
		CheckpointFail |= checkpointFail(SETUP_AW_REG, 0x03);
		nRF24L01_Write1Byte(SETUP_RETR_REG, 0x03);
		CheckpointFail |= checkpointFail(SETUP_RETR_REG, 0x03);
		nRF24L01_Write1Byte(RF_CH_REG, 0x02);
		CheckpointFail |= checkpointFail(RF_CH_REG, 0x02);
		nRF24L01_Write1Byte(RF_SETUP_REG, 0x0E);
		CheckpointFail |= checkpointFail(RF_SETUP_REG, 0x0E);
		nRF24L01_Write1Byte(STATUS_REG, 0x0E);
		CheckpointFail |= checkpointFail(STATUS_REG, 0x0E);
		nRF24L01_Write1Byte(OBSERVE_TX_REG, 0x00);
		CheckpointFail |= checkpointFail(OBSERVE_TX_REG, 0x00);
		nRF24L01_Write1Byte(RPD_REG, 0x00);
		CheckpointFail |= checkpointFail(RPD_REG, 0x00);
		Buffer[0] = Buffer[1] = Buffer[2] = Buffer[3] = Buffer[4] = 0xEF;
		nRF24L01_WriteMulBytes(RX_ADDR_P0_REG, Buffer, 5);
		Buffer[0] = Buffer[1] = Buffer[2] = Buffer[3] = Buffer[4] = 0xC2;
		nRF24L01_WriteMulBytes(RX_ADDR_P1_REG, Buffer, 5);
		nRF24L01_Write1Byte(RX_ADDR_P2_REG, 0xC3);
		CheckpointFail |= checkpointFail(RX_ADDR_P2_REG, 0xC3);
		nRF24L01_Write1Byte(RX_ADDR_P3_REG, 0xC4);
		CheckpointFail |= checkpointFail(RX_ADDR_P3_REG, 0xC4);
		nRF24L01_Write1Byte(RX_ADDR_P4_REG, 0xC5);
		CheckpointFail |= checkpointFail(RX_ADDR_P4_REG, 0xC5);
		nRF24L01_Write1Byte(RX_ADDR_P5_REG, 0xC6);
		CheckpointFail |= checkpointFail(RX_ADDR_P5_REG, 0xC6);
		Buffer[0] = Buffer[1] = Buffer[2] = Buffer[3] = Buffer[4] = 0xEF;
		nRF24L01_WriteMulBytes(TX_ADDR_REG, Buffer, 5);
		/* Checkpoint */
		Buffer[0] = Buffer[1] = Buffer[2] = Buffer[3] = Buffer[4] = 0;
		nRF24L01_ReadMulBytes(TX_ADDR_REG, Buffer, 5);
		if (Buffer[0] != 0xEF || Buffer[1] != 0xEF || Buffer[2] != 0xEF
			|| Buffer[3] != 0xEF || Buffer[4] != 0xEF)
		{
			while (1);
		}
		/*  End checkpoint */
		nRF24L01_Write1Byte(RX_PW_P0_REG, 0);
		CheckpointFail |= checkpointFail(RX_PW_P0_REG, 0);
		nRF24L01_Write1Byte(RX_PW_P1_REG, 0);
		CheckpointFail |= checkpointFail(RX_PW_P1_REG, 0);
		nRF24L01_Write1Byte(RX_PW_P2_REG, 0);
		CheckpointFail |= checkpointFail(RX_PW_P2_REG, 0);
		nRF24L01_Write1Byte(RX_PW_P3_REG, 0);
		CheckpointFail |= checkpointFail(RX_PW_P3_REG, 0);
		nRF24L01_Write1Byte(RX_PW_P4_REG, 0);
		CheckpointFail |= checkpointFail(RX_PW_P4_REG, 0);
		nRF24L01_Write1Byte(RX_PW_P5_REG, 0);
		CheckpointFail |= checkpointFail(RX_PW_P5_REG, 0);
		nRF24L01_Write1Byte(FIFO_STATUS_REG, 0x11);
		CheckpointFail |= checkpointFail(FIFO_STATUS_REG, 0x11);
		nRF24L01_Write1Byte(DYNPD_REG, 0);
		CheckpointFail |= checkpointFail(DYNPD_REG, 0);
		nRF24L01_Write1Byte(FEATURE_REG, 0);
		CheckpointFail |= checkpointFail(FEATURE_REG, 0);
		if (CheckpointFail > 0)
			while (1);
	}
}

void nRF24L01_Init(void)
{
	uint8_t CheckpointFail = 0;
	if (SPIInit_b == FALSE)
	{
		SPI1_Init();
		SPIInit_b = TRUE;
	}
	GPIO_SetOutPut(NRF24L01_CE_PIN, General_Push_Pull);
	nRF24L01_Disable();
	nRF24L01_Reset(ALL_REGISTER);
	nRF24L01_Write1Byte(CONFIG_REG, 0);
	CheckpointFail |= checkpointFail(CONFIG_REG, 0);
	nRF24L01_Write1Byte(EN_AA_REG, 0);
	CheckpointFail |= checkpointFail(EN_AA_REG, 0);
	nRF24L01_Write1Byte(EN_RXADDR_REG, 0);
	CheckpointFail |= checkpointFail(EN_RXADDR_REG, 0);
	nRF24L01_Write1Byte(SETUP_AW_REG, 0x03);
	CheckpointFail |= checkpointFail(SETUP_AW_REG, 0x03);
	nRF24L01_Write1Byte(SETUP_RETR_REG, 0);
	CheckpointFail |= checkpointFail(SETUP_RETR_REG, 0);
	nRF24L01_Write1Byte(RF_CH_REG, 0);
	CheckpointFail |= checkpointFail(RF_CH_REG, 0);
	nRF24L01_Write1Byte(RF_SETUP_REG, 0x0E);
	CheckpointFail |= checkpointFail(RF_SETUP_REG, 0x0E);
	if (CheckpointFail > 0)
		while (1);
	nRF24L01_Enable();
}

void nRF24L01_Command(uint8_t Command)
{
	SPI1_Start();
	SPI1->DR = Command;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
	SPI1_Stop();
}


void nRF24L01_TxMode(uint8_t *Address, uint8_t Channel)
{
	uint8_t Dummy;
	nRF24L01_Disable();
	nRF24L01_Write1Byte(RF_CH_REG, Channel);
	nRF24L01_WriteMulBytes(TX_ADDR_REG, Address, 5);
	Dummy = nRF24L01_Read1Byte(CONFIG_REG);
	Dummy &= ~0b10;
	Dummy = Dummy | (1<<1);
	Dummy = Dummy & 0xF2;
	nRF24L01_Write1Byte(CONFIG_REG, Dummy);
	TimerDelayUs(1500);
	nRF24L01_Enable();
}

bool nRF24L01_Transmit(uint8_t *Data)
{
	uint8_t FifoStatus = 0;
	SPI1_WriteMulBytes(nRF24_COMMAND_WRITE_TX_PAYLOAD, Data, 32);
	TimerDelayUs(1000);
	FifoStatus = nRF24L01_Read1Byte(FIFO_STATUS_REG);
	if ((FifoStatus & (1<<4)) && !((FifoStatus) & (1<<3)))
	{
		nRF24L01_Command(FLUSH_TX);
		nRF24L01_Reset(FIFO_STATUS_REG);
		return TRUE;
	}
	return FALSE;
}

void nRF24L01_RxMode(uint8_t *Address, uint8_t Channel)
{
	uint8_t Dummy;
	nRF24L01_Disable();
	nRF24L01_Write1Byte(RF_CH_REG, Channel);
	/* Select data pipe 1 */
	Dummy = nRF24L01_Read1Byte(EN_RXADDR_REG);
	Dummy |= (1<<1);
	nRF24L01_Write1Byte(EN_RXADDR_REG, Dummy);
	nRF24L01_WriteMulBytes(RX_ADDR_P1_REG, Address, 5);	/* Write Pipe 1 Address */
	nRF24L01_Write1Byte(RX_PW_P1_REG, 32);				/* 32 bit payload for pipe 2 */

	/* Power up device in Rx Mode */
	Dummy = nRF24L01_Read1Byte(CONFIG_REG);
	Dummy &= ~0b11;
	Dummy |= (1<<1) | (1<<0);
	nRF24L01_Write1Byte(CONFIG_REG, Dummy);
	nRF24L01_Enable();
}

bool nRF24L01_DataAvailable(uint8_t PipeIndex)
{
	uint8_t status = nRF24L01_Read1Byte(STATUS_REG);
	if ((status & 0b1110) != 0b1110)  /* If data is exist */
	{
		if ((status & (1<<6)) && (status & (PipeIndex<<1)) )
		{
			nRF24L01_Write1Byte(STATUS_REG, (1<<6));
			return 1;
		}
	}
	return 0;
}

void nRF24L01_ReceiveData(uint8_t *DestinationData)
{
	SPI1_ReadMulBytes(nRF24_COMMAND_READ_RX_PAYLOAD, DestinationData, 32);
	TimerDelayUs(1000);
	nRF24L01_Command(FLUSH_TX);
}

void nRF24L01_ReadAllData(uint8_t *Data)
{
	int i = 0;
	for (i=0; i<10; i++)
	{
		*(Data+i) = nRF24L01_Read1Byte(i);
	}
	nRF24L01_ReadMulBytes(RX_ADDR_P0_REG, (Data+ 10), 5);
	nRF24L01_ReadMulBytes(RX_ADDR_P1_REG, (Data+ 15), 5);

	*(Data+20) = nRF24L01_Read1Byte(RX_ADDR_P2_REG);
	*(Data+21) = nRF24L01_Read1Byte(RX_ADDR_P3_REG);
	*(Data+22) = nRF24L01_Read1Byte(RX_ADDR_P4_REG);
	*(Data+23) = nRF24L01_Read1Byte(RX_ADDR_P5_REG);

	nRF24L01_ReadMulBytes(RX_ADDR_P0_REG, (Data+ 24), 5);
	for (i=29; i<38; i++)
	{
		*(Data+i) = nRF24L01_Read1Byte(i-12);
	}
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/