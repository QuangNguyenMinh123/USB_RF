#include "nRF24L01.h"
#include "SPI.h"
#include "GPIO.h"
#include "CLOCK.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define RF_SETUP_250KB			0b00100000
#define RF_SETUP_1MB			0b00000000
#define RF_SETUP_2MB			0b00001000
#define RF_SETUP__18dBm			0b00000000
#define RF_SETUP__12dBm			0b00000010
#define RF_SETUP__6dBm			0b00000100
#define RF_SETUP_0dBm			0b00000110

#define SETUP_RETR_250uS		0b00000000
#define SETUP_RETR_500uS		0b00010000
#define SETUP_RETR_750uS		0b00100000
#define SETUP_RETR_1000uS		0b00000000
#define SETUP_RETR_1250uS		0b00000000
#define SETUP_RETR_1500uS		0b00000000
#define SETUP_RETR_1750uS		0b00000000
#define SETUP_RETR_2000uS		0b00000000
#define SETUP_RETR_2250uS		0b00000000
#define SETUP_RETR_2500uS		0b00000000
#define SETUP_RETR_2750uS		0b00000000
#define SETUP_RETR_3000uS		0b00000000
#define SETUP_RETR_3250uS		0b00000000
#define SETUP_RETR_3500uS		0b00000000
#define SETUP_RETR_3750uS		0b00000000
#define SETUP_RETR_4000uS		0b00000000
#define SETUP_RETR_0_RE_TRANSMIT		0b00000000
#define SETUP_RETR_1_RE_TRANSMIT		0b00000001
#define SETUP_RETR_2_RE_TRANSMIT		0b00000010
#define SETUP_RETR_3_RE_TRANSMIT		0b00000011
#define SETUP_RETR_4_RE_TRANSMIT		0b00000100
#define SETUP_RETR_5_RE_TRANSMIT		0b00000101
#define SETUP_RETR_6_RE_TRANSMIT		0b00000110
#define SETUP_RETR_7_RE_TRANSMIT		0b00000111
#define SETUP_RETR_8_RE_TRANSMIT		0b00001000
#define SETUP_RETR_9_RE_TRANSMIT		0b00001001
#define SETUP_RETR_10_RE_TRANSMIT		0b00001010
#define SETUP_RETR_11_RE_TRANSMIT		0b00001011
#define SETUP_RETR_12_RE_TRANSMIT		0b00001100
#define SETUP_RETR_13_RE_TRANSMIT		0b00001101
#define SETUP_RETR_14_RE_TRANSMIT		0b00001110
#define SETUP_RETR_15_RE_TRANSMIT		0b00001111
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/
void nRF24L01_Write1Byte(uint8_t Register, uint8_t Data);
static void nRF24L01_WriteMulBytes(uint8_t Register, uint8_t *Data, uint8_t Size);
uint8_t nRF24L01_Read1Byte(uint8_t Register);
void nRF24L01_ReadMulBytes(uint8_t Register, uint8_t *Des, uint8_t Size);
void nRF24L01_Enable(void);
void nRF24L01_Disable(void);
static void TimerDelayUs(int time);
/***********************************************************************************************************************
 * Variables
 **********************************************************************************************************************/
static int timer = 0;
/***********************************************************************************************************************
 * Static Function
 **********************************************************************************************************************/
 void nRF24L01_Write1Byte(uint8_t Register, uint8_t Data)
{
	SPI1_Write1Byte(nRF24_COMMAND_WRITE(Register), Data);
}

static void nRF24L01_WriteMulBytes(uint8_t Register, uint8_t *Data, uint8_t Size)
{
	SPI1_WriteMulBytes(nRF24_COMMAND_WRITE(Register), Data, Size);
}

 uint8_t nRF24L01_Read1Byte(uint8_t Register)
{
	return (SPI1_Read1Byte(nRF24_COMMAND_READ(Register)));
}

void nRF24L01_ReadMulBytes(uint8_t Register, uint8_t *Des, uint8_t Size)
{
	SPI1_ReadMulBytes(nRF24_COMMAND_READ(Register), Des, Size);
}

 void nRF24L01_Enable(void)
{
	GPIO_PinHigh(NRF24L01_CE_PIN);
}

 void nRF24L01_Disable(void)
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

