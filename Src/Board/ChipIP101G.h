/*
 * ChipIP101.h
 *
 *  Created on: 2017年8月18日
 *      Author: Administrator
 */

#ifndef SRC_BOARD_CHIPIP101G_H_
#define SRC_BOARD_CHIPIP101G_H_

#include "..\inc\type.h"

#define IP101_PHY	7
#define IP101_PAGE_REG 20
#define IP101G_PHYID1 0x0243

bool initIP101G(void);

uint16 readIP101G(uint8 phyaddr, uint8 regaddr, uint8 page);
void writeIP101G(uint8 phyaddr, uint8 regaddr, uint8 page, uint16 regdata);
#endif /* SRC_BOARD_CHIPIP101G_H_ */
