
#ifndef __CFG_VARIABLE_H__
#define __CFG_VARIABLE_H__

#include <stdint.h>

extern uint16_t record_gap;     // ��������¼���: unit: sec
extern uint16_t  cfgVar_LcdBackLightSec;  // LCD ������ʾʱ��, unit: sec
extern uint16_t cfgVar_FirstLcdBackLightSec; // �������һ����ʾʱ��
extern uint16_t cfgVar_SnsRemainRunTime;  // ������ʣ������ʱ��
extern uint8_t   cfgVar_DbgWritten;
extern uint16_t cfgVar_DiskFreeSize;
extern uint8_t  cfgVar_GetMac;   // �Ƿ��ȡmac��ַ
extern uint8_t  cfgVar_net_sel;

#if 1
#define  STR_FirstLcdBackLightSec   "FirstLcdBackLightSec=\0"
#define  STR_LcdBackLightSec            "LcdBackLightTime=\0"
#define  STR_SnsRemainRunTime      "SnsRemainRunTime=\0"
#else
extern const char STR_FirstLcdBackLightSec[];
extern const char STR_LcdBackLightSec[];
extern const char STR_SnsRemainRunTime[];
#endif

#endif

