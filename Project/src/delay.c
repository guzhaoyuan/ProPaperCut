#include "delay.h"

u32 fac_ms;//ȫ�ֱ���
u8 fac_us;//ȫ�ֱ���

/****************************************************
�������ܣ�ms����ʱ
���������nms : ����
�����������
��    ע�����ô˺���ǰ����Ҫ��ʼ��Delay_Init()����
*****************************************************/							    
void delay_ms(u16 nms)
{
   	  SysTick->LOAD = (u32)fac_ms*nms-1;//����ʱ��ֵ
	  SysTick->VAL = 1;//���д��ֵ��������ؼĴ�����ֵ
	  SysTick->CTRL |= BIT(0);//SysTickʹ��
	  while(!(SysTick->CTRL&(1<<16)));//�ж��Ƿ����0
	  SysTick->CTRL &=~BIT(0);//�ر�SysTick
}

/****************************************************
�������ܣ���ʱ��ʼ��
���������SYSCLK : ϵͳʱ��(168)MHZ
�����������
��    ע����
*****************************************************/
void Delay_Init(u8 SYSCLK)
{
     RCC_HCLKConfig(RCC_HCLK_Div1); 
     SysTick->CTRL |=BIT(2);//ѡ���ڲ�ʱ��(��AHB)����Ϊ�ⲿʱ����Ϊ�˷�Ƶ��AHB
	 SysTick->CTRL &=~BIT(1);//�رն�ʱ������0����ж�����
	 SysTick->CTRL &=~BIT(0);//�ر�systick
	 fac_us = SYSCLK;//�����SysTick����ֵ
	 fac_ms = (u32)fac_us*1000;	 
}

/****************************************************
�������ܣ�us����ʱ
���������nus : ΢��
�����������
��    ע�����ô˺���ǰ����Ҫ��ʼ��Delay_Init()����
*****************************************************/		    								   
void delay_us(u32 nus)
{		  
	  SysTick->LOAD = (u32)fac_us*nus-1;//����ʱ��ֵ
	  SysTick->VAL = 1;//���д��ֵ��������ؼĴ�����ֵ
	  SysTick->CTRL |= BIT(0);//SysTickʹ��
	  while(!(SysTick->CTRL&(1<<16)));//�ж��Ƿ����0
	  SysTick->CTRL &=~BIT(0);//�ر�SysTick
}


