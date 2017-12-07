/*
 * ChipSE0164.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 *      SE0164是一款EOP芯片，完成E1/ETH的协议转换，是SVPE48D的核心芯片
 *      芯片与单片机通过并口相连，对其寄存器访问映射为扩展内存的访问
 */
#include "..\Board\ChipSE0164.h"
#include "..\STCLib\config.h"
#include "..\STCLib\delay.h"
#include "..\STCLib\GPIO.h"
#include "../STCLib/spi.h"
#include <stdio.h>
#include <RTX51TNY.H>
#include "..\inc\taskID.h"
#define SE0164_CS 		P37		//片选信号，低（0）有效
//static bool FIRST_MDIO = true;
/*
 * 读取SE0164寄存器
 * 		regaddr 寄存器地址
 * 		return 寄存器值
 */
uint8 readSE0164(uint16 regaddr) {
	uint8 regdata = 0;
	SPI_SS_2 = 0;
	SPDAT = (uint8)regaddr;					//读命令及寄存器地址
	while (!(SPSTAT & SPIF));			//等待spi操作完成
	SPSTAT = SPIF | WCOL;				//清除SPI状态位
	SPDAT = 0;							//写入任意值
	while (!(SPSTAT & SPIF));           //等待spi操作完成
	SPSTAT = SPIF | WCOL;               //清除SPI状态位
	regdata = SPDAT;					//读出寄存器值
	SPI_SS_2 = 1;
	return regdata;
}

/*
 * 写入SE0164寄存器
 * 		regaddr 寄存器地址
 * 		regdata 待写入数据
 *
 * 注意：SE0164有写保护寄存器，本函数自动解除写保护，写完成后打开写保护
 */
void writeSE0164(uint16 regaddr, uint8 regdata) {
	SPI_SS_2 = 0;
	SPDAT = regaddr | 0x80;	//写命令及寄存器地址
	while (!(SPSTAT & SPIF));			//等待spi操作完成
	SPSTAT = SPIF | WCOL;				//清除SPI状态位
	SPDAT = regdata;					//写入寄存器值
	while (!(SPSTAT & SPIF));           //等待spi操作完成
	SPSTAT = SPIF | WCOL;               //清除SPI状态位
	SPI_SS_2 = 1;
}

/*
 * 写入SE0164寄存器 并回读校验
 * 		regaddr 寄存器地址
 * 		regdata 待写入数据
 * 		bitmask 需要校验的bit位，只校验为1的
 *
 * 注意：SE0164有写保护寄存器，本函数自动解除写保护，写完成后打开写保护
 */

bool writeSE0164Verify(uint16 regaddr, uint8 regdata, uint8 bitmask) {
	uint8 regv = 0;
	writeSE0164(regaddr, regdata);
	regv = readSE0164(regaddr);
	return (regdata & bitmask) == (regv & bitmask);
}

/*
 * 芯片初始化函数
 *  首先初始化SPI控制器，访问SE0164使用SPI接口
 *  芯片软复位
 * 	读取芯片ID，判断芯片是否在位
 */
bool initSE0164(void) {
    SPI_InitTypeDef info = {0};
    info.SPI_Module 	= ENABLE;			//spi enable
    info.SPI_SSIG 		= DISABLE;			//由Mode选项确定主从
    info.SPI_FirstBit 	= SPI_MSB;			//每字节高位在前
    info.SPI_Mode 		= SPI_Mode_Master;	//工作在主模式
    info.SPI_CPOL 		= SPI_CPOL_Low;		//时钟信号空闲为低
    info.SPI_CPHA 		= SPI_CPHA_1Edge;	//下降沿送数，上升沿采数
    info.SPI_Interrupt 	= DISABLE;			//SPI中断禁能
    info.SPI_Speed 		= SPI_Speed_64;		//SPI时钟频率为CPU时钟4分频
    info.SPI_IoUse 		= SPI_P24_P23_P22_P21;
    SPI_Init(&info);
//	softReset();
	if( readChipID() != SE0164_CHIPID ) {
		return false;
	}
    return setDefault();
}


/*
 * 软复位芯片
 */
