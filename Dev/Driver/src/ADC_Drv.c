
#include "ADC_Drv.h"
#include "board_version.h"


#include <stdio.h>
#include <stdarg.h>
#include "delay.h"
#include "FONT_API.h"

#if   DEBUG_ADC_EN
//#define ADC_DEBUG(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define  ADC_DEBUG  dbg_print_detail
#else
#define ADC_DEBUG(...)
#endif


// �����ѹֵ
// ��ʵ��ֵ��1000��, ������3λС��תΪ����
#define  GetVoltValue(adc_val) \
	((uint16_t)((double)adc_val * 3300 / 4096))  

// �����ص�ѹ
// ����: uint16_t volt: ADC�����ĵ�ѹ, ��λ: mV
// ����ֵ: uint16_t ��ص�ѹ: ��λ: mV
#define  GetBatVolt(volt)   ((uint16_t)(( 635.0 / 470.0) * (volt)))




#define  ADC_DMA_MODE_EN    1    // ADC DMA ģʽʹ��(1)


#define  N   10    // ÿͨ���� 10 ��
#define  M   3     // 3 ��ͨ��

vu16 AD_Value[N][M];    // ������� ADC ת�������Ҳ�� DMA ��Ŀ���ַ
uint16_t aver_volt[M];           // �ܽŵ�ƽ����ѹֵ, ����ADֵ, unit: mV


#if DEBUG_ADC_EN
#if 1
static const 
#else
static __eeprom	
#endif

uint8_t AdcChannelString[][12] = {
{"BAT_ADC  "},    // 0
{"Light_1  "},    // 1

{"TempSensor"},    // 2
//{"Vrefint   "},    // 3, �ڲ��ο���ѹֵһ��Ϊ 1.16 V - 1.26 V
};
#endif

typedef enum
{
	E_BAT_ADC   = 0,
	E_LIGHT_1    = 1,
	E_TEMP_SNS = 2,  // MCU �ڲ��¶ȴ�����
	E_Vrefint,  // MCU ADC �ڲ��ο���ѹ
}E_ADC_CH_NAME;

// ���ADC1 ת������ļĴ����ĵ�ַ�� ADC1->DR
// ��ַ����: 0x40000000 + 0x10000 + 0x2400 + 0x4C = 0x40012400 + 0x4C
// 0x4C = 76, 76 / 4 = 19, ��DR�Ĵ����� ADC1 ����ַ��ƫ����19���Ĵ���(һ���Ĵ���Ϊ32λ)

void adc_gpio_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  //ģ����������


	 // ��ص�ѹ��������ܽ�
       GPIO_InitStructure.GPIO_Pin  = BAT_ADC_Pin;
       GPIO_Init(BAT_ADC_PORT, &GPIO_InitStructure);
	 

        // Light 1 ��������������
	// GPIO_InitStructure.GPIO_Pin  = LIGHT_1_Pin;
       // GPIO_Init(LIGHT_1_PORT, &GPIO_InitStructure);
}

// ADC ����ʹ��
void adc_peripheral_enable(void)
{
	// ���� ADC �� DMA ֧��(Ҫʵ�� DMA ���ܣ������������ DMA ͨ���Ȳ���)
    ADC_DMACmd(ADC1, ENABLE);

    ADC_Cmd(ADC1, ENABLE);  // ʹ��ָ���� ADC1

    ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼  
    while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����

    ADC_StartCalibration(ADC1); //����ADУ׼
    while(ADC_GetCalibrationStatus(ADC1));  //�ȴ�У׼����
}

void adc_peripheral_disable(void)
{
    ADC_DMACmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);  // ��ָֹ���� ADC1
	     
    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, DISABLE);   // ��ֹDMA ��������ж�
    DMA_Cmd(DMA1_Channel1, DISABLE); //��ֹDMA ͨ��
}

