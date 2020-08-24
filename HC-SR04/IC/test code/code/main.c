/*
 * ���������ģ��(CX20106+4069�汾)
 * 
 * ��;�����������ģ����Գ���
 * 
 * ����					����				��ע
 * Huafeng Lin			20011/01/05			����
 * Huafeng Lin			20011/01/05			�޸�
 * 
 */

#include<reg51.h>
#include<INTRINS.H>

#define uchar unsigned char
#define uint  unsigned int

#define	nop			_nop_()

#define LCM_Data     P2    //���ݽӿ�
#define Busy         0x80   //���ڼ��LCM״̬���е�Busy��ʶ

uint	dis;
uchar	flag=0,high_time,low_time;

sbit 	in=P3^3;	//�ⲿ�ж�1����CX20106��7��
sbit 	csb=P1^0;	//40KHz���������

sbit    LCM_RW     = P0^1;  //��д��������ˣ�LCD1602�ĵ����
sbit    LCM_RS     = P0^2;  //�Ĵ���ѡ������ˣ�LCD1602�ĵ��Ľ�
sbit    LCM_E      = P0^0;  //ʹ���ź������,LCD1602�ĵ�6��

void    WriteDataLCM		(uchar WDLCM);		//LCDģ��д����
void    WriteCommandLCM	(uchar WCLCM,BuysC); 	//LCDģ��дָ��
uchar   ReadDataLCM		(void);		//LCDģ�������
uchar   ReadStatusLCM	(void);		//��LCDģ���æ��
void    DisplayOneChar   (uchar X,uchar Y,uchar ASCII);		//�ڵ�X+1�еĵ�Y+1λ����ʾһ���ַ�
void    DisplayListChar  (uchar X,uchar Y,uchar delayms,uchar code *DData);
void    DisplayCursorPos (uchar X, uchar Y);
void    LCMInit          (void);
void    DisplayIntData   (uchar X, uchar Y,int ZhengShu,uchar Digit,uchar XiaoShu);
void    DisplayCharData  (uchar X, uchar Y,uchar ZiFu);

void DelayUs(uint us);
void DelayMs(uint Ms);

void 	init(void);			//��ʼ��
void 	delay_nms(uint n);	//��ʱnms
void 	display(uint dat);	//��ʾ����
void 	tran(void);			//���䳬����
void 	delay100us();		//��ʱ100us

/**********************************
�������ƣ���ʼ������
�޸����ڣ�
��ڲ�������
����ֵ��  ��
**********************************/
void init(void)
{
   	TMOD=0x01;//��ʱ��0��ʽ1���ڼ�ʱ����ʱ��1���ڲ���38K����
	TH0=0;
	TL0=0;	        /* �趨T0�Ĺ���ģʽΪ2*/
	EA=1;
	IT1=1;//�½�����Ч,�󴫸���
}
/**********************************
�������ƣ���ʱ����
�޸����ڣ�
��ڲ�����n
����ֵ��  ��
**********************************/
void delay_nms(uint n)
{
 	uchar i; 
      while(n--)
      {
          for(i = 0; i < 123; i++);
       }				;
}
/************************************************
��ʱ100us����
***********************************************/
void delay100us()
{
	uchar j;
	for(j=50;j>0;j--);
}
/**********************************
�������ƣ����������亯��
�޸����ڣ�
��ڲ�������
����ֵ��  ��
**********************************/
void tran(void)
{
	uchar i;
	float temp;
	TH0=0;
	TL0=0;//�嶨ʱ0
	TR0=1;//����ʱ0
	for(i=8;i>0;i--)
	{
		csb=!csb;
		//nop;
		//nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
	}
	csb=1;
	delay_nms(1);//��ʱ1ms���Һ��ٿ��жϣ�����ֱ�ӻ����Ļز�
	EX1=1;
	delay_nms(50);
	if(flag==1)
	{
		temp=high_time*256+low_time;
		temp=(temp/1000)/2;
		temp*=344;
		temp=temp/10;
		//if(temp>10)
		dis=(unsigned int)temp;
		flag=0;
	}
	//else dis=0;
}
/**********************************
�������ƣ��жϺ���
�޸����ڣ�
��ڲ�������
����ֵ��  ��
**********************************/
void TT() interrupt 2
{
	uint tmp;
	TR0=0;//�ض�ʱ��0
	ET1=0;//���ⲿ�ж�
	//flag=1;//�ⲿ�жϱ�־λ
	tmp=TH0*256+TL0;
	if((tmp>0)&&(tmp<60000))	
	{
		high_time=TH0;
		low_time=TL0;
		flag=1;	
	}
	else 
	{
		high_time=0;
		low_time=0;	
	} 
}

