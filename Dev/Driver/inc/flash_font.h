
#ifndef  __FLASH_FONT_H__
#define  __FLASH_FONT_H__


#include <stdint.h>



#pragma pack(1)

#define FONT_NAME_STR_LEN  20

// ���������ʼ����
#define  FLASH_FONT_LIB_START_SECTOR  (FLASH_FONT_START_SECTOR + 1)


// ������Ϣ����
typedef struct
{
       uint8_t     name_hash;  // font_name ��hash ����
       uint16_t   index;    // ����
       uint8_t    font_name[FONT_NAME_STR_LEN];
       uint16_t   d0;                // 1ά�����С
       uint16_t   d1;               // 2ά�����С
      // uint16_t   char_size;    // �����ַ����ȣ� B
       uint32_t   size;             // �����ܳ���
       uint32_t   offset;          // ��FLASH�еĵ�ַƫ��
       uint32_t   font_sum;    // ��������У���
       uint32_t   reserved;    // ����
       
       uint8_t     font_u8_sum;
       uint8_t     desc_sum;  // ����������Ϣ�ṹ���У���
}FONT_DESC;
#pragma pack()


void FlashFont_Task(void);

#endif

