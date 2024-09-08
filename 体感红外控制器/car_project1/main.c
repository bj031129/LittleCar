#include "STC15F2K60S2.H"
#include "sys.H"
#include "displayer.H"
#include "beep.H" 
#include "key.h"
#include "uart2.h"
#include "math.h"
#include "beep.h"
#include "ir.h"
#include "adc.h"
#include "DS1302.h"
#include "ADXL345_Func.h"
code  unsigned long SysClock=11059200;

#ifdef _displayer_H_
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
	              /* ���:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15     */
                /* ��ʾ:   0   1    2    3    4     5    6    7   8    9     A      B  		C 		D			E			F	*/  
	                       0x3f|0x80,0x06|0x80,0x5b|0x80,0x4f|0x80,0x66|0x80,0x6d|0x80,0x7d|0x80,0x07|0x80,0x7f|0x80,0x6f|0x80,
             /* ��С����     0         1         2         3         4         5         6         7         8         9        */
														0x00,0x08,0x40,0x01, 0x41, 0x48,0x3D,
													
							/*            (��)   ��-  ��-  ��-  ����-  ����-    		G				*/
								/*        26    27  	 28 		29		30		31		32				*/
													0x76, 0x10, 0x0E, 0x7A, 0x38, 0x55, 0x54, 
							/*          H      I  		J 		K			L			M			N				*/
								/*        33    34  	 35 		36		37		38		39				*/	
													0x5C, 0x73, 0x67,       0x50, 0x65, 0x78, 
								/*          O      P  	Q						R			S			T						*/
								/*        40    41  	 42 				43		44		45				*/	
													0x3E, 0x1C, 0x6A, 0x64, 0x6E, 0x5A,	
								/*          U      V  	W	  	X			Y			Z					*/
								/*        46    47  	 48		 49			50     51				*/	

							/*Сдabcd����52��ʼ*/						
													0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x3d,
													0x76, 0x0f, 0x0e, 0x75, 0x38, 0x37, 0x54,
													0x5c, 0x73, 0x67,         0x31, 0x49, 0x78,
													0x3e, 0x1c, 0x7e, 0x64, 0x6e, 0x59};
#endif

char KeyPressing=0;
char initing=0; //����ǰ���ʼ��
unsigned char carSpeed=0x11;
unsigned char sendBuffer[1];
unsigned char ir_command = 0x00;
													
#define THRESHOLD_ANGLE 30       // �Ƕȱ仯��ֵ�������жϷ�ת����
#define THRESHOLD_ACCELERATION 50U //���ٶȱ仯��ֵ�������ж�ƽ������  

													
//����ָ��
void send2car(unsigned char carCommand){
			sendBuffer[0]=carCommand;
			IrPrint(sendBuffer,1);
		//	Uart2Print(sendBuffer,1);
}			

//�ٶȵ���
void speed_callback(){

	//�����ٶ�
	if(GetAdcNavAct(enumAdcNavKeyUp) == enumKeyPress){
		carSpeed++;
		SetBeep(800,18);
	}
	if(GetAdcNavAct(enumAdcNavKeyDown) == enumKeyPress){
		carSpeed--;
		SetBeep(800,18);
	}  
	if(GetAdcNavAct(enumAdcNavKeyCenter) == enumKeyPress){
		carSpeed=0x11;
		SetBeep(800,18);
	} 
	if(carSpeed < 0x11 || carSpeed  > 0x1a){
			if(carSpeed < 0x11)carSpeed=0x11;
			if(carSpeed > 0x1a)carSpeed=0x1a;
	} 
	send2car(carSpeed);
}

//����ָ��ʶ��													
void Gesture_Recognition(){

	//��ֹ̬��ʾcar���ٶ�
		Seg7Print(12,10,43,26,47,carSpeed&0x0f,26,28);	

	
		//��ת
	if(accZ_f > THRESHOLD_ACCELERATION){
		
		//ǰ��ת������K1��  //ǰ
	if(angleY_f > THRESHOLD_ANGLE  && angleX_f < THRESHOLD_ANGLE  && KeyPressing==0){
		Seg7Print(12,10,43,26,47,carSpeed&0x0f,26,15);
		ir_command=0x01;
		send2car(ir_command);
  }	
	//ǰ��ת����K1��	//��
	else if(angleY_f > THRESHOLD_ANGLE  && angleX_f < THRESHOLD_ANGLE && KeyPressing==1){
		Seg7Print(12,10,43,26,47,carSpeed&0x0f,26,11);
		ir_command=0x04;
		send2car(ir_command);
  }		

	//���ҷ�ת������K1��//�� 
	else if(angleX_f > THRESHOLD_ANGLE && angleY_f < THRESHOLD_ANGLE && KeyPressing==0 ){
		Seg7Print(12,10,43,26,47,carSpeed&0x0f,26,37);
		ir_command=0x02;
		send2car(ir_command);
  }
	//���ҷ�ת����K1�� //��
	else if(angleX_f > THRESHOLD_ANGLE   && angleY_f < THRESHOLD_ANGLE && KeyPressing==1){
		Seg7Print(12,10,43,26,47,carSpeed&0x0f,26,43);
		ir_command=0x03;
		send2car(ir_command);
  }	

}

}