/*===========================================================================
 ������
=============================================================================*/
void main(void)
{
	uchar i;
	uint tp=0;
	init();
	LCMInit();
	DisplayOneChar(1,10,'M');
	DisplayListChar(0,0,0,"LCSOFT (C) 2011");
	while(1)
	{
		tran();
		for(i=100;i>0;i--)
		{
			DisplayIntData(1,9,dis,4,2);
		}	 
	}
}	 
/*====================================================================  
�� �ܣ�    ��1602��ʾһ���������� 
˵ ����    ��ʾһ����������-9999->32625.  ����������ʾ����5λ��    
======================================================================*/
void DisplayIntData(uchar X, uchar Y,int ZhengShu,uchar Digit,uchar XiaoShu)
{
      uchar i=0,k=0, BCD[5]={0};
	  if(Digit>5)  Digit=5;
      if(ZhengShu<0)
	  {
         k=1;//����ʾ־λ
         ZhengShu=-ZhengShu;
	  }
      BCD[4]  =  ZhengShu / 10000;         //�����λ����
      ZhengShu = ZhengShu % 10000;
      BCD[3]  =  ZhengShu / 1000;          //���ǧλ����
      ZhengShu = ZhengShu % 1000;
      BCD[2]  =  ZhengShu / 100;           //�����λ����
      ZhengShu = ZhengShu % 100;
      BCD[1]  =  ZhengShu / 10;            //���ʮλ����
      BCD[0]  =  ZhengShu % 10;            //�����λ����
          
      for(i=0;i<Digit;i++)//�����ʾ����ֵ
      {    
	       if((i==XiaoShu)&&(0!=XiaoShu))
		   {  
		      DisplayOneChar(X,Y-i,'.');//���С���� 
			  Y= Y-1; 
		    } 
			 
	       DisplayOneChar(X,Y-i,BCD[i]+0x30); //��ʾһ���ַ�
	  }
      if(k==1)
	  DisplayOneChar(X,Y-1,'-');//�������
}
/*==================================================================== 
�� �ܣ���1602��ʾһ���ַ����� 
˵ ������ʾһ���ַ�����0~256.  ����������ʾ����3λ
======================================================================*/
void DisplayCharData(uchar X, uchar Y,uchar ZiFu)
{
      uchar i=0;
      uchar ValueBCD[3];
      ValueBCD[0]  = ZiFu / 100;           //�����λ����
              ZiFu = ZiFu % 100;
      ValueBCD[1]  = ZiFu / 10;            //���ʮλ����
      ValueBCD[2]  = ZiFu % 10;            //�����λ����
          
      for(i=0;i<3;i++)//�����ʾ����ֵ
      {         
	       DisplayOneChar(X,Y+i,ValueBCD[i]+0x30); //��ʾһ���ַ�
	  }
}
/*======================================================================
 LCM��ʼ��
======================================================================*/
void LCMInit(void) 
{
 LCM_Data = 0;
 WriteCommandLCM(0x38,0); //������ʾģʽ���ã������æ�ź�
 DelayMs(5);
 WriteCommandLCM(0x38,0);
 DelayMs(5);
 WriteCommandLCM(0x38,0);
 DelayMs(5);
 WriteCommandLCM(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 WriteCommandLCM(0x08,1); //�ر���ʾ
 WriteCommandLCM(0x01,1); //��ʾ����
 WriteCommandLCM(0x06,1); // ��ʾ����ƶ�����
 WriteCommandLCM(0x0C,1); // ��ʾ�����������
 
 DelayMs(100);
}
/*====================================================================
   ��ʾ����λ��
====================================================================*/
void DisplayCursorPos( unsigned char X, unsigned char Y)
{
 X &= 0x1;
 Y &= 0xF; //����Y���ܴ���15��X���ܴ���1
 if (X) Y |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
 Y |= 0x80; // ���ָ����
 WriteCommandLCM(Y, 1); //���ﲻ���æ�źţ����͵�ַ��
}
/*====================================================================  
  ��ָ��λ����ʾһ���ַ�:�� X ��,�� y��
  ע��:�ַ������ܳ���16���ַ�
======================================================================*/
void DisplayListChar(uchar X,uchar Y,uchar delayms, uchar code *DData)
{
 unsigned char ListLength;

 ListLength = 0;

 X &= 0x1;
 Y &= 0xF; //����X���ܴ���15��Y���ܴ���1
 while (DData[ListLength]!='\0') //�������ִ�β���˳�
  { 
     if (Y <= 0xF) //X����ӦС��0xF
     {
        DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
        ListLength++;
        Y++;
	    DelayMs(delayms);//��ʱ��ʾ�ַ���
     }
     else
	    break;//����ѭ����
  }
}
/*====================================================================
  �趨��ʱʱ��:x*1us
====================================================================*/
void DelayUs(uint us)
{
    while(us--);
} 
/*====================================================================
  �趨��ʱʱ��:x*1ms
====================================================================*/
void DelayMs(uint Ms)
{
  uint i,TempCyc;
  for(i=0;i<Ms;i++)
  {
    TempCyc = 250;
    while(TempCyc--);
  }
}				  
/*=====================================================================
 д���ݺ���: E =������ RS=1 RW=0
======================================================================*/
void WriteDataLCM(unsigned char WDLCM)
{
 ReadStatusLCM(); //���æ
 LCM_Data = WDLCM;
 LCM_RS = 1;
 LCM_RW = 0;
 LCM_E = 0; //�������ٶ�̫�߿���������С����ʱ
 LCM_E = 0; //��ʱ
 LCM_E = 1;
}
/*====================================================================
  дָ���: E=������ RS=0 RW=0
======================================================================*/
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysCΪ0ʱ����æ���
{
 if (BuysC) ReadStatusLCM(); //������Ҫ���æ
 LCM_Data = WCLCM;
 LCM_RS = 0;
 LCM_RW = 0;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
}
/*====================================================================
//������
======================================================================*/
unsigned char ReadDataLCM(void)
{
 LCM_RS = 1;
 LCM_RW = 1;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
 return(LCM_Data);
}
/*====================================================================
  ������д����֮ǰ������LCD������״̬:E=1 RS=0 RW=1;
  DB7: 0 LCD���������У�1 LCD������æ��
  ��״̬
======================================================================*/
unsigned char ReadStatusLCM(void)
{
 LCM_Data = 0xFF;
 LCM_RS = 0;
 LCM_RW = 1;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
 while (LCM_Data & Busy); //���æ�ź�  
 return(LCM_Data);
}
/*======================================================================
�� ��:     ��1602 ָ��λ����ʾһ���ַ�:��һ��λ��0~15,�ڶ���16~31
˵ ��:     �� X ��,�� y ��  ע��:�ַ������ܳ���16���ַ�
======================================================================*/
void DisplayOneChar( unsigned char X, unsigned char Y, unsigned char ASCII)
{
 X &= 0x1;
 Y &= 0xF; //����Y���ܴ���15��X���ܴ���1
 if (X) Y |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
 Y |= 0x80; // ���ָ����
 WriteCommandLCM(Y, 0); //���ﲻ���æ�źţ����͵�ַ��
 WriteDataLCM(ASCII);
}