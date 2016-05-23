#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12xs128.h>
#include "KEY.H"
#include "PLL.H"
#include "PWM.H"
#include "ATD.H"
#include "LCD5110.H"
#include "PIT.H"
#include "MATH.H"
#include "ECT.H"
#define  AD_NUM  6
#define  xiaosjudge  0

/***************************************************************/
long ad_value[AD_NUM]={0,0,0,0,0,0};
/**********************卡尔曼滤波*****************************************/

float gyro=0,acceler=0,Vref=2.048; //2.048
float angle=0,angle_dot=0;		
unsigned int fangdabeishu =5; 
float C_0 = 1;
float Q_angle=0.001; 
float Q_gyro=0.003; 
float R_angle=0.5;
float dt=0.006;

float P[2][2] = {
                 {1,0},
				         {0,1}
			        	};
float Pdot[4] ={0,0,0,0};
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;  
/*********************中断变量**************************************/
unsigned char EventCount=4,SpeedControlPeriod=99,DirectionControlPeriod=0;
unsigned char j=0,S=0,xiaosflag=0,xiaosflag1=0,xiaosflag2=0,xiaoscount=0,xiaoscount1=0,xiaoscount2=0;
unsigned char InputVoltageCount=0,SpeedControlCount=0,DirectionControlCount=0,zhili_count=0,Count=0,count_c=0,Run_Flag=0;
/************************直立PID************************************/
unsigned int JIAODU_CENTER=755,JIAO_SPEED_CENTER=700;//初始值780  700
unsigned int ZHILI_PID_P=580,ZHILI_PID_D=8;
int ZHILI;
/*************************速度PID*************************************/
unsigned int Sudu_PID_P=80,Sudu_PID_D=0,a=0;
long Speed_P=0,Speed_D=0,Speed_error=0,Speed_error_old=0,Speed_want=55,Speed[3]={55,70,80};// Speed【6】初始值 
long SpeedLeft_now=0,SpeedRight_now=0;
long CarSpeed=0,SpeedControlOutNew=0,SpeedControlOutOld=0;
int  SpeedControlOut=0;
long LeftMotorPulseSigma=0,RightMotorPulseSigma=0;
float fValue=0;
/*************************方向PID******************************/
unsigned int DIR_CONTROL_P=20,DIR_CONTROL_D=400,DIRD[2]={100,420};
long InputVoltageSigma[6]=0;
int LeftMotorOut=0,RightMotorOut=0;
int DirectionControlOutNew=0, DirectionControlOutOld=0;
float Last_fLeftRight=0,fLeftRight=0,DDValue=0,fLeftRightAdd=0,fLeftRightSub=0;
int DirectionControlOut=0,DValue=0;
/////////////////////////////////////////////////////
int jiansu_flag;
//控速标志位
/////////////////////////////////////////////////////