unsigned char ir_counter=0; 
//��ʼ����ɺ�ÿ50msʶ��һ������
void my10mS_callback(){
	
	if(initing<4)return;
	if(ir_counter==5)ir_counter=0;
	
	Multiple_Read_ADXL345();//��ȡ����
	ACC_XYZ(2);//ʵʱ��ȡ�ǶȺͼ��ٶȣ�ƽ��ֵ��  

	if(ir_counter==1){
	Gesture_Recognition();//ʶ����
	}

	ir_counter++;
}


void myKey_callback(){

	char k;
	
	//ʹ��K2��ʼ������3��-X��Y��Z��
	k=GetKeyAct(enumKey2);
		if(initing==3 && k == enumKeyPress ){
				Seg7Print(26,26,26,26,0,26,26,26);
				offX=(x0g[0]+x0g[1])/2;
				offY=(y0g[0]+y0g[1])/2;
				offZ=(z0g[0]+z0g[1])/2;
				factorX=(double)1000/abs(x1g-offX);
				factorY=(double)1000/abs(y1g-offY);
				factorZ=(double)1000/abs(z1g-offZ);
			  initing++;
}
		if(initing==2 && k == enumKeyPress ){	 
			Seg7Print(26,26,26,1,26,26,26,26);
			Multiple_Read_ADXL345();
			x0g[0]=(Buffer[1]<<8)+Buffer[0];
			y0g[0]=(Buffer[3]<<8)+Buffer[2];
			z1g=(Buffer[5]<<8)+Buffer[4];
			initing++;
}

	if(initing==1 && k == enumKeyPress ){ 
			Seg7Print(26,26,26,2,26,26,26,26);
			Multiple_Read_ADXL345();
			x0g[1]=(Buffer[1]<<8)+Buffer[0];
			z0g[1]=(Buffer[5]<<8)+Buffer[4];
			y1g=(Buffer[3]<<8)+Buffer[2];
			initing++;
}
	if(initing==0 && k == enumKeyPress ){
		Seg7Print(26,26,26,3,26,26,26,26);
		Multiple_Read_ADXL345();//������Ĵ���ֵ��ȡ��Buffer������
			z0g[0]=(Buffer[5]<<8)+Buffer[4];
			y0g[1]=(Buffer[3]<<8)+Buffer[2];
			x1g=(Buffer[1]<<8)+Buffer[0];
			initing++;
}

	//K1����״̬
	k=GetKeyAct(enumKey1);
	if( k == enumKeyPress ) KeyPressing=1;
	  else if( k == enumKeyRelease ) KeyPressing=0;

}


void main()
{
  DisplayerInit();  
	KeyInit();
	AdcInit(ADCexpEXT);
	Init_ADXL345();
	BeepInit();
	LedPrint(0);
	IrInit(NEC_R05d);
	SetDisplayerArea(0,7);
	Uart2Init(9600,Uart2Usedfor485);

	
  Delay(500);	                   //�ϵ���ʱ		
	P0M1=0x00;
  P0M0=0xff;
						
	
		//����ADXL345оƬ�Ƿ�ɷ���						
  if(Single_Read_ADXL345(0x00)==0xE5)	     			//0�������ܷ���	  // 1�����ɷ��ʣ�����ģʽ��
	 {												//                             2�����ɷ��ʣ�����ģʽ����������£�
	      Seg7Print(10, 10, 10, 10, 10, 10, 10, 1);                                   	//����������Ϊ0XE5,��ʾʱ����ȷ, ��ʾ��1�� 
        if(Single_Read_ADXL345(0x2D)==0x08) Seg7Print(26,26,26,26,26,26,26,2);    //����������Ϊ0x08 ��ʾ��345�ϵ��ˣ���ʾ��2��
   }
	else   Seg7Print(10, 10, 10, 10, 10, 10, 10, 0);							    //��ʾ��0�� 

	 
	Seg7Print(26,11, 14, 15, 40, 43, 14, 26);

	Multiple_Read_ADXL345();//������Ĵ���ֵ��ȡ��Buffer������
	offX=(Buffer[1]<<8)+Buffer[0];
	offY=(Buffer[3]<<8)+Buffer[2];
	offZ=(Buffer[5]<<8)+Buffer[4];				 

	SetEventCallBack(enumEventSys10mS, my10mS_callback);
	SetEventCallBack(enumEventKey, myKey_callback);
	SetEventCallBack(enumEventNav,speed_callback);
	 MySTC_Init();	          
    while(1)                 
        { 

					MySTC_OS();    
        }      
}


