/*
 * BoardSVPE48D.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */
#include "BoardSVPE18MII.h"

#include "..\communication\console.h"
#include "..\communication\dcc.h"
#include "..\Board\LED.h"
#include "..\STCLib\GPIO.h"
#include "..\inc\taskID.h"
#include <stdio.h>
#include <RTX51TNY.H>
#include "../STCLib/delay.h"
#include "ChipSE0164.h"

void boardInit(void) {

	/*设置P0~P5均为准双向模式*/
	uint8 i = 0;
    GPIO_InitTypeDef info = {0};
    info.Pin = GPIO_Pin_All;
    info.Mode = GPIO_PullUp;
    for (i = 0; i < 6; ++i) {
		GPIO_Inilize(i, &info);
    }
    
    /*配置UART1为 8位数据位，一位停止位，波特率9600*/
	initConsole();
	printf("Wait Board power up!\r\n");
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	printf("Start initial board!\r\n");

	if( !initSE0164() ) {
        printf("Init SE0164 failed!!\r\n");
    }
    initDCC();
	initLEDs();
	printf("Board initial completed.\r\n");

//	boardAutoTest();
}
void boardAutoTest(void) {
	os_create_task(tsk_test);
	printf("Board test completed.\r\n");
}

