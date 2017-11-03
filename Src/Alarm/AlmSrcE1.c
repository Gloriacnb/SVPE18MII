/*
 * AlmSrcE1.c
 *
 *  Created on: 2017年9月5日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcE1.h"
#include "../inc/ErrCode.h"
#include "../Business/E1Port.h"
#include "../Board/ChipSE0164.h"

static bool ifE1Alarm(uint8 sid, ALM_TYPE* types);
//extern xdata uint8 looped = 0;

int initALMSrcE1(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	for (i = 0; i < count; ++i) {
		src[i].sid = E1_ASID_BASE + i;
		src[i].saveCfg = saveE1Cfg;
		src[i].typeCount = E1_TYPE_SIZE;
		src[i].getAlarm = ifE1Alarm;
		for (j = 0; j < E1_TYPE_SIZE; ++j) {
			src[i].type[j].tid = E1_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = getE1ConfigStruct(i)->attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

/*
 * 告警处理任务的回调函数，负责采集各个告警信号的最新状态
 */
bool ifE1Alarm(uint8 sid, ALM_TYPE* types) {
	int alarmd;
    if(SN(sid) != 0) {
        return false;
    }
	alarmd = getE1Alarm();
	types[E1_LOS].actState = ((alarmd & (1<<7)) != 0);
	types[E1_AIS].actState = ((alarmd & (1<<6)) != 0);
	types[E1_LOF].actState = ((alarmd & (1<<3)) != 0);
	types[E1_LOM].actState = ((alarmd & (1<<2)) != 0);
	types[E1_CV ].actState = ((alarmd & (1<<0)) != 0);
	types[E1_CRC].actState = ((alarmd & (1<<1)) != 0);
	types[E1_LOOP].actState = ((alarmd & (1<<4)) != 0);
//	if(types[E1_LOOP].actState) {
//		looped = 1;
//	}
//	else {
//		looped = 0;
//	}
	types[E1_RDI].actState = false;
	return true;
}

