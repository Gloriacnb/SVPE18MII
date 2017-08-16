/*
 * ChipSE0164.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_BOARD_CHIPSE0164_H_
#define SRC_BOARD_CHIPSE0164_H_

#include "..\inc\type.h"

uint8 readSE0164(uint16 regaddr);
void writeSE0164(uint16 regaddr, uint8 regdata);
bool writeSE0164Verify(uint16 regaddr, uint8 regdata, uint8 bitmask);
bool initSE0164(void);
void sendByteSDO0(uint8 byte);
uint8 getByteSDO0(void);
int getE1Alarm(uint8 portN);
uint16 readMDIO(uint8 phy, uint8 phyreg);
bool writeMDIO(uint8 phy, uint8 phyreg, uint16 regData);

/*for CMF 管理通道*/
void writeTCMFData(uint8 sn, uint8 b);
void setTCMFLength(uint8 len);
bool ifTCMFOver(void);
void startTCMF(void);

bool ifRCMFReady(void);
uint8 getRCMFLength(void);
uint8 readRCMFData(uint8 sn);

/*for E1时钟*/
void setE1ClockMode(uint8 mode);
void setE1RecoverClockSouce(uint8 e1port);

/*for GFP ALARM*/
bool ifGFPSyncLOSS(void);

#define SE0164_GLOBAL_ADDR_BASE								0x0000
#define SE0164_E1_ADDR_BASE									0x0100
#define SE0164_LCAS_ADDR_BASE									0x0200
#define SE0164_GFP_ADDR_BASE									0x0300
#define SE0164_ETH_ADDR_BASE									0x0400
#define SE0164_HDLC_ADDR_BASE									0x0500

#define SE0164_CHIPID			0x0165
#define SE0164_E1_MAXNUM		8

/************全局寄存器*************/
#define SE0164_GLOBAL_RSTLOOP1_REG								(SE0164_GLOBAL_ADDR_BASE + 0x00)
#define SE0164_GLOBAL_CHIP_IDH									(SE0164_GLOBAL_ADDR_BASE + 0x02)
#define SE0164_GLOBAL_CHIP_IDL									(SE0164_GLOBAL_ADDR_BASE + 0x03)
#define SE0164_GLOBAL_CHIP_VER									(SE0164_GLOBAL_ADDR_BASE + 0x04)

#define SE0164_GLOBAL_PHYADDR_REG								(SE0164_GLOBAL_ADDR_BASE + 0x0D)
#define SE0164_GLOBAL_PHYREGADDR_REG							(SE0164_GLOBAL_ADDR_BASE + 0x0E)
#define SE0164_GLOBAL_MDIOCONTROL_REG							(SE0164_GLOBAL_ADDR_BASE + 0x0F)
#define SE0164_GLOBAL_MDIOWDATH_REG							(SE0164_GLOBAL_ADDR_BASE + 0x10)
#define SE0164_GLOBAL_MDIOWDATL_REG							(SE0164_GLOBAL_ADDR_BASE + 0x11)
#define SE0164_GLOBAL_MDIORDATH_REG							(SE0164_GLOBAL_ADDR_BASE + 0x12)
#define SE0164_GLOBAL_MDIORDATL_REG							(SE0164_GLOBAL_ADDR_BASE + 0x13)

#define MDIO_START	(1<<7)
#define MDIO_WRITE	(1<<6)
#define MDIO_READY	(1<<0)

#define SE0164_GLOBAL_SDIO_OPT									(SE0164_GLOBAL_ADDR_BASE + 0x6F)
#define SE0164_GLOBAL_SDO0BYTE0_REG							(SE0164_GLOBAL_ADDR_BASE + 0x70)
#define SE0164_GLOBAL_SDO0BYTE1_REG							(SE0164_GLOBAL_ADDR_BASE + 0x71)
#define SE0164_GLOBAL_SDO0BYTE2_REG							(SE0164_GLOBAL_ADDR_BASE + 0x72)
#define SE0164_GLOBAL_SDO0BYTE3_REG							(SE0164_GLOBAL_ADDR_BASE + 0x73)

