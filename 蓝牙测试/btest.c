#include "STC15F2K60S2.H"        
#include "sys.H"                 
#include "Key.h"
#include "displayer.H" 
#include "music.H"
#include "beep.H"
#include "adc.H"
#include "ext.H"
#include "hall.H"
#include "ir.H"
#include "stepmotor.H"
#include "vib.H"
#include "FM_Radio.H"
#include "DS1302.H"
#include "M24C02.H"
#include "uart1.H"
#include "uart2.H"

code unsigned long SysClock=11059200;         
#ifdef _displayer_H_                          //显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等） 
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01, 0x41, 0x48, 
	              /* 序号:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15     */
                /* 显示:   0   1    2    3    4     5    6    7   8    9  (无)   下-  中-  上-  上中-  中下-   */  
	                        0x3f|0x80,0x06|0x80,0x5b|0x80,0x4f|0x80,0x66|0x80,0x6d|0x80,0x7d|0x80,0x07|0x80,0x7f|0x80,0x6f|0x80,   
             /* 带小数点     0         1         2         3         4         5         6         7         8         9        */
													0x76,0x38};
#endif

xdata unsigned char buffer[30],re[30],*match;
unsigned int send=9,recive=12;

void addData()
{
	Seg7Print(12,12,12,12,12,12,12,1);
	Uart1Print(buffer,send);
	Uart2Print(buffer,send);
}

void reData()
{
	LedPrint(1);
	Uart1Print(re,recive);
}

void main()
{
	
	Uart1Init(9600);
	Uart2Init(9600,Uart2UsedforEXT);
	DisplayerInit();
	
	LedPrint(0);
	SetDisplayerArea(0,7);
	Seg7Print(12,12,12,12,12,12,12,12);
	
	//SetUart1Rxd(buffer,send,match,0);
	//SetEventCallBack(enumEventUart1Rxd,addData);
	
	SetUart2Rxd(re,recive,match,0);
	SetEventCallBack(enumEventUart2Rxd,reData);
	
	MySTC_Init();
	while(1)
	{
		MySTC_OS();
	}
}