
#if 0
/* --------------------------Includes ---------------------------------------------*/
#include "stm32f10x.h"
#include "ILI9341_4Line_SPI.h"
#include "TFT_API.h"
#include "UserGUI.h"
#include "Font1.h"
#include "delay.h"
#include "Picture.h"
#include "QDTFT_demo.h"
/* ----------------------End of Includes ---------------------------------------------*/

unsigned char Num[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

//���Ʋ��Բ˵�
//2D������ťʾ��
void Redraw_Mainmenu(void)
{

	UserGUI_LCDClear(GRAY0);
	
	UserGUI_DrawFont_GBK16(16,2,BLUE,GRAY0,"TFTҺ�����Թ���");
	UserGUI_DrawFont_GBK16(16,20,RED,GRAY0,"Һ�����Գ���");

	UserGUI_DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	UserGUI_DrawFont_GBK16(16,40,GREEN,GRAY0,"��ɫ������");

	UserGUI_DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	UserGUI_DrawFont_GBK16(16,70,BLUE,GRAY0,"������ʾ����");

	UserGUI_DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	UserGUI_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");

	//Gui_DrawFont_GBK16(16,120,BLUE,GRAY0,"Welcome");
	UserGUI_DrawFont_GBK24(16,140, BLUE, GRAY0, "Welcome");
	
	UserGUI_DrawFont_Num32(100,125, BLACK, GRAY0,Num[5]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, BLUE, GRAY0,Num[4]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, BROWN, GRAY0,Num[3]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, GREEN, GRAY0,Num[2]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, ORANGE, GRAY0,Num[1]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125,RED,GRAY0,Num[0]);	
}
//�������������
void Num_Test(void)
{
	u8 i = 0;
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	UserGUI_LCDClear(GRAY0);

	for(i = 0; i < 10; i++)
	{
	   UserGUI_DrawFont_Num32((i % 3) * 40, 32 * (i / 3) + 30, RED, GRAY0, Num[i]);
	   delay_ms(100);
	}
	
}
//��Ӣ����ʾ����
void Font_Test(void)
{
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16, 10, BLUE, GRAY0, "������ʾ����");

	delay_ms(1000);
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16, 30, RED,  GRAY0, "ȫ�����Ӽ���");
	UserGUI_DrawFont_GBK16(16, 50, BLUE, GRAY0, "רעҺ������");
	UserGUI_DrawFont_GBK16(16, 70, RED,  GRAY0, "ȫ�̼���֧��");
	UserGUI_DrawFont_GBK16(0, 100, BLUE, GRAY0, "Tel:15989313508");
	UserGUI_DrawFont_GBK16(0, 130, RED,  GRAY0, "www.qdtech.net");	
	delay_ms(1000);	
}