#define SE0164_GLOBAL_SDO1BYTE0_REG							(SE0164_GLOBAL_ADDR_BASE + 0x74)
#define SE0164_GLOBAL_SDO1BYTE1_REG							(SE0164_GLOBAL_ADDR_BASE + 0x75)
#define SE0164_GLOBAL_SDO1BYTE2_REG							(SE0164_GLOBAL_ADDR_BASE + 0x76)
#define SE0164_GLOBAL_SDO1BYTE3_REG							(SE0164_GLOBAL_ADDR_BASE + 0x77)

#define SE0164_GLOBAL_SDIBYTE0_REG								(SE0164_GLOBAL_ADDR_BASE + 0x7F)
#define SE0164_GLOBAL_SDIBYTE1_REG								(SE0164_GLOBAL_ADDR_BASE + 0x7E)
#define SE0164_GLOBAL_SDIBYTE2_REG								(SE0164_GLOBAL_ADDR_BASE + 0x7D)
#define SE0164_GLOBAL_SDIBYTE3_REG								(SE0164_GLOBAL_ADDR_BASE + 0x7C)

#define SE0164_GLOBAL_WLOCK_REG								(SE0164_GLOBAL_ADDR_BASE + 0xC0)

/***************LCAS寄存器****************/
#define SE0164_LCAS_TVCG_CFG									(SE0164_LCAS_ADDR_BASE + 0x10)
#define SE0164_LCAS_RVCG_CFG									(SE0164_LCAS_ADDR_BASE + 0x80)
#define SE0164_LCAS_TTRIB_USE_SQ(n)							(SE0164_LCAS_ADDR_BASE + 0x40 + (n) * 0x8)
#define SE0164_LCAS_RTRIB_USE_SQ(n)							(SE0164_LCAS_ADDR_BASE + 0xC0 + (n) * 0x8)


/***************E1支路寄存器***************/
#define SE0164_E1_ALARM1(n)										(SE0164_E1_ADDR_BASE + 0x00 + (n) * 0x10)
#define SE0164_E1_ALARM2(n)										(SE0164_E1_ADDR_BASE + 0x04 + (n) * 0x10)
#define SE0164_E1_CFG											(SE0164_E1_ADDR_BASE + 0xE1)
#define SE0164_E1_CFG2											(SE0164_E1_ADDR_BASE + 0xE2)

/***************GFP寄存器*****************/
#define SE0164_GFP_RCMF_READY									(SE0164_GFP_ADDR_BASE + 0x19)
#define SE0164_GFP_RCMF_DAT_INDEX								(SE0164_GFP_ADDR_BASE + 0x1A)
#define SE0164_GFP_RCMF_DAT									(SE0164_GFP_ADDR_BASE + 0x1B)
#define SE0164_GFP_RCMF_DAT_LEN								(SE0164_GFP_ADDR_BASE + 0x1C)

#define SE0164_GFP_TCMF_CTRL									(SE0164_GFP_ADDR_BASE + 0x0A)
#define SE0164_GFP_TCMF_DAT_INDEX								(SE0164_GFP_ADDR_BASE + 0x0C)
#define SE0164_GFP_TCMF_DAT									(SE0164_GFP_ADDR_BASE + 0x0D)
#define SE0164_GFP_TCMF_DAT_LEN								(SE0164_GFP_ADDR_BASE + 0x06)

#define SE0164_GFP_STA											(SE0164_GFP_ADDR_BASE + 0x11)
#define SE0164_GFP_ALARM										(SE0164_GFP_ADDR_BASE + 0x17)


void softReset(void);
uint16 readChipID(void);
bool setDefault(void);

#endif /* SRC_BOARD_CHIPSE0164_H_ */
