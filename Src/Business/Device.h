/*
 * Device.h
 *
 *  Created on: 2017年7月30日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_DEVICE_H_
#define SRC_BUSINESS_DEVICE_H_

#include "..\inc\type.h"
#define Production_date_bytes	4
#define Exfactory_date_bytes	4
#define Warranty_date_bytes		4
#define LastRepair_date_bytes	4
#define Device_serialnumber_bytes 2

#define FACTORY_INFO_BYTES	(Production_date_bytes +\
							 Exfactory_date_bytes  +\
							 Warranty_date_bytes   +\
							 LastRepair_date_bytes +\
							 Device_serialnumber_bytes)

typedef struct SYS_CFG_DATA {
	uint8 clock;		//0 主时钟，1恢复时钟
	uint8 ts[4];		//32bit 对应32个时隙 ts[0] bit0=0 成帧 bit0=1 透明
						//ts[0] 7~0, ts[1] 15~8, ts[2] 23~16, ts[3] 31~24
	uint8 crc;
}SYS_CFG;

uint8 getFactoryInfo(uint8* info);
uint8 setFactoryInfo(uint8* d, uint8 len);
uint16 getSerialNumber(void);

void initSysConfig(void);
bool setClockMode(uint8 mode);
uint8 getClockMode(void);

bool getTsSel(uint8* ts, uint8 len);
bool setTsSel(uint8* ts, uint8 len);
#endif /* SRC_BUSINESS_DEVICE_H_ */