#include "os_global.h"
//��ˢ������
void Color_Test(void)
{
	u8 i = 1;
	
	UserGUI_LCDClear(GRAY0);
	
	UserGUI_DrawFont_GBK16(20, 10, BLUE, GRAY0, "��ɫ������");
	delay_ms(1200);

	while(i--)
	{
	    os_printf("before white, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		UserGUI_LCDClear(WHITE); 
		os_printf("after white, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		delay_ms(500);

		os_printf("before black, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		UserGUI_LCDClear(BLACK); 
		os_printf("after black, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		delay_ms(500);

		os_printf("before red, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		UserGUI_LCDClear(RED);	  
		os_printf("after red, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		
		delay_ms(500);
		
	  	UserGUI_LCDClear(GREEN); 
		delay_ms(500);
		
	  	UserGUI_LCDClear(BLUE);  
		delay_ms(500);
		
		UserGUI_LCDClear(YELLOW);  
		delay_ms(1000);
		
		UserGUI_LCDClear(CYAN);  
		delay_ms(1000);
		
		UserGUI_LCDClear(BROWN);  
		delay_ms(1000);
		
		UserGUI_LCDClear(BRRED);  
		delay_ms(1000);
		
		UserGUI_LCDClear(MAGENTA);  
		delay_ms(1000);
	}		
}








#include "font32_32_api.h"





u16 QDTFT_RGB_TO_COLOR(u32 rgb)
{
    u16 color;
	u8 r, g, b;

	r = (u8)((rgb & 0xFF0000) >> 16);
	g = (u8)((rgb & 0x00FF00) >> 8);
	b = (u8)(rgb & 0x0000FF);

	color = ILI9341_COLOR(r, g, b);

	return color;
}
#else
#include "stm32f10x.h"
#include "ILI9341_4Line_SPI.h"
#include "TFT_API.h"
#include "UserGUI.h"
#include "Font1.h"
#include "delay.h"
#include "Picture.h"
#include "QDTFT_demo.h"
/* ----------------------End of Includes ---------------------------------------------*/

unsigned char Num[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

//���Ʋ��Բ˵�
//2D������ťʾ��
void Redraw_Mainmenu(void)
{

	UserGUI_LCDClear(GRAY0);
	
	UserGUI_DrawFont_GBK16(16,2,BLUE,GRAY0,"TFTҺ�����Թ���");
	UserGUI_DrawFont_GBK16(16,20,RED,GRAY0,"Һ�����Գ���");

	UserGUI_DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
	UserGUI_DrawFont_GBK16(16,40,GREEN,GRAY0,"��ɫ������");

	UserGUI_DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
	UserGUI_DrawFont_GBK16(16,70,BLUE,GRAY0,"������ʾ����");

	UserGUI_DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
	UserGUI_DrawFont_GBK16(16,100,RED,GRAY0,"ͼƬ��ʾ����");

	//Gui_DrawFont_GBK16(16,120,BLUE,GRAY0,"Welcome");
	UserGUI_DrawFont_GBK24(16,140, BLUE, GRAY0, "Welcome");
	
	UserGUI_DrawFont_Num32(100,125, BLACK, GRAY0,Num[5]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, BLUE, GRAY0,Num[4]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, BROWN, GRAY0,Num[3]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, GREEN, GRAY0,Num[2]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125, ORANGE, GRAY0,Num[1]);
	delay_ms(1000);
	UserGUI_DrawFont_Num32(100,125,RED,GRAY0,Num[0]);	
}
//�������������
void Num_Test(void)
{
	u8 i = 0;
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	UserGUI_LCDClear(GRAY0);

	for(i = 0; i < 10; i++)
	{
	   UserGUI_DrawFont_Num32((i % 3) * 40, 32 * (i / 3) + 30, RED, GRAY0, Num[i]);
	   delay_ms(100);
	}
	
}
//��Ӣ����ʾ����
void Font_Test(void)
{
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16, 10, BLUE, GRAY0, "������ʾ����");

	delay_ms(1000);
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16, 30, RED,  GRAY0, "ȫ�����Ӽ���");
	UserGUI_DrawFont_GBK16(16, 50, BLUE, GRAY0, "רעҺ������");
	UserGUI_DrawFont_GBK16(16, 70, RED,  GRAY0, "ȫ�̼���֧��");
	UserGUI_DrawFont_GBK16(0, 100, BLUE, GRAY0, "Tel:15989313508");
	UserGUI_DrawFont_GBK16(0, 130, RED,  GRAY0, "www.qdtech.net");	
	delay_ms(1000);	
}

#include "os_global.h"
//��ˢ������
void Color_Test(void)
{
	u8 i = 1;
	
	UserGUI_LCDClear(GRAY0);
	
	UserGUI_DrawFont_GBK16(20, 10, BLUE, GRAY0, "��ɫ������");
	delay_ms(1200);

	while(i--)
	{
	    os_printf("before white, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		UserGUI_LCDClear(WHITE); 
		os_printf("after white, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		delay_ms(500);

		os_printf("before black, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		UserGUI_LCDClear(BLACK); 
		os_printf("after black, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		delay_ms(500);

		os_printf("before red, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		UserGUI_LCDClear(RED);	  
		os_printf("after red, tick = %ld, systick = %ld\n", os_get_tick(), SysTick->VAL);
		
		delay_ms(500);
		
	  	UserGUI_LCDClear(GREEN); 
		delay_ms(500);
		
	  	UserGUI_LCDClear(BLUE);  
		delay_ms(500);
		
		UserGUI_LCDClear(YELLOW);  
		delay_ms(1000);
		
		UserGUI_LCDClear(CYAN);  
		delay_ms(1000);
		
		UserGUI_LCDClear(BROWN);  
		delay_ms(1000);
		
		UserGUI_LCDClear(BRRED);  
		delay_ms(1000);
		
		UserGUI_LCDClear(MAGENTA);  
		delay_ms(1000);
	}		
}

//������ʾ����
//16λBMP 40X40 QQͼ��ȡģ����
//Image2LCDȡģѡ������
//ˮƽɨ��
//16λ
//40X40
//������ͼ��ͷ����
//��������
//�Զ�����
//��λ��ǰ
void showimage(const unsigned char *p) //��ʾ40*40 QQͼƬ
{
  	int i,j,k; 
	unsigned char picH,picL; 
	UserGUI_LCDClear(GRAY0);
	UserGUI_DrawFont_GBK16(16, 10, BLUE, GRAY0, "ͼƬ��ʾ����");
	delay_ms(1000);

	UserGUI_LCDClear(GRAY0);
	#if 1
	for(k = 0; k < Y_MAX_PIXEL / 40; k++)
	{
	   	for(j = 0; j < X_MAX_PIXEL / 40; j++)
		{	
			 UserGUI_SetRegion(40 * j, 40 * k, 40 * j + 39, 40 * k + 39); //��������
		     for(i = 0; i < 40 * 40; i++)
			 {	
			 	picL = *(p + i * 2);	//���ݵ�λ��ǰ
				picH = *(p + i * 2 + 1);				
				UserGUI_WriteData_16Bit(picH <<8 | picL);  		
			 }	
			 UserGUI_WriteData16Bit_End();
		 }
	}		
	#else
    

	do
	{


	   /*
       for(i = 0; i < 40 * 40; i++)
	   {	
	 	  picL = *(p + i * 2);	//���ݵ�λ��ǰ
		  picH = *(p + i * 2 + 1);		
          for(k = 0; k < Y_MAX_PIXEL / 40; k++)
          {
             for(j = 0; j < X_MAX_PIXEL / 40; j++)
		     {
		      UserGUI_DrawPoint(40 * j + i / 40, k * 40 + i % 40, picH << 8 | picL);
		     }
          }				
	   }*/	

	    for(k = 0; k < Y_MAX_PIXEL / 40; k++)
		{
		   	for(j = 0; j < X_MAX_PIXEL / 40; j++)
			{	
				 UserGUI_DrawRegion(40 * j, 40 * k, 40, 40, (u16 *)gImage_qq); //��������
			}
		}	
	}while(0);
	#endif
}

void QDTFT_PrintfASCII(void)
{
    uint8_t i = 0;

	for(i = ' '; i < 127; i++)
	{
	   os_printf("%c", i);
	}
}

//�ۺϲ��Ժ���
void QDTFT_Test_Demo(void)
{
	QDTFT_PrintfASCII();
	//LCD_LED_SET;//ͨ��IO���Ʊ�����				
	Redraw_Mainmenu();//�������˵�(�����������ڷֱ��ʳ�������ֵ�����޷���ʾ)

	os_printf("Mainmenu: read point err cnt = %ld, ok_cnt = %ld\n", rp_err_cnt, rp_ok_cnt);
	
	Color_Test();//�򵥴�ɫ������
	os_printf("Color_Test: read point err cnt = %ld, ok_cnt = %ld\n", rp_err_cnt, rp_ok_cnt);
	
	Num_Test();//������������
	os_printf("Num_Test: read point err cnt = %ld, ok_cnt = %ld\n", rp_err_cnt, rp_ok_cnt);
	
	Font_Test();//��Ӣ����ʾ����		
	os_printf("Font_Test: read point err cnt = %ld, ok_cnt = %ld\n", rp_err_cnt, rp_ok_cnt);
	
	showimage(gImage_qq);//ͼƬ��ʾʾ��
	os_printf("showimage: read point err cnt = %ld, ok_cnt = %ld\n", rp_err_cnt, rp_ok_cnt);
	
	delay_ms(1500);
	//LCD_LED_CLR;//IO���Ʊ�����	
}

#include "font_api.h"
void QDTFT_Demo(void)
{
    uint8_t i;
	
	UserGUI_LCDClear(DGRAY);
    FONT2416_DrawString(0, 0, BLUE, DGRAY, " !\"#$%&\'()*+,-.//0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

	for(i = 0; i < 10; i++)
	{
	   FONT3232_DrawNum(0, 144, WHITE, DGRAY, i);
	   delay_ms(1000);
	}	
	FONT3232_DrawNum(32, 144, WHITE, DGRAY, 10);
	FONT3232_DrawNum(64, 144, WHITE, DGRAY, 11);
	FONT3232_DrawNum(96, 144, WHITE, DGRAY, 12);
	FONT3232_DrawNum(128, 144, WHITE, DGRAY, 13);
	FONT3232_DrawNum(160, 144, WHITE, DGRAY, 14);
	for(i = 0; i < 7; i++)
	{
	   FONT3232_DrawNum(192, i * 32, WHITE, DGRAY, i);
	   
	}	
}




u16 QDTFT_RGB_TO_COLOR(u32 rgb)
{
    u16 color;
	u8 r, g, b;

	r = (u8)((rgb & 0xFF0000) >> 16);
	g = (u8)((rgb & 0x00FF00) >> 8);
	b = (u8)(rgb & 0x0000FF);

	color = ILI9341_COLOR(r, g, b);

	return color;
}

void QDTFT_DisplayString_PM2P5(u16 x, u16 y, u16 fc, u16 bc)
{
    FONT2416_DrawString(x,      y, fc, bc , "PM2");    // 5 * 16 + 40 = 120
    FONT2416_DrawString(x + 45, y, fc, bc , ".");  // 5 * 16 + 40 = 120
    FONT2416_DrawString(x + 48, y, fc, bc , "5");  // 5 * 16 + 40 = 120
}

void QDTFT_DisplaySensor(void)
{ 
    u16 bc = QDTFT_RGB_TO_COLOR(SkyBlue);
	u16 fc = QDTFT_RGB_TO_COLOR(MintCream);
	u16 data_x_offset = 110;   // ���ݵ�X����ʼλ��
	u16 unit_x_offset   = 200; // ��λ��X����ʼλ��
	u16 y_base_offset = 48;    // Y����ʼƫ��
    u16 sns_color = BLACK, unit_color = BLACK;
	
	
    UserGUI_LCDClear(bc );
	
    QDTFT_DisplayString_PM2P5(40, y_base_offset + 32 * 0, sns_color, bc);
    FONT2416_DrawString(40, y_base_offset + 32 * 1,  sns_color, bc , "PM10");  
	FONT2416_DrawString(40, y_base_offset + 32 * 2,  sns_color, bc , "CO");  
	FONT2416_DrawString(40, y_base_offset + 32 * 3,  sns_color, bc , "VOCs");  
	FONT2416_DrawString(40, y_base_offset + 32 * 4,  sns_color, bc , "Temp");  
	FONT2416_DrawString(40, y_base_offset + 32 * 5,  sns_color, bc , "RH..");  
	FONT2416_DrawString(40, y_base_offset + 32 * 6,  sns_color, bc , "HCHO");  

	FONT2416_DrawString(unit_x_offset,          y_base_offset + 4,  unit_color, bc , "ug/m");
	FONT2416_DrawUnit(unit_x_offset + 16 * 4,   y_base_offset + 4,  unit_color, bc ,  FONT_UNIT_Cube);

	FONT2416_DrawString(unit_x_offset,          y_base_offset + 32 * 1 + 4,  unit_color, bc , "ug/m");
	FONT2416_DrawUnit(unit_x_offset + 16 * 4,   y_base_offset + 32 * 1+ 4,  unit_color, bc ,  FONT_UNIT_Cube);

	FONT2416_DrawString(unit_x_offset,          y_base_offset + 32 * 2 + 4,   unit_color, bc , "ppm");  // CO 
	FONT2416_DrawString(unit_x_offset,          y_base_offset + 32 * 3 + 4,   unit_color, bc , "ppm");  // VOCs

	FONT2416_DrawUnit(unit_x_offset + 16,       y_base_offset + 32 * 4 + 4,   unit_color, bc , FONT_UNIT_Celsius);   // �¶ȵ�λ: 'C
	FONT2416_DrawUnit(unit_x_offset + 16,       y_base_offset + 32 * 5 + 4,   unit_color, bc , FONT_UNIT_Percent);   // ʪ��: %

    // HCHO: mg/m3
	FONT2416_DrawString(unit_x_offset,          y_base_offset + 32 * 6 + 4,  unit_color, bc , "mg/m");
	FONT2416_DrawUnit(unit_x_offset + 16 * 4,   y_base_offset + 32 * 6 + 4,  unit_color, bc ,  FONT_UNIT_Cube);

    // д����
	FONT_WriteInt(data_x_offset,                    y_base_offset + 0,      fc, bc , 24, 32, 123, 3);   // PM2.5
	FONT_WriteInt(data_x_offset,                    y_base_offset + 32 * 1, fc, bc , 24, 32, 110, 3);   // PM10
	FONT_WriteInt(data_x_offset,                    y_base_offset + 32 * 2, fc, bc , 24, 32, 109,  3);  // CO

	// VOCsŨ��ֵ: 0.01
	FONT3224_DrawNum(data_x_offset,                 y_base_offset + 32 * 3,  fc,  bc , 0);     // '0'
	FONT3224_DrawNum(data_x_offset + 21,            y_base_offset + 32 * 3,  fc, bc , 10);   // '.'
	FONT3224_DrawNum(data_x_offset + 21 + 12,       y_base_offset + 32 * 3,   fc, bc , 0);    // '0'
	FONT3224_DrawNum(data_x_offset + 21 + 12 + 24, y_base_offset + 32 * 3,  fc, bc , 1);    // '1'

    // �¶�ֵ
	FONT3224_DrawNum(data_x_offset,                 y_base_offset +  32 * 4,    fc, bc , 2);   // '2'
	FONT3224_DrawNum(data_x_offset + 24,            y_base_offset +  32 * 4,    fc, bc , 2);   // '2'
	FONT3224_DrawNum(data_x_offset + 24 + 21,       y_base_offset + 32 * 4,    fc, bc , 10);   // '.'
	FONT3224_DrawNum(data_x_offset + 24 + 21 + 12,  y_base_offset + 32 * 4,    fc, bc , 1);   // '1'

    // ʪ��
	FONT3224_DrawNum(data_x_offset,                  y_base_offset + 32 * 5,    fc, bc , 5);   
	FONT3224_DrawNum(data_x_offset + 24,             y_base_offset + 32 * 5,    fc, bc , 6);   

    // HCHO 
	//FONT3224_DrawNum(data_x_offset,                   32 * 6,     fc, bc, 0);    // '0'
	//FONT3224_DrawNum(data_x_offset + 21,              32 * 6,    fc, bc, 10);  // '.'
	//FONT3224_DrawNum(data_x_offset + 21 + 12,         32 * 6,    fc, bc, 0);   // '8'
	//FONT3224_DrawNum(data_x_offset + 21 + 12 + 24,    32 * 6,    fc, bc, 1);   // '1'
}

// ��������ͼ����ʾ
void QDTFT_SnsGraph(void)
{

}
#endif



