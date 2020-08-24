/*
 * 超声波测距模块(CX20106+4069版本)
 * 
 * 用途：超声波测距模块测试程序
 * 
 * 作者					日期				备注
 * Huafeng Lin			20011/01/05			新增
 * Huafeng Lin			20011/01/05			修改
 * 
 */

#include<reg51.h>
#include<INTRINS.H>

#define uchar unsigned char
#define uint  unsigned int

#define	nop			_nop_()

#define LCM_Data     P2    //数据接口
#define Busy         0x80   //用于检测LCM状态字中的Busy标识

uint	dis;
uchar	flag=0,high_time,low_time;

sbit 	in=P3^3;	//外部中断1，接CX20106的7脚
sbit 	csb=P1^0;	//40KHz方波输出脚

sbit    LCM_RW     = P0^1;  //读写控制输入端，LCD1602的第五脚
sbit    LCM_RS     = P0^2;  //寄存器选择输入端，LCD1602的第四脚
sbit    LCM_E      = P0^0;  //使能信号输入端,LCD1602的第6脚

void    WriteDataLCM		(uchar WDLCM);		//LCD模块写数据
void    WriteCommandLCM	(uchar WCLCM,BuysC); 	//LCD模块写指令
uchar   ReadDataLCM		(void);		//LCD模块读数据
uchar   ReadStatusLCM	(void);		//读LCD模块的忙标
void    DisplayOneChar   (uchar X,uchar Y,uchar ASCII);		//在第X+1行的第Y+1位置显示一个字符
void    DisplayListChar  (uchar X,uchar Y,uchar delayms,uchar code *DData);
void    DisplayCursorPos (uchar X, uchar Y);
void    LCMInit          (void);
void    DisplayIntData   (uchar X, uchar Y,int ZhengShu,uchar Digit,uchar XiaoShu);
void    DisplayCharData  (uchar X, uchar Y,uchar ZiFu);

void DelayUs(uint us);
void DelayMs(uint Ms);

void 	init(void);			//初始化
void 	delay_nms(uint n);	//延时nms
void 	display(uint dat);	//显示函数
void 	tran(void);			//发射超声波
void 	delay100us();		//延时100us

