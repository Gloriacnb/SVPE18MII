/*
 * ConfigModule.c
 *
 *  Created on: 2017年9月13日
 *      Author: Administrator
 */

#include "../Business/ConfigModule.h"
#include "../Business/E1Port.h"
#include "../Business/VCG.h"
#include "../Business/EthPort.h"
#include "../Business/Device.h"

void initConfigModule(void) {
	initSysConfig();
	initE1PortConfig();
	initVCGConfig();
	initEthPortConfig();
}
