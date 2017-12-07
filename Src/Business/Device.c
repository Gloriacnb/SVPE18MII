/*
 * Device.c
 *
 *  Created on: 2017年7月30日
 *      Author: Administrator
 */


#include "../Business/Device.h"
#include "../Business/ConfigData.h"
#include "../inc/ErrCode.h"
#include "../Board/ChipSE0164.h"
#include "../inc/taskID.h"

static const uint8 DEVICE_TYPE[] = {0x30, 0x11};
static const uint8 HARDWARE_VER[] = {1,2};
static const uint8 SOFTWARE_VER[] = {1,0,4};

static xdata SYS_CFG CFG_DATA = {0};

uint8 getFactoryInfo(uint8* info) {
	uint8 dataLen = 0;
	char i;
	if( info == 0 ) {
		return 0;
	}
	for (i = 0; i < sizeof(DEVICE_TYPE); ++i) {
		info[dataLen++] = DEVICE_TYPE[i];
	}
	for (i = 0; i < sizeof(HARDWARE_VER); ++i) {
		info[dataLen++] = HARDWARE_VER[i];
	}
	for (i = 0; i < sizeof(SOFTWARE_VER); ++i) {
		info[dataLen++] = SOFTWARE_VER[i];
	}
	readConfig(DEV_ATTR_SECTOR, &info[dataLen], FACTORY_INFO_BYTES);
	return dataLen + FACTORY_INFO_BYTES;
}

uint8 setFactoryInfo(uint8* d, uint8 len) {
	if( d == 0 || len < FACTORY_INFO_BYTES ) {
		return ERR_INPUT;
	}
	return saveConfig(DEV_ATTR_SECTOR, d, len);
}

uint16 getSerialNumber(void) {
	uint8 info[FACTORY_INFO_BYTES];
	readConfig(DEV_ATTR_SECTOR, info, FACTORY_INFO_BYTES);
	return (info[FACTORY_INFO_BYTES-2] << 8) | info[FACTORY_INFO_BYTES-1];
}

void initSysConfig(void) {
	readConfig(SYS_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(CFG_DATA));
	if( CFG_DATA.crc != 0x5A ) {
		CFG_DATA.clock = 0;
		CFG_DATA.ts[0] = 0xfe;
		CFG_DATA.ts[1] = 0xff;
		CFG_DATA.ts[2] = 0xff;
		CFG_DATA.ts[3] = 0xff;
		CFG_DATA.crc = 0x5A;
		saveConfig(SYS_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(CFG_DATA));
	}
	setE1ClockMode(CFG_DATA.clock);
	writeTsSel(CFG_DATA.ts, 4);
}

bool setClockMode(uint8 mode) {
	setE1ClockMode(mode);
	CFG_DATA.clock = mode;
	CFG_DATA.crc = 0x5A;
	saveConfig(SYS_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(CFG_DATA));
	return true;
}

uint8 getClockMode(void) {
	return CFG_DATA.clock;
}

bool getTsSel(uint8* ts, uint8 len) {
	uint8 i = 0;
	if( ts == 0 || len > 4 ) {
		return false;
	}
	for (i = 0; i < len; ++i) {
		ts[i] = CFG_DATA.ts[i];
	}
	return true;
}

bool setTsSel(uint8* ts, uint8 len) {
	uint8 i = 0;
	if( ts == 0 || len < 4 ) {
		return false;
	}
	writeTsSel(ts, len);
	for (i = 0; i < len; ++i) {
		CFG_DATA.ts[i] = ts[i];
	}
	saveConfig(SYS_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(CFG_DATA));
	return true;

}