void softReset(void) {
	uint8 reg = readSE0164(SE0164_GLOBAL_CHIP_RST_EN);
	writeSE0164(SE0164_GLOBAL_CHIP_RST_EN, reg | 0x80);
    delay_ms(5*12);
	writeSE0164(SE0164_GLOBAL_CHIP_RST_EN, reg & 0x7F);
    delay_ms(5*12);
}

/*
 * 读芯片ID
 */
uint16 readChipID(void) {
	return (readSE0164(SE0164_GLOBAL_CHIP_IDH) << 8) | readSE0164(SE0164_GLOBAL_CHIP_IDL);
}

/*
 * 将芯片配置为默认状态
 * 配置IP101为工作状态**
 */
bool setDefault(void) {
	uint8 i = 0;
	writeSE0164(SE0164_GLOBAL_CHIP_RST_EN, 0x3F);//控制方式为寄存器输入，WAN侧以太侧发送接收使能，打开WAN侧环回检测
	writeSE0164(SE0164_GLOBAL_WAN_TS_CLK, 0x20);//WAN收发均采用E1 HDB3模式 TS选择bit模式
	writeSE0164(SE0164_GLOBAL_CFG_4, 0x98);//禁止带宽耦合，封包模式为GFP
	writeSE0164(SE0164_GLOBAL_CFG_5, 0x07);//PHY接口强制百兆全双工，禁止自动网管帧，禁止MAC地址过滤，SDRAM大容量，使能流控
	writeSE0164(0x06, 0xA7);//检测到环回时，禁止MII接口；GFP收管理帧到接收RAM；
	writeSE0164(0x07, 0xDD);//使能地址表老化和初始化；使能流控
	writeSE0164(0x0d, 0xC0); //选择网管帧发送接收均为数据格式标准网管帧
	writeSE0164(0x10, 0xFF);//时系选择
	writeSE0164(0x1e, 0x25);//最大帧长(高位)
	writeSE0164(0x1f, 0x81);//最大帧长(低位)
	return true;
}

///*
// * 串行输出Byte0到SDO0 用于E1 LED
// *
// */
//void sendByteSDO0(uint8 byte) {
//	writeSE0164(SE0164_GLOBAL_SDO0BYTE0_REG, byte);			//写入待发送字节
//	while( (readSE0164(SE0164_GLOBAL_SDIO_OPT) & 1) != 0 ); 	//判断是否忙
//	writeSE0164(SE0164_GLOBAL_SDIO_OPT, (1<<7) | (1<<6) );	//bit7 req; bit6 send
//}
//
///*
// * 串行输入Byte0 from SDO0
// */
//uint8 getByteSDO0(void) {
//	while( (readSE0164(SE0164_GLOBAL_SDIO_OPT) & 1) != 0 ); 	//判断是否忙
//	writeSE0164(SE0164_GLOBAL_SDIO_OPT, (1<<7) );	//bit7 req; bit6 send
//	return readSE0164(SE0164_GLOBAL_SDIBYTE0_REG);
//
//}

/*
 * 获取E1接口告警
 * portN E1序号 0~7
 * return
 * 			bit7 LOS
 * 			bit6 AIS
 * 			bit5 --
 * 			bit4 LOOP
 * 			bit3 LOF
 * 			bit2 LOMF
 * 			bit1 CRC
 * 			bit0 CV
 */
int getE1Alarm(void) {
	return (((readSE0164(SE0164_E1_ALARM2)<<1)&0xf0) | (readSE0164(SE0164_E1_ALARM1)&0x0f));
}

/*
 * 读smi
 * 	phy IP101G phy地址
 * 	phyreg IP101G 寄存器地址
 */
uint16 readMDIO(uint8 phy, uint8 phyreg) {
	writeSE0164(SE0164_GLOBAL_PHYADDR_REG, phy);
	writeSE0164(SE0164_GLOBAL_PHYREGADDR_REG, phyreg);
	writeSE0164(SE0164_GLOBAL_MDIOCONTROL_REG, MDIO_START);
//	if( !FIRST_MDIO ) {
//		while( (readSE0164(SE0164_GLOBAL_MDIOCONTROL_REG) & MDIO_READY) == 0 );
//	}
//	else {
//		FIRST_MDIO = false;
//		delay_ms(5);
//	}
	delay_ms(1);
	return (readSE0164(SE0164_GLOBAL_MDIORDATH_REG)<<8) | readSE0164(SE0164_GLOBAL_MDIORDATL_REG);
}

