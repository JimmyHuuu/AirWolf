#include "cfg_variable.h"

/*
���±������ڴ��̸�Ŀ¼��config.txt�ļ��޸Ĵ�ֵ

*/
uint16_t record_gap = 20;  // ��¼���, Ĭ��ÿ�� 15 ���¼һ�δ���������
uint16_t  cfgVar_LcdBackLightSec  = 45; // LCD ������ʾ����ʱ��
uint16_t cfgVar_FirstLcdBackLightSec = 180;   // �������һ����ʾʱ��
uint16_t cfgVar_SnsRemainRunTime = 300;  // Ĭ�� 5 ����
uint8_t   cfgVar_DbgWritten = 0;  // �Ƿ�д�������Ϣ���ļ���
uint16_t cfgVar_DiskFreeSize = 0;   // �ļ�ϵͳ��Ҫ��������С���д�С, ��λKB

uint8_t cfgVar_GetMac = 0;   // �Ƿ��ȡmac��ַ

uint8_t  cfgVar_net_sel = 1;  // select network WIFI or GPRS: WIFI: 1;  GPRS: 2

//const char STR_FirstLcdBackLightSec[32] = "FirstLcdBackLightSec=\0";
//const char STR_LcdBackLightSec[32] =            "LcdBackLightTime=\0";
//const char STR_SnsRemainRunTime[32] =      "SnsRemainRunTime=\0";