void nRF24L01_Command(uint8_t Command)
{
	SPI1_Start();
	SPI1->DR = Command;
	while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE){}			/* Wait until bufer is empty */
	while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY){}
	SPI1_Stop();
}
/***********************************************************************************************************************
 * Global Function
 **********************************************************************************************************************/
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
		TimerDelayUs(100000);
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
		while (CheckpointFail >= 2);
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

	nRF24L01_Write1Byte(CONFIG_REG, 8);
	CheckpointFail |= checkpointFail(CONFIG_REG, 8);

	nRF24L01_Write1Byte(EN_AA_REG, 1);
	CheckpointFail |= checkpointFail(EN_AA_REG, 1);

	nRF24L01_Write1Byte(EN_RXADDR_REG, 1);
	CheckpointFail |= checkpointFail(EN_RXADDR_REG, 1);

	nRF24L01_Write1Byte(RF_SETUP_REG, RF_SETUP_2MB | RF_SETUP_0dBm);
	CheckpointFail |= checkpointFail(RF_SETUP_REG, RF_SETUP_2MB | RF_SETUP_0dBm);


	nRF24L01_Write1Byte(SETUP_RETR_REG,
						SETUP_RETR_750uS | SETUP_RETR_3_RE_TRANSMIT);
	CheckpointFail |= checkpointFail(SETUP_RETR_REG,
						SETUP_RETR_750uS | SETUP_RETR_3_RE_TRANSMIT);

	nRF24L01_Write1Byte(FEATURE_REG, 0);
	CheckpointFail |= checkpointFail(FEATURE_REG, 0);

	nRF24L01_Write1Byte(DYNPD_REG, 0);
	CheckpointFail |= checkpointFail(DYNPD_REG, 0);

	nRF24L01_Write1Byte(STATUS_REG, BIT_6);
	nRF24L01_Write1Byte(STATUS_REG, BIT_5);
	nRF24L01_Write1Byte(STATUS_REG, BIT_4);
	if (CheckpointFail > 1)
		while (1);
}

void nRF24L01_TxMode(uint8_t *Address, uint8_t Channel)
{
	nRF24L01_Disable();
	nRF24L01_Write1Byte(RF_CH_REG, Channel);
	nRF24L01_WriteMulBytes(TX_ADDR_REG, Address, 5);
	nRF24L01_WriteMulBytes(RX_ADDR_P0_REG, Address, 5);
	nRF24L01_WriteMulBytes(RX_ADDR_P1_REG, Address, 5);
	nRF24L01_Write1Byte(CONFIG_REG, 0xe);
	TimerDelayUs(10000);
	nRF24L01_Enable();
	TimerDelayUs(150);
}

void nRF24L01_Transmit(uint8_t *Data)
{
	nRF24L01_Disable();
	nRF24L01_Command(FLUSH_TX);
	nRF24L01_Write1Byte(STATUS_REG, STATUS_TX_DS);
	SPI1_WriteMulBytes(nRF24_COMMAND_WRITE_TX_PAYLOAD, Data, PACKET_SIZE);
	nRF24L01_Enable();
}

void nRF24L01_RxMode(uint8_t *Address, uint8_t Channel)
{
	nRF24L01_Disable();
	nRF24L01_Write1Byte(RF_CH_REG, Channel);
	nRF24L01_WriteMulBytes(TX_ADDR_REG, Address, 5);
	nRF24L01_WriteMulBytes(RX_ADDR_P0_REG, Address, 5);
	nRF24L01_Write1Byte(CONFIG_REG, 0xf | BIT_5 | BIT_4);
	nRF24L01_Write1Byte(RX_PW_P0_REG, PACKET_SIZE);
	TimerDelayUs(10000);
	nRF24L01_Enable();
	TimerDelayUs(150);
}

bool nRF24L01_DataAvailable(uint8_t PipeIndex)
{
	uint8_t status = nRF24L01_Read1Byte(STATUS_REG);
	if (status != 0xe)
		timer = 0;
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
	SPI1_ReadMulBytes(nRF24_COMMAND_READ_RX_PAYLOAD, DestinationData, PACKET_SIZE);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/