/*
 * 写smi
 *
 */
bool writeMDIO(uint8 phy, uint8 phyreg, uint16 regData) {
	if(phy > 0x1f) {
		return false;
	}
	writeSE0164(SE0164_GLOBAL_PHYADDR_REG, phy);
	writeSE0164(SE0164_GLOBAL_PHYREGADDR_REG, phyreg);
	writeSE0164(SE0164_GLOBAL_MDIOWDATH_REG, (regData>>8) & 0xff);
	writeSE0164(SE0164_GLOBAL_MDIOWDATL_REG, regData & 0xff);
//	if( !FIRST_MDIO ) {
//		while( (readSE0164(SE0164_GLOBAL_MDIOCONTROL_REG) & MDIO_READY) == 0 );
//	}
//	else {
//		FIRST_MDIO = false;
//		delay_ms(1);
//	}
	writeSE0164(SE0164_GLOBAL_MDIOCONTROL_REG, MDIO_START | MDIO_WRITE);
	delay_ms(1);
	return true;
}

/*for cmf 读写*/
void writeTCMFData(uint8 sn, uint8 b) {
	writeSE0164(SE0164_NMI_DATA, b);
	writeSE0164(SE0164_NMI_TRSEL, sn | (3 << 6));
}

void setTCMFLength(uint8 len) {
	writeSE0164(SE0164_NMI_TX, len);
}
bool ifTCMFOver(void) {
	return (readSE0164(SE0164_NMI_TX) & (1<<6)) != 0;
}
void startTCMF(void) {
	uint16 regv = readSE0164(SE0164_NMI_TX);

	writeSE0164(SE0164_NMI_TX, regv | (1<<7));
}

bool ifRCMFReady(void) {
	uint8 v = readSE0164(SE0164_NMI_RX);
	return (v & 0x80) != 0;
}
uint8 getRCMFLength(void) {
	return readSE0164(SE0164_NMI_RX) & 0x3f;
}
uint8 readRCMFData(uint8 sn) {
	writeSE0164(SE0164_NMI_TRSEL, sn);
	return readSE0164(SE0164_NMI_DATA);
}

void readRCMFOver(void) {
	writeSE0164(SE0164_GFP_TCMF_DAT_INDEX, 0);
}
/*
 * 设置E1时钟模式，
 * 	mode 0 主时钟
 * 		 1恢复时钟
 */
void setE1ClockMode(uint8 mode) {
	uint8 v = readSE0164(SE0164_GLOBAL_WAN_TS_CLK);
	if( mode == 0 ) {
		//主时钟
		v &= ~(1<<CLOCK_SEL);
	}
	else {
		v |= (1<<CLOCK_SEL);
	}
	writeSE0164(SE0164_GLOBAL_WAN_TS_CLK, v);
}

/*
 * 恢复时钟支路选择
 * 	e1port 0~7
 */
//void setE1RecoverClockSouce(uint8 e1port) {
//	uint8 v = readSE0164(SE0164_E1_CFG);
//	v &= ~0xf;
//	v |= (e1port & 0xf);
//	writeSE0164(SE0164_E1_CFG2, v);
//}

void writeTsSel(uint8* ts, uint8 len) {
	uint8 i = 0;
	if( (ts != 0) || (len > 4) ) {
		for (i = 0; i < 4; ++i) {
			writeSE0164(0x13-i, ts[i]);
		}
	}
}


bool ifGFPSyncLOSS(void) {
	uint8 v = readSE0164(SE0164_GFP_STA);
	return ((v >> 5) & 1) != 0;
}

void testChipSE0164(void) _task_ tsk_test {
	xdata uint16 chipID = 0x0165;
	while(1) {
	    os_delete_task (tsk_test);            	//stop init task (no longer needed)
	}
}