#include "LCDKEY.H"  //液晶控制程序
/***********************读AD******************************************/
void Read_Atd_Several() 
{
    ATD0CTL5=(atd0_scan<<5) + (atd0_mult<<4) + atd0_cd_cc_cb_ca;
    while(!ATD0STAT0_SCF);
     
     ad_value[0]=ATD0DR0;
     ad_value[1]=ATD0DR1;
    // ad_value[2]=ATD0DR2;
     
     //ad_value[3]=ATD0DR3; //两电感方案。。无用项
     //ad_value[4]=ATD0DR4;
     
   //  ad_value[5]=ATD0DR5;
}
/*************************卡尔曼滤波*********************************************/
void Kalman_Filter(float angle_m,float gyro_m)	//gyro_m:gyro_measure
{                                      
	angle+=(gyro_m-q_bias) * dt;//先验估计
	angle_err = angle_m - angle;//zk-先验估计
	
	Pdot[0]=Q_angle - P[0][1] - P[1][0];// Pk-' 先验估计误差协方差的微分
	Pdot[1]=- P[1][1];
	Pdot[2]=- P[1][1];
	Pdot[3]=Q_gyro;
	
	P[0][0] += Pdot[0] * dt;// Pk- 先验估计误差协方差微分的积分 = 先验估计误差协方差
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;
		
	PCt_0 = C_0 * P[0][0];
	PCt_1 = C_0 * P[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * P[0][1];

	P[0][0] -= K_0 * t_0;//后验估计误差协方差
	P[0][1] -= K_0 * t_1;
	P[1][0] -= K_1 * t_0;
	P[1][1] -= K_1 * t_1;
	
	
	angle	+= K_0 * angle_err;//后验估计
	q_bias	+= K_1 * angle_err;//后验估计
  angle_dot = gyro_m-q_bias; 
}  
/***************************AD计算********************************************/
void AD_calculate(void) 
{
  acceler=(float)ad_value[0];		
  gyro=(float)ad_value[1];
  acceler=(JIAODU_CENTER-acceler)*Vref/(1024*0.8)*90;
  gyro=(JIAO_SPEED_CENTER-gyro)*Vref/(1024*0.00067*fangdabeishu);
  Kalman_Filter(acceler,gyro); //卡尔曼滤波
}
/*************************直立PID控制*********************************************/                        
void zhili_kongzhi(void) 
{
ZHILI=(int)((angle)*ZHILI_PID_P+(angle_dot)*ZHILI_PID_D);

}
/*****************************速度PID控制*****************************************************/

void GetLeftMotorPulse(void) 
{
   SpeedLeft_now=PACNT;
   PACNT=0;
   if(LeftMotorOut<0)  SpeedLeft_now=-SpeedLeft_now;
   LeftMotorPulseSigma+=SpeedLeft_now;
}
void GetRightMotorPulse(void) 
{
   SpeedRight_now=PACNT;
   PACNT=0;                       
   if(RightMotorOut<0)  SpeedRight_now=-SpeedRight_now;
   RightMotorPulseSigma+=SpeedRight_now;
}

void SpeedControl(void) 
{ 
  CarSpeed=(RightMotorPulseSigma+LeftMotorPulseSigma)/10;
  RightMotorPulseSigma=0;
  LeftMotorPulseSigma=0;
  Speed_error=Speed_want-CarSpeed;
  Speed_P=(Speed_error)*Sudu_PID_P;
  Speed_D=(Speed_error-Speed_error_old)*Sudu_PID_D;
  Speed_error_old=Speed_error;
  SpeedControlOutOld=SpeedControlOutNew;
  SpeedControlOutNew=Speed_P+Speed_D;
  
}
void SpeedControlOutput(void)
{
  fValue = (float)(SpeedControlOutNew-SpeedControlOutOld);
  SpeedControlOut =(int)(fValue * (SpeedControlPeriod+1) / 100 + SpeedControlOutOld);
}
/**************************方向PID控制***********************************************************/
void SampleInputVoltage(void) 
{
    Read_Atd_Several();
    InputVoltageSigma[0] += ad_value[0];
  	InputVoltageSigma[1] += ad_value[1];
	  InputVoltageSigma[2] += ad_value[2];
   // InputVoltageSigma[3] += ad_value[3];
   //	InputVoltageSigma[4] += ad_value[4];
  	InputVoltageSigma[5] += ad_value[5];

  	InputVoltageCount++;
}
void GetInputVoltageAverage(void) 
{
	int i;
	for(i = 0; i < 6; i ++) 
	  {
		ad_value[i] =(InputVoltageSigma[i]/InputVoltageCount);
		InputVoltageSigma[i] = 0;
    }
	
  InputVoltageCount = 0;
}

void xiaostext(void)   
{
  if((ad_value[5]-ad_value[2])>120)  xiaosflag=1;
  else if((ad_value[5]-ad_value[2])<-120)  xiaosflag=0; 
  
  xiaosflag2=xiaosflag1;
  xiaosflag1=xiaosflag;
  if(xiaosflag!=xiaosflag1)
  {
  xiaoscount=0;
  } 
  else
  {
   xiaoscount++;
  }
  
  if(xiaoscount>200) 
  {
   xiaoscount=50;
  }
  xiaoscount2=xiaoscount1;
  xiaoscount1=xiaoscount;
 /* if(xiaoscount==0) 
  {
     if(xiaoscount2 < 10)
            PTT_PTT5 = 0;      
        else
            PTT_PTT5 = 1;
  }
  else
    {
        if(PTT_PTT5 == 0) 
        {          
          if(xiaoscount2 > 10)
            PTT_PTT5 = 1;  
        }
    }
  if(PTT_PTT5==0)
  {
   DIR_CONTROL_P=6;
  } 
  else
  {
  DIR_CONTROL_P=20;
  }  */
    
}
void DirectionControl(void) 
{
	fLeftRightSub=(float)(ad_value[5]-ad_value[2]);
	fLeftRightAdd=(float)(ad_value[5]+ad_value[2]);
	fLeftRight=fLeftRightSub/fLeftRightAdd;
	DDValue=fLeftRight-Last_fLeftRight; 
  DValue=(int)(fLeftRight*DIR_CONTROL_P+DDValue*DIR_CONTROL_D);
  Last_fLeftRight=fLeftRight;
	DirectionControlOut=DValue;
}
/********************************电机输出**************************************************************/
 void PWM_out(void)  
{ 
 if(LeftMotorOut>0)
 { 
 PWMDTY7=LeftMotorOut+3;
 PWMDTY5=0;//左电机 
 }
 else 
 {
 PWMDTY5=-LeftMotorOut+3;
 PWMDTY7=0;//左电机 
 }
 if(RightMotorOut>0) 
 {
   PWMDTY1=RightMotorOut+3; //右电机
   PWMDTY3=0;
 } 
 else 
 {                                                  
  PWMDTY1=0;
  PWMDTY3=-RightMotorOut+3;
 }
}
void MotorOutput(void) 
{
 LeftMotorOut=(int)(ZHILI/10-SpeedControlOut/10-DirectionControlOut*10);
 RightMotorOut=(int)(ZHILI/10-SpeedControlOut/10+DirectionControlOut*10);
 
 if(LeftMotorOut>245)	         LeftMotorOut=245;
 else if(LeftMotorOut<-245)	   LeftMotorOut=-245;
 if(RightMotorOut>245) 	       RightMotorOut=245;
 else if(RightMotorOut<-245) 	 RightMotorOut=-245;
 
 if(angle>30||angle<-30||(Run_Flag==0)||(fLeftRightAdd<100))
 {
 LeftMotorOut=0;
 RightMotorOut=0; 
 }
 PWM_out(); 
}  
/**********************************中断函数************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 66 Pit0_interrupt(void) 
{               
        DisableInterrupts;
 if(ad_value[5]>ad_value[2])
       { if(ad_value[5]-ad_value[2]>260)
        jiansu_flag=1;
        else
        jiansu_flag=0;
       }
        else
        {
        if(ad_value[5]<ad_value[2])
        if(ad_value[2]-ad_value[5]>260)
        jiansu_flag=1;
        else
        jiansu_flag=0;
        else
        jiansu_flag=0;
       } 
        if(jiansu_flag==1)
        JIAO_SPEED_CENTER+=1;
        else
        JIAODU_CENTER=780;     
        
       EventCount++;
        SpeedControlPeriod++;
        if(SpeedControlPeriod>=100) 
        {
        SpeedControlPeriod=0;
        }
        SpeedControlOutput(); 
        MotorOutput();
      if(EventCount>=5) 
      {
        EventCount = 0; 
      	for(j = 0;j < 20;j++)
	    	SampleInputVoltage();
     	} 
      else if(EventCount == 1)
     	{
     	  GetInputVoltageAverage();
      	AD_calculate();
      	zhili_kongzhi(); 
     	}
     
      else if(EventCount == 2) 
  	  { 
  	  	Count++;                             
  	    if(Count==1) 
  	    {              
  	  	GetLeftMotorPulse();
  	  	PWMDTY4=250;
  	    } 
  	    else if(Count==2) 
  	    {            
  	      Count=0;
  	      GetRightMotorPulse();
  	    	PWMDTY4=0;
      	}
       }
      else if(EventCount == 3) 
      {                            // Car speed adjust
  		  SpeedControlCount ++;
  		if(SpeedControlCount >= 20) 
  	    { 	
  		    SpeedControlCount = 0;
  		  	SpeedControl(); 			
  		  }
    	} 
    	else if(EventCount == 4)
     	{
     	#if xiaosjudge
     	S++;
     	if(S>=10)
     	S=0;
     	if(S==0)
     	{
     	xiaostext();
     	}
     #endif   	
      DirectionControl();	
      }
  	 	
        PITTF_PTF0=1; 
        EnableInterrupts;   
     
}
#pragma CODE_SEG DEFAULT    
////////////////////////////////////////////////自我添加模块
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 67 Pit2_interrupt(void)
/////////////////////////////////////
{        
         DisableInterrupts;
 /*        if(ad_value[5]>ad_value[2])
       { if(ad_value[5]-ad_value[2]>270)
        jiansu_flag=1;
        else
        jiansu_flag=0;
       }
        else
        {
        if(ad_value[5]<ad_value[2])
        if(ad_value[2]-ad_value[5]>270)
        jiansu_flag=1;
        else
        jiansu_flag=0;
        else
        jiansu_flag=0;
       } 
        if(jiansu_flag==1)
        JIAO_SPEED_CENTER+=1;
        else
        JIAODU_CENTER=780;*/
         EventCount++;
        SpeedControlPeriod++;
        if(SpeedControlPeriod>=100) 
        {
        SpeedControlPeriod=0;
        }
        SpeedControlOutput(); 
        MotorOutput();
      if(EventCount>=5) 
      {
        EventCount = 0; 
      	for(j = 0;j < 20;j++)
	    	SampleInputVoltage();
     	} 
      else if(EventCount == 1)
     	{
     	  GetInputVoltageAverage();
      	AD_calculate();
      	zhili_kongzhi(); 
     	}
     
      else if(EventCount == 2) 
  	  { 
  	  	Count++;                             
  	    if(Count==1) 
  	    {              
  	  	GetLeftMotorPulse();
  	  	PWMDTY4=250;
  	    } 
  	    else if(Count==2) 
  	    {            
  	      Count=0;
  	      GetRightMotorPulse();
  	    	PWMDTY4=0;
      	}
       }
      else if(EventCount == 3) 
      {                            // Car speed adjust
  		  SpeedControlCount ++;
  		if(SpeedControlCount >= 20) 
  	    { 	
  		    SpeedControlCount = 0;
  		  	SpeedControl(); 			
  		  }
    	} 
    	else if(EventCount == 4)
     	{
     	#if xiaosjudge
     	S++;
     	if(S>=10)
     	S=0;
     	if(S==0)
     	{
     	xiaostext();
     	}
     #endif   	
      DirectionControl();	
      } 
        
        PITTF_PTF1=1; 
        EnableInterrupts;  
}   //自我添加控速模块
   //////////////////////////////////// 
#pragma CODE_SEG DEFAULT  
////////////////////////////////////////////////////////////





/*********************************初始化****************************************************/
void Init_ALL()                
{
        
        Pll_Init();
        Pwm1_Init();
        Pwm3_Init();
        Pwm4_Init();
        Pwm5_Init();
        Pwm7_Init();
        ATD0_Init();
        Pit_Init();
        Pit2_Init();
        Ect7_Init();
        Pulse_Add_Init();
        key_port_init();
        DDRT=0X7F;   //液晶使能
        LCD_init();
        LCD_clear();  
        PTT_PTT5=1;   //液晶背光
       
 }
 /*************************************主函数*****************************************************/
void main(void)
 {  
     DisableInterrupts;
     Init_ALL();
     EnableInterrupts;    
    for(;;)            
    {
     LCD_Display(); 
    } 
     /* please make sure that you never leave main */
 }
 