// ADC DMA ģʽ��ʼ��
void adc_dma_mode_init(void)
{
    ADC_InitTypeDef ADC_InitStructure;

	ADC_DeInit(ADC1);  //����Ϊȱʡֵ

	/*ADC1configuration ----------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // ADC ����ģʽ:ADC1 �� ADC2�����ڶ���ģʽ
    
    ADC_InitStructure.ADC_ScanConvMode        = ENABLE;  // ɨ��ģʽ, ���Զ�ͨ������
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //DISABLE;  // ����ת����ֹ, ��ֻ����ת��
    ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None; // �ⲿ�����ر�
    ADC_InitStructure.ADC_DataAlign           = ADC_DataAlign_Right;       //  ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel        = M;      //˳����й���ת���� ADC ͨ������Ŀ
    
    ADC_Init(ADC1, &ADC_InitStructure);        //���� �Ĵ���

	/*ADC1regularchannel11configuration*/
     //����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
     //ADC1, ADC ͨ�� x,�������˳��ֵΪ y,����ʱ��Ϊ 239.5 ����
     ADC_RegularChannelConfig(BAT_ADC_x,              BAT_ADC_Channel,            1,   ADC_SampleTime_239Cycles5);
     ADC_RegularChannelConfig(LIGHT_1_ADC_x,          LIGHT_1_Channel,       2,   ADC_SampleTime_239Cycles5);
     ADC_RegularChannelConfig(ADC1,                    ADC_Channel_TempSensor,  3,  ADC_SampleTime_239Cycles5);
     //ADC_RegularChannelConfig(ADC1,                    ADC_Channel_Vrefint,      4,  ADC_SampleTime_239Cycles5);

    /* Enable the temperature sensor and vref internal channel */
	ADC_TempSensorVrefintCmd(ENABLE);   // �¶ȴ�����, �ڲ��ο���ѹ����ʹ��
	//adc_peripheral_enable();             // ADC ����ʹ��
}

static void adc_dma_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	
    DMA_DeInit(DMA1_Channel1); // ��λ�Ĵ���
    
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR); // DMA ����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr      = (u32)&AD_Value;    // DMA �ڴ����ַ
    DMA_InitStructure.DMA_DIR                  =  DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize          =  N * M;   // ָ�� DMAͨ�������С
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;  //����Ĵ�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;        //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݿ��Ϊ 16 λ
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;     //�ڴ����ݿ��Ϊ 16 λ
    DMA_InitStructure.DMA_Mode                 = DMA_Mode_Circular;     // ѭ��ģʽ
    DMA_InitStructure.DMA_Priority             = DMA_Priority_High;  // DMA ͨ�� ӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M                  = DMA_M2M_Disable;     // DMA ͨ�� x ��ֹ����Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); // ���üĴ���

    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);   // ʹ�� DMA ��������ж�
    DMA_Cmd(DMA1_Channel1, ENABLE); //���� DMA ͨ��
}

// DMA ��������
void adc_dma_reconfig(void)
{
   adc_dma_config();
}


// �����ѹֵ
// ��ʵ��ֵ��1000��, ������3λС��תΪ����
uint16_t adc_get_volt_value(uint16_t adc_val)
{
    return (uint16_t)((double)adc_val * 3300 / 4096);  // ��ʵ��ֵ��1000��, ������3λС��תΪ����
}

// �����ص�ѹ
// ����: uint16_t volt: ADC�����ĵ�ѹ, ��λ: mV
// ����ֵ: uint16_t ��ص�ѹ: ��λ: mV
//#define  adc_get_bat_volt(volt)   ((uint16_t)(((double)630.0 * (volt)) / 470.0))  //((uint16_t)(( 635.0 / 470.0) * (volt)))

uint16_t adc_get_bat_volt(uint16_t volt)
{
       uint16_t mV;

       mV = ((double)630) * volt / 470.0;
       return mV;
}
static uint8_t  first_core_temp_save = E_FALSE;
static uint16_t first_core_temp;



