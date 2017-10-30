/*
 * AlmSrcVCG.h
 *
 *  Created on: 2017年9月8日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCVCG_H_
#define SRC_ALARM_ALMSRCVCG_H_

#include "../Alarm/AlarmDefine.h"

enum {
	GFP_SYNCLOS,
	VCG_TYPE_SIZE,
};//定义E1的告警类型


int initALMSrcVCG(ALM_SRC* src, uint8 cout);

#endif /* SRC_ALARM_ALMSRCVCG_H_ */