/**********************************
函数名称：初始化函数
修改日期：
入口参数：无
返回值：  无
**********************************/
void init(void)
{
   	TMOD=0x01;//定时器0方式1用于计时，定时器1用于产生38K方波
	TH0=0;
	TL0=0;	        /* 设定T0的工作模式为2*/
	EA=1;
	IT1=1;//下降沿有效,左传感器
}
/**********************************
函数名称：延时函数
修改日期：
入口参数：n
返回值：  无
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
延时100us函数
***********************************************/
void delay100us()
{
	uchar j;
	for(j=50;j>0;j--);
}
/**********************************
函数名称：超声波发射函数
修改日期：
入口参数：无
返回值：  无
**********************************/
void tran(void)
{
	uchar i;
	float temp;
	TH0=0;
	TL0=0;//清定时0
	TR0=1;//开定时0
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
	delay_nms(1);//延时1ms左右后再开中断，避免直接回来的回波
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
函数名称：中断函数
修改日期：
入口参数：无
返回值：  无
**********************************/
void TT() interrupt 2
{
	uint tmp;
	TR0=0;//关定时器0
	ET1=0;//关外部中断
	//flag=1;//外部中断标志位
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
 主程序
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
功 能：    在1602显示一个整数数据 
说 明：    显示一个整数数据-9999->32625.  从右至左显示数据5位：    
======================================================================*/
void DisplayIntData(uchar X, uchar Y,int ZhengShu,uchar Digit,uchar XiaoShu)
{
      uchar i=0,k=0, BCD[5]={0};
	  if(Digit>5)  Digit=5;
      if(ZhengShu<0)
	  {
         k=1;//负数示志位
         ZhengShu=-ZhengShu;
	  }
      BCD[4]  =  ZhengShu / 10000;         //求出万位数据
      ZhengShu = ZhengShu % 10000;
      BCD[3]  =  ZhengShu / 1000;          //求出千位数据
      ZhengShu = ZhengShu % 1000;
      BCD[2]  =  ZhengShu / 100;           //求出百位数据
      ZhengShu = ZhengShu % 100;
      BCD[1]  =  ZhengShu / 10;            //求出十位数据
      BCD[0]  =  ZhengShu % 10;            //求出个位数据
          
      for(i=0;i<Digit;i++)//输出显示的数值
      {    
	       if((i==XiaoShu)&&(0!=XiaoShu))
		   {  
		      DisplayOneChar(X,Y-i,'.');//输出小数点 
			  Y= Y-1; 
		    } 
			 
	       DisplayOneChar(X,Y-i,BCD[i]+0x30); //显示一个字符
	  }
      if(k==1)
	  DisplayOneChar(X,Y-1,'-');//输出负符
}
/*==================================================================== 
功 能：在1602显示一个字符数据 
说 明：显示一个字符数据0~256.  从左至右显示数据3位
======================================================================*/
void DisplayCharData(uchar X, uchar Y,uchar ZiFu)
{
      uchar i=0;
      uchar ValueBCD[3];
      ValueBCD[0]  = ZiFu / 100;           //求出百位数据
              ZiFu = ZiFu % 100;
      ValueBCD[1]  = ZiFu / 10;            //求出十位数据
      ValueBCD[2]  = ZiFu % 10;            //求出个位数据
          
      for(i=0;i<3;i++)//输出显示的数值
      {         
	       DisplayOneChar(X,Y+i,ValueBCD[i]+0x30); //显示一个字符
	  }
}
/*======================================================================
 LCM初始化
======================================================================*/
void LCMInit(void) 
{
 LCM_Data = 0;
 WriteCommandLCM(0x38,0); //三次显示模式设置，不检测忙信号
 DelayMs(5);
 WriteCommandLCM(0x38,0);
 DelayMs(5);
 WriteCommandLCM(0x38,0);
 DelayMs(5);
 WriteCommandLCM(0x38,1); //显示模式设置,开始要求每次检测忙信号
 WriteCommandLCM(0x08,1); //关闭显示
 WriteCommandLCM(0x01,1); //显示清屏
 WriteCommandLCM(0x06,1); // 显示光标移动设置
 WriteCommandLCM(0x0C,1); // 显示开及光标设置
 
 DelayMs(100);
}
/*====================================================================
   显示光标的位置
====================================================================*/
void DisplayCursorPos( unsigned char X, unsigned char Y)
{
 X &= 0x1;
 Y &= 0xF; //限制Y不能大于15，X不能大于1
 if (X) Y |= 0x40; //当要显示第二行时地址码+0x40;
 Y |= 0x80; // 算出指令码
 WriteCommandLCM(Y, 1); //这里不检测忙信号，发送地址码
}
/*====================================================================  
  按指定位置显示一串字符:第 X 行,第 y列
  注意:字符串不能长于16个字符
======================================================================*/
void DisplayListChar(uchar X,uchar Y,uchar delayms, uchar code *DData)
{
 unsigned char ListLength;

 ListLength = 0;

 X &= 0x1;
 Y &= 0xF; //限制X不能大于15，Y不能大于1
 while (DData[ListLength]!='\0') //若到达字串尾则退出
  { 
     if (Y <= 0xF) //X坐标应小于0xF
     {
        DisplayOneChar(X, Y, DData[ListLength]); //显示单个字符
        ListLength++;
        Y++;
	    DelayMs(delayms);//延时显示字符串
     }
     else
	    break;//跳出循环体
  }
}
/*====================================================================
  设定延时时间:x*1us
====================================================================*/
void DelayUs(uint us)
{
    while(us--);
} 
/*====================================================================
  设定延时时间:x*1ms
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
 写数据函数: E =高脉冲 RS=1 RW=0
======================================================================*/
void WriteDataLCM(unsigned char WDLCM)
{
 ReadStatusLCM(); //检测忙
 LCM_Data = WDLCM;
 LCM_RS = 1;
 LCM_RW = 0;
 LCM_E = 0; //若晶振速度太高可以在这后加小的延时
 LCM_E = 0; //延时
 LCM_E = 1;
}
/*====================================================================
  写指令函数: E=高脉冲 RS=0 RW=0
======================================================================*/
void WriteCommandLCM(unsigned char WCLCM,BuysC) //BuysC为0时忽略忙检测
{
 if (BuysC) ReadStatusLCM(); //根据需要检测忙
 LCM_Data = WCLCM;
 LCM_RS = 0;
 LCM_RW = 0;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
}
/*====================================================================
//读数据
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
  正常读写操作之前必须检测LCD控制器状态:E=1 RS=0 RW=1;
  DB7: 0 LCD控制器空闲，1 LCD控制器忙。
  读状态
======================================================================*/
unsigned char ReadStatusLCM(void)
{
 LCM_Data = 0xFF;
 LCM_RS = 0;
 LCM_RW = 1;
 LCM_E = 0;
 LCM_E = 0;
 LCM_E = 1;
 while (LCM_Data & Busy); //检测忙信号  
 return(LCM_Data);
}
/*======================================================================
功 能:     在1602 指定位置显示一个字符:第一行位置0~15,第二行16~31
说 明:     第 X 行,第 y 列  注意:字符串不能长于16个字符
======================================================================*/
void DisplayOneChar( unsigned char X, unsigned char Y, unsigned char ASCII)
{
 X &= 0x1;
 Y &= 0xF; //限制Y不能大于15，X不能大于1
 if (X) Y |= 0x40; //当要显示第二行时地址码+0x40;
 Y |= 0x80; // 算出指令码
 WriteCommandLCM(Y, 0); //这里不检测忙信号，发送地址码
 WriteDataLCM(ASCII);
}