// ��ƽ��ֵ
void filter(void)
{
    uint16_t i, count;
    double sum = 0;
	
	for(i = 0; i < M; i++)   // һ�� M ��ͨ��
	{
	    for(count = 0; count < N; count++)  // ���� N ��
	    {
	        sum += AD_Value[count][i];
	    }
		aver_volt[i] = adc_get_volt_value(sum / N);
		sum = 0;
	}
}

uint16_t adc_get_temperature(uint16_t val)
{
    int16_t aver_temp;

	#if 0  // �˲��ֵ� val  �� AD ����ֵ
    double Vsense = 0.0; //  �¶ȴ�������ָ���¶�ʱ�ĵ�ѹֵ,unit: V
    
    // �¶ȼ��㹫ʽ: temp = (V25 - Vsense) / Avg_Slope + 25;
	// ����: V25 = 1.43V, Vsense = val * 3.3 / 4096, Avg_Slope = 4.3 mV = (4.3 / 1000) V
	Vsense = (val * 3.3 / 4096);
	if(1.43 > Vsense)  // �¶ȸ��� 25 'C
	{
	    aver_temp = (1.43 - Vsense) * 1000 / 4.3 + 25;
	}
	else  // �¶ȵ��� 25 'C
	{
	    aver_temp = 25 - (Vsense - 1.43) * 1000 / 4.3;
	}
	#else   // �����val  �� �����ĵ�ѹֵ, ��λ: mV
    if(1430 > val)  // �¶ȸ��� 25 'C
    {
           aver_temp = ((double)(1430 - val)) / 4.3 + 25;
    }
    else  // �¶ȵ��� 25 'C
   {
	    aver_temp = 25 - ((double)(val - 1430)) / 4.3;
    }
    #endif
	
    return aver_temp;
}

#include "os_timer.h"
static os_timer_t  tTimerADCGetValue;
static os_timer_t  tTimerADCMeasure;      // ����ADC �����Ķ�ʱ��
static os_timer_t  tTimerADCRealStart;  

extern void BatLev_VoltToPercent(uint16_t bat_volt);

// �ں��¶�
uint16_t ADCDrv_GetFirstCoreTemp(void)
{
      return first_core_temp;
}

uint16_t ADCDrv_GetCurCoreTemp(void)
{
      return adc_get_temperature(aver_volt[E_TEMP_SNS]); 
}

static void Timer_ADCGetValue_CallBack(void * arg)
{
       ADC_DEBUG("ADCGet Value cb t = %ld \r\n", os_get_tick());
       filter();

        if(! first_core_temp_save)
	 {
	        uint16_t temp;

	       temp  = adc_get_temperature(aver_volt[E_TEMP_SNS]);                                                  
	       if(temp < 100)
	       {
                     first_core_temp = temp;
                     first_core_temp_save = E_TRUE;
	       }
	 }
	 else
	 {
               ADC_DEBUG("first core temp = %d 'C \r\n",   first_core_temp);
	 }
	 
	ADCDrv_DrawBatCapacity(0); 
}

extern uint8_t BatLev_GetPercent(void);
extern uint8_t BatteryIsCharging(void);
void ADCDrv_DrawBatCapacity(uint8_t reflush)
{
      #if DEBUG_ADC_EN
       uint8_t i;
       #endif
      
       uint16_t v = 0;
      
       v = adc_get_bat_volt(aver_volt[E_BAT_ADC]);

       #if DEBUG_ADC_EN
	 ADC_DEBUG("\r\n");
	 for(i = 0; i < M; i++)
	 {
		 ADC_DEBUG("%s volt[%d] = %01d.%03d V\r\n", AdcChannelString[i], i, aver_volt[i] / 1000, aver_volt[i] % 1000);
	 }
	 ADC_DEBUG("temp = %d 'C\n", adc_get_temperature(aver_volt[E_TEMP_SNS]));
	 
	 ADC_DEBUG("battery volt = %01d.%03d V\n", v / 1000, v % 1000);
	 ADC_DEBUG("\r\n\r\n");
	 #endif

	  BatLev_VoltToPercent(v);
	  ICON_SetBatPercent(BatLev_GetPercent(), BatteryIsCharging(),  reflush);
	  ADC_DEBUG("bat percent = %3d%% \r\n",  BatLev_GetPercent());
}

