#ifndef _NRF24L01_H
#define _NRF24L01_H
#include "CLOCK.h"
/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define nRF24_COMMAND                   0xff
#define CONFIG_REG                      0x00
#define EN_AA_REG                       0x01
#define EN_RXADDR_REG                   0x02
#define SETUP_AW_REG                    0x03
#define SETUP_RETR_REG                  0x04
#define RF_CH_REG                       0x05
#define RF_SETUP_REG                    0x06
#define STATUS_REG                      0x07
#define OBSERVE_TX_REG                  0x08
#define RPD_REG                         0x09
#define RX_ADDR_P0_REG                  0x0A
#define RX_ADDR_P1_REG                  0x0B
#define RX_ADDR_P2_REG                  0x0C
#define RX_ADDR_P3_REG                  0x0D
#define RX_ADDR_P4_REG                  0x0E
#define RX_ADDR_P5_REG                  0x0F
#define TX_ADDR_REG                     0x10
#define RX_PW_P0_REG                    0x11
#define RX_PW_P1_REG                    0x12
#define RX_PW_P2_REG                    0x13
#define RX_PW_P3_REG                    0x14
#define RX_PW_P4_REG                    0x15
#define RX_PW_P5_REG                    0x16
#define FIFO_STATUS_REG                 0x17
#define ACK_PLD_REG                     nRF24_COMMAND
#define TX_PLD_REG                      nRF24_COMMAND
#define RX_PLD_REG                      nRF24_COMMAND
#define DYNPD_REG                       0x1C
#define FEATURE_REG                     0x1D
#define ALL_REGISTER					0xff
/* Instruction Mnemonics */
#define nRF24_COMMAND_READ(Reg)			(0b00000000 | (Reg & 0b00011111))
#define nRF24_COMMAND_WRITE(Reg)		(0b00100000 | (Reg & 0b00011111))
#define nRF24_COMMAND_READ_RX_PAYLOAD	0b01100001
#define nRF24_COMMAND_WRITE_TX_PAYLOAD	0b10100000
#define nRF24_COMMAND_FLUSH_TX			0b11100001
#define nRF24_COMMAND_FLUSH_RX			0b11100010
#define nRF24_COMMAND_REUSE_TX_PL		0b11100011
#define nRF24_COMMAND_R_RX_PL_WID		0b01100000
#define R_REGISTER                      0x00
#define W_REGISTER                      0x20
#define REGISTER_MASK                   0x1F
#define ACTIVATE                        0x50
#define R_RX_PL_WID                     0x60
#define R_RX_PAYLOAD                    0x61
#define W_TX_PAYLOAD                    0xA0
#define W_ACK_PAYLOAD                   0xA8
#define FLUSH_TX                        0xE1
#define FLUSH_RX                        0xE2
#define REUSE_TX_PL                     0xE3
#define NOP                             0xFF

#define NRF24L01_CE_PIN					PB1
#define NRF24L01_IRQ_PIN				PA3
/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/
void nRF24L01_Init(void);

void nRF24L01_Command(uint8_t Command);

void nRF24L01_RxMode(uint8_t *Address, uint8_t channel);

void nRF24L01_TxMode(uint8_t *Address, uint8_t channel);

void nRF24L01_Transmit(uint8_t *data);

bool nRF24L01_DataAvailable(uint8_t PipeIndex);

void nRF24L01_ReceiveData(uint8_t *DestinationData);

uint8_t nRF24L01_Read1Byte(uint8_t Register);
#endif  /* _NRF24L01_H */