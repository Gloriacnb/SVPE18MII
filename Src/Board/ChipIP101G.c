/*
 * ChipIP101G.c
 *
 *  Created on: 2017年8月18日
 *      Author: Administrator
 */
#include "../Board/ChipIP101G.h"
#include "../Board/ChipSE0164.h"

bool initIP101G(void) {
	if( readIP101G(IP101_PHY, 2, 0) != IP101G_PHYID1 ) {
		return false;
	}
	writeIP101G(IP101_PHY, 23, 1, 0);	//因为硬件没有上拉环回测试脚 INTR,在此用软件初始化
	return true;
}


uint16 readIP101G(uint8 phyaddr, uint8 regaddr, uint8 page) {
	if( page != 0 ) {
		writeMDIO(phyaddr, IP101_PAGE_REG, page);
	}
	return readMDIO(phyaddr, regaddr);
}
void writeIP101G(uint8 phyaddr, uint8 regaddr, uint8 page, uint16 regdata) {
	if( page != 0 ) {
		writeMDIO(phyaddr, IP101_PAGE_REG, page);
	}
	writeMDIO(phyaddr, regaddr, regdata);
}