// ��ʱ����������ADC ����
static void Timer_ADCRealStart_CallBack(void * arg)
{
    adc_dma_reconfig();
    adc_peripheral_enable();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  /* Start ADC1 Software Conversion */ 

    // �������, ������һ������ѭ��
    os_timer_arm(&tTimerADCMeasure, SEC(10), 0);
}

// ���� , ��� 10 s ����һ��
static void Timer_ADCMeasure_CallBack(void * arg)
{
	//BAT_CE_Set(SW_CLOSE);  // �ȹرյ�س��, �ټ���ص�ѹ, ���������ĵ�ص�ѹ��׼ȷ
    
	// 500 ms ������, ��΢�ȴ� 500ms ��Ϊ�� CE �ܽŵĵ�ѹ��ȫ���͵�ʵ�ʵĵ�ص�ѹ
      os_timer_arm(&tTimerADCRealStart, 1, 0);  
}



// ADC ������ʼ��
void ADCDrv_DMALaunch(void)
{
      STM32_RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
		                            | RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��

	// PCLK2 = 48MHz, ADCʱ����Ҫ��Ƶ
       // ADCCLK = PCLK2 / 6 = 72 MHz / 6 = 12M
	STM32_RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6, 72 M /6 = 12, ADC���ʱ�䲻�ܳ���14M
	STM32_RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ�� DMA ����

       adc_gpio_init();
	adc_dma_mode_init();
	
      // DMA ͨ�����ȼ�����
      STM32_NVICInit(DMA1_Channel1_IRQn, 4, 6, 0);	// ��4 �����ȼ�, 4λ��ռ���ȼ�, 0λ��Ӧ���ȼ�
    
	os_timer_setfn(&tTimerADCGetValue, Timer_ADCGetValue_CallBack, NULL);
	os_timer_setfn(&tTimerADCMeasure,   Timer_ADCMeasure_CallBack,  NULL);
	os_timer_setfn(&tTimerADCRealStart, Timer_ADCRealStart_CallBack, NULL);
	os_timer_arm(&tTimerADCMeasure, 0, 0);

	// ��һ�β���
	adc_dma_config();
	adc_peripheral_enable();
       ADC_SoftwareStartConvCmd(ADC1, ENABLE);  /* Start ADC1 Software Conversion */ 
}

// ��Ĭ״̬��ʼ��
void ADCDrv_SilentInit(void)
{
   STM32_RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE); //��ֹ DMA ����
   adc_peripheral_disable();

   os_timer_disarm(&tTimerADCGetValue);
   os_timer_disarm(&tTimerADCMeasure);
   os_timer_disarm(&tTimerADCRealStart);
}

// DMA �ж����
void DMA1_Channel1_IRQHandler(void)
{
   if(DMA_GetITStatus(DMA1_IT_TC1))  // DMA �����ж����
   {
	   DMA_ClearITPendingBit(DMA1_IT_GL1);
	   DMA_ClearFlag(DMA1_FLAG_TC1);
          adc_peripheral_disable();
	  // BAT_CE_Set(SW_OPEN);  // �� TP4056 ���г��

	   os_timer_arm(&tTimerADCGetValue, 0, 0);  // ������ʱ����ȡ����
   }
}

/************************************��ص�ѹ�������� end ***********************************************/


////////////////////////////////////////////////////////////////////////////////////////////
/************************************��ص�ѹ�������� begin  ***********************************************/

// �����ADC ģʽ��ʼ��, ��ص�ѹ����
void ADCDrv_BatLevel_NormalModeInit(void)
{
       ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	STM32_RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PC0 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	STM32_GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}

// ADC ��ص�ѹ��������
void ADCDrv_BatLevelMeasureStart(void)
{
    ADCDrv_BatLevel_NormalModeInit();
    STM32_ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );
}	


static os_timer_t tTimerBatLevelADC;

#define MAX_TIMES   40    // 40 ��ƽ��
static uint32_t total_sum = 0;
static uint16_t get_adc_count = 0;







// ������ز���
void ADCDrv_TrigBatMeasure(void)
{
      STM32_ADC_SoftwareStartConvCmd(ADC1, ENABLE);   // ������� AD ת��
      os_timer_arm(&tTimerBatLevelADC, 5, 0);  
}

static void Timer_BatLevelADC_CallBack(void * arg)
{
       uint32_t tick = 2;
	if(READ_REG_32_BIT(ADC1->SR, ADC_SR_EOC)) //ת������  
	{
	     #if DEBUG_ADC_EN
	      uint16_t cur_volt = 0;  //��ѹ
		{
		    
			
			cur_volt = (uint16_t)ADC1->DR;
			cur_volt = 3300 * cur_volt / 4096;
			cur_volt = adc_get_bat_volt(cur_volt);
			ADC_DEBUG("1th=%d.%03d V\n", cur_volt / 1000, cur_volt % 1000);
		}
		#endif
		
	       total_sum += (uint16_t)ADC1->DR;
		get_adc_count++;
		if(get_adc_count >= MAX_TIMES)
		{
		   uint16_t aver_volt, aver_adc, bat_volt;
		   
		   aver_adc = total_sum / MAX_TIMES;

                 aver_volt = adc_get_volt_value(aver_adc); // ƽ����ѹֵ
		   bat_volt  = adc_get_bat_volt(aver_volt);
		   
		   ADC_DEBUG("adc=%d, v=%d.%03d V, bat=%d.%03d V, %ld\n", aver_adc, aver_volt / 1000, aver_volt % 1000, 
		   	           bat_volt / 1000, bat_volt % 1000, os_get_tick());
		   
		   total_sum = 0;
		   get_adc_count = 0;
		   
		   if(arg)
		   {
		       ((void (*)(uint16_t))(arg))(bat_volt);  // ����ص�ѹת��Ϊ�ٷֱ�
		   }
		   return;
		}
	}
	else
	{
	       ADC_DEBUG("adc not end\n");
	       tick = 50;
	}
       STM32_ADC_SoftwareStartConvCmd(ADC1, ENABLE);   // ������� AD ת��
       os_timer_arm(&tTimerBatLevelADC, tick, 0);  
}



/*****************************
����: ���� ADC �������ʣ�����
             ����������ִ��(*end_exe_func)(uint16_t ) �ص�����
����: end_exe_func: ������������Ҫִ�еĲ���
********************************/
void ADCDrv_StartBatMeasure(void  (*end_exe_func)(uint16_t arg))
{
       ADCDrv_BatLevelMeasureStart();
	STM32_ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // ������� AD ת��
	
	os_timer_setfn(&tTimerBatLevelADC, Timer_BatLevelADC_CallBack, end_exe_func);
       os_timer_arm(&tTimerBatLevelADC, 5, 0);
}

extern void BatLev_VoltToPercent(uint16_t bat_volt);

static os_timer_t tTimerBatRoutine;
static void TimerBatRoutine_CallBack(void * arg)
{
      ADCDrv_TrigBatMeasure();
      os_timer_arm(&tTimerBatRoutine,    SEC(8),  0);
}

void ADCDrv_NormalLaunch(void)
{
      ADCDrv_StartBatMeasure(BatLev_VoltToPercent);
      os_timer_setfn(&tTimerBatRoutine,  TimerBatRoutine_CallBack,  NULL);
      os_timer_arm(&tTimerBatRoutine,    0,  0);
}

