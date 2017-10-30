/*
 * AlmSrcEth.c
 *
 *  Created on: 2017年9月6日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcEth.h"
#include "../inc/ErrCode.h"
#include "../Business/EthPort.h"
#include "../Board/ChipIP101G.h"

static bool ifEthAlarm(uint8 sid, ALM_TYPE* types);

int initALMSrcETH(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	for (i = 0; i < count; ++i) {
		src[i].sid = ETH_ASID_BASE + i;
		src[i].saveCfg = saveEthCfg;
		src[i].typeCount = ETH_TYPE_SIZE;
		src[i].getAlarm = ifEthAlarm;
		for (j = 0; j < ETH_TYPE_SIZE; ++j) {
			src[i].type[j].tid = ETH_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = getEthConfigStruct(i)->attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

/*
 * 告警处理任务的回调函数，负责采集各个告警信号的最新状态
 */
bool ifEthAlarm(uint8 sid, ALM_TYPE* types) {
	uint16 value = readIP101G(IP101_PHY, 1, 0);
	types[ETH_LNKDOWN].actState = (value & (1<<2))?true:false;
	value = readIP101G(IP101_PHY, 0, 0);
	types[ETH_SPEEDLOW].actState = (value & (1<<13))?false:true;
	types[ETH_HALFDUP].actState = (value & (1<<8))?false:true;
	return true;
}

