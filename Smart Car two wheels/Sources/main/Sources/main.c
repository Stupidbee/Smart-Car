#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12xs128.h>
#include "FAT16.h"
#include "SDdriver.h"
#include "PLL.H"
#include "PWM.H"
#include "ATD.H"
#include "KEY.H"
#include "LCD5110.H"
#include "FAT.h"
#include "PIT.H"
#include "ECT.H"

/***********************************SD********************/
FIL file;
DIR FileDir;
unsigned char file_name[]={"LIMANGLITXT"};
byte BUFFER[512];
#define active_sd 0

#define center   7500            //舵机中心
#define left 6100   //6200 1400
#define right 9150 //9000 1600

#define mid   6300 
#define behind 4800
#define before 7800

#define AD_NUM 8
unsigned long 
long int Ad_Value[16];
unsigned long  Track_Straight=0,Track_Before=0;
long Lose_Track=0;
long value,Dvalue,text,text1,last_value=0,duoji=6500,duoji2=7500,last_duoji2,last_duoji,duoji_lost,DD=20,DPP,last_PP;
long i=0,j=0,a,b,c,PP=30,max,max1,PP_text[2]={30,30};
long P=50,D1=8,X=9,P_get[4]={15,20,35,50},P1,i1=2,VP1,VD1;
int chaliang,temp[2];
unsigned int speed=100,Speed[6]={500,450,450,400,400,100};  //直到 小弯 小直 大弯（坡道） 出线   刹车
int KP=20;
int KI=20;
int KD=10;
int EK1=0,EK2=0;  
/**********************************************************************************
***********************************************************************************
***********************************************************************************
**********************************************************************************/
unsigned int shache=150;
unsigned int zhidao=230;
unsigned int xiaowan=170;
unsigned int dawan=190;

unsigned char lose_track=0,out_track=0;
unsigned char LCD_Flag=1,Run_Flag=0,Stop_Flag=0,c_flag=0,puodao_fiag=0,puodao_fiag1=0,Run_Flag1=0,stop_flag=0;
int ZD_count=0,XW_count=0,Stop_count=0,count_c=0,count_stop=0,count_wan=0,count_puodao=0,count_run=0,count_shache=0;






/***********PID算法************/
unsigned int Speed_Now=0,Speed_Before=0;
long Speed_Give=3000;
long adjust=0,value,c_value,last_cvalue;
unsigned int SetPoint=0;
unsigned int Proportion=17;    //24
unsigned int Integral=1;
unsigned int Derivative=2;
signed int Speed_Error=0;                                                 
signed int Speed_LastError=0;

//#include "LCDKEY.H"  //液晶控制程序

/***********************FUZZY_steer*打角模糊*******************************/
long PFF[4]={0,200,400,700};

//Output D language feature.  
long DFF[4]={0,16,32,64};

//Output UD language feature.  
long UD[4]={800,1000,1200,1400};

//Output UD language feature.
long UP[4]={25,35,50,60};

long P_rule[7][7]=
{ 
  {3,3,2,2,1,1,0,},

  {3,2,2,1,1,0,0,},

  {3,2,1,0,1,1,3,},

  {3,1,1,0,1,1,3,},//  {3,1,0,0,0,1,3,}

  {3,1,1,0,1,2,3,},

  {0,0,1,1,2,2,3,},

  {0,1,1,2,2,3,3,}
};



long D_rule[7][7]=
{  
 //0,1,2,3,4,5,6,
  {1,1,0,0,0,1,1,},//0

  {2,2,1,0,1,2,2,},//1

  {3,2,1,0,1,2,3,},//2 //{3,3,1,0,1,3,3,}

  {3,3,1,0,1,3,3,},//3

  {3,2,1,0,1,2,3,},//4 //{3,3,1,0,1,3,3,}

  {2,2,1,0,1,2,2,},//5

  {1,1,0,0,0,1,1,} //6  {1,2,0,0,0,2,1,}
};

//////////////FUZZY_speed速度模糊/////////////////////
long pff[4]={0,400,500,700};


long dff[4]={0,16,32,64};

 
long ud[4]={0,300,600,900};

long up[4]={500,450,350,330};

long p_rule[7][7]=
{ 
  {3,3,2,2,1,1,0,},

  {3,2,2,1,1,0,0,},

  {3,2,1,0,1,1,3,},

  {3,1,1,0,1,1,3,},//  {3,1,0,0,0,1,3,}

  {3,1,1,0,1,2,3,},

  {0,0,1,1,2,2,3,},

  {0,1,1,2,2,3,3,}
};



long d_rule[7][7]=
{  
 //0,1,2,3,4,5,6,
  {1,1,0,0,0,1,1,},//0

  {2,2,1,0,1,2,2,},//1

  {3,2,1,0,1,2,3,},//2 //{3,3,1,0,1,3,3,}

  {3,3,1,0,1,3,3,},//3

  {3,2,1,0,1,2,3,},//4 //{3,3,1,0,1,3,3,}

  {2,2,1,0,1,2,2,},//5

  {1,1,0,0,0,1,1,} //6  {1,2,0,0,0,2,1,}
};


#include "Fuzzy_control.h"
#include "B.H"
#include "KEY.H"
#include "LCDKEY.H"  //液晶控制程序




void Read_Atd_Several()  //读取AD
{
    ATD0CTL5=(atd0_scan<<5) + (atd0_mult<<4) + atd0_cd_cc_cb_ca;
    while(!ATD0STAT0_SCF);
    Ad_Value[0]=ATD0DR0;
    Ad_Value[1]=ATD0DR1;
    Ad_Value[2]=ATD0DR2;
    Ad_Value[3]=ATD0DR3;
    //Ad_Value[4]=ATD0DR4;
    Ad_Value[5]=ATD0DR5;
    Ad_Value[6]=ATD0DR6;
    Ad_Value[7]=ATD0DR7;
    
    
}
void puodao_text(void)        //坡道检测
{
  text=Ad_Value[1]+Ad_Value[2];
    if(text>1100&&puodao_fiag1==0) //中间两个的和大于1100
    {
       puodao_fiag=1;
       puodao_fiag1=1;
     }
}

void Get_Track(void)          //失线判断
{
    if(puodao_fiag==0)         //正常失线
    {   
      if(Ad_Value[2]+Ad_Value[1]>450)
        {
           Lose_Track=0;
        } 
      else
        {
          Lose_Track=1;
        }
    }
    if(puodao_fiag==1)        //坡道上失线
    {   
      if(Ad_Value[2]+Ad_Value[1]>350)
        {
           Lose_Track=0;
        } 
      else
        {
          Lose_Track=1;
        }
    }
    if(puodao_fiag1==1)          //进入坡道模式140*5ms
    {
       count_puodao++;
       if(count_puodao>140) puodao_fiag=0;
       if(count_puodao>200) count_puodao=puodao_fiag1=0;
    }
    
    
}

void Get_X(void)             //D项的参数设置
{

     if(Lose_Track==1) X=3; 
     else X=2;
     //X=4;
         
}

void Steer_Controller()     //角度控制
{
        
    c_value=Ad_Value[1]-Ad_Value[2]; //中间两个传感器的差值
    
    value=Ad_Value[3]-Ad_Value[0];    //左右传感器的差值
    Dvalue=value-last_value;
                        
    //Get_X();                          //系数设定
    
    text=Ad_Value[1]+Ad_Value[2];    //中间两个传感器的和，用于计算P值
    if(text>800)text=800;
    if(text<30)text=30;
    PP=800-text;
    DPP=PP-last_PP;
     
    P1=Fuzzy(PP,DPP,1,0);            //模糊计算P值
    
    //P=Fuzzy(value,Dvalue,0);
    //PP=Fuzzy(value,Dvalue,0);
    if(Lose_Track==0)
     { 
       if((Ad_Value[0]+Ad_Value[3])>100)     //失线方向的判断
         {
          if(c_value-value/2>=0) i1=0;
          if(c_value-value/2<0) i1=1;
         } 
      
     }
    if(Lose_Track==1) 
     {
     
        //D1=Fuzzy(PP,DPP,0);
       // DD=X*D1/10;                              
        if((Ad_Value[1]+Ad_Value[2])>300)    //失线方向的判断
          {
           if(value-2*c_value>=0) i1=1;
           if(value-2*c_value<0) i1=0;
          } 
       /*if(i1==1)
           {
              
              duoji2=center+DPP*DD/10+PP*P1/(PP_text[1]+2);//
           }
       if(i1==0)
           {
              
              duoji2=center-DPP*DD/10-PP*P1/(PP_text[1]+4);//
           } */
    
     }
     
     if(i1==1)                  //使两个函数连续，value和pp计算所得值连续
       {
         if(value>(PP-100)) 
          {
             D1=Fuzzy(value,Dvalue,0,0);
             DD=2*D1/10;
             duoji2=center+Dvalue*DD/10+value*P1/(PP_text[0]+2);//
          }// 
         else
          { 
             D1=Fuzzy(PP,DPP,0,0);
             DD=X*D1/10;
             duoji2=center+DPP*DD/10+PP*P1/(PP_text[1]+2);// 
           }
         
      } 
     else if(i1==0)
       {
         if((-value)>(PP-100))
          {
              D1=Fuzzy(value,Dvalue,0,0);
              DD=2*D1/10;
              duoji2=center+Dvalue*DD/10+value*P1/(PP_text[0]+4);//
          }
         else 
          {
              D1=Fuzzy(PP,DPP,0,0);
              DD=X*D1/10;
              duoji2=center-DPP*DD/10-PP*P1/(PP_text[1]+4) ;//
          }
      } 
      else ;
     /*if(c_flag==1)
     {
      count_c++;
      duoji2=center-(5*c_value+3*(c_value-last_cvalue));
      if(count_c==8)
      {
        count_c=0;
        c_flag=0;
        
      }*/
      
      if(((value>160&&c_value>80)||(value<-160&&c_value<-80)&&text>500)||((value>300&&c_value>0)||(value<-300&&c_value<0)&&text<500))  //十字交叉弯的判断
       {
         c_flag=1;
         Stop_Flag=1;
       }
      
      if(c_flag==1)
     {
      count_c++;
      //duoji2=center-(5*c_value);//+2*(c_value-last_cvalue)
      if(text>500)
      {
        if(c_value<40&&c_value>-40)
        {
          duoji2=center;
        }
        else 
         {
           if(c_value>0)  duoji2=center-200;
           if(c_value<=0) duoji2=center+200;
         }
        
      }
      else if(text<=500) 
      {
          if(c_value>0)  duoji2=center-600;
          if(c_value<=0) duoji2=center+600;
      }
      if(count_c==10)  //十字交叉弯持续时间n*5ms
      {
        count_c=0;
        c_flag=0; 
      }
     }
     
      if(duoji>=before)     //4800-7800
        {
            duoji=before;
        }
      if(duoji<=behind)
        {
            duoji=behind;
        } 
        if(duoji_lost>=before)     //7000~3700
        {
            duoji_lost=before;
        }
      if(duoji_lost<=behind)
        {
            duoji_lost=behind;
        }                                                                                                                           
     if(duoji2>=right)    //7800~1500
        {
            duoji2=right;
        }
      if(duoji2<=left)
        {
            duoji2=left;
        }  
     last_value=value;
     last_duoji=duoji;   
     last_duoji2=duoji2;
     last_cvalue=c_value;
     last_PP=PP;
     PWMDTY01=(unsigned int)duoji2;
     PWMDTY67=(unsigned int)duoji;
     
}
void Wafing_Controller(void)               //6900~220摆尾舵机的控制
{
      PWMDTY23=(unsigned int)(7300-(duoji2-center)/7);//
      if(PWMDTY23>7420) PWMDTY23=7420;
      if(PWMDTY23<7180) PWMDTY23=7180;
}

void Get_Speed(void)       //分段取速度
{
      max=Ad_Value[1]+Ad_Value[2];
      max1=Ad_Value[0]+Ad_Value[1]+Ad_Value[2]+Ad_Value[3];
      if(max>500)
         {
          //X=5;
          ZD_count++;
          count_wan=0;
          XW_count=0;      
          speed=Speed[2];
          if(ZD_count>50) 
          {
           speed=Speed[0];
           //if(speed>Speed[0]) speed=Speed[0];
          }
         } 
       else if(max>400&max<=500) 
       {
          XW_count++;
          //ZD_count++;
          count_wan=0;
          speed=Speed[1]; 
         if(XW_count>400)XW_count=400;
       }
       else if(max<=400) 
         {
           //X=6;
           if(ZD_count>70)  Stop_Flag=1;//1    
           else if (ZD_count<=70) 
            {
              count_wan++;
              if(ZD_count>10) Stop_Flag=2;//2
              if(count_wan>200) count_wan=200;
            } 
            speed=Speed[3];
            ZD_count=0;
         } 
      else speed=0;
      
      if(puodao_fiag1==1) speed=Speed[4];
      if(max1<150) speed=Speed[4];
      //if(max1<50) speed=Speed[4];
      
      
      
      if(Stop_Flag==1)
      {  
         speed=Speed[5];
         Stop_count++;
         if(Stop_count>15)
          {
            Stop_count=0;
            Stop_Flag=0;
          }
         ZD_count=0;
      }
      if(Stop_Flag==2)
      {  
         speed=Speed[5];
         Stop_count++;
         if(Stop_count>8)
          {
            Stop_count=0;
            Stop_Flag=0;
          }
         ZD_count=0;
      }
      
      
      if(ZD_count>1200) ZD_count=1200;
}

void Fuzzy_Speed(void) //模糊控制速度
{
    VP1=Fuzzy(PP,DPP,1,1);
    VD1=Fuzzy(PP,DPP,0,1);
    speed=VP1-VD1/10-Speed[5];

}





void Speed_controller(int want,int back)     //速度PID
{
    signed long fuzhi;
    int error;
    int incpid;
    error=want-back;
    incpid=KP*(error-EK1)/30+KI*error/30+KD*(error-EK1*2+EK2)/50;
    //fuzhi=PWMDTY23+KP*(error-EK1)*2+KI*error/5+KD*(error-EK1*2+EK2)/5;
    EK2=EK1;
    EK1=error;
    fuzhi=((signed long)PWMDTY5+incpid);
    if(fuzhi>200) 
    {
      fuzhi=200;
        
    }
     if (fuzhi>=0) 
    {
       PWMDTY4=0;
    }
    if (fuzhi<0) 
    {
       if(fuzhi<-100) fuzhi=-100;
       PWMDTY4=-fuzhi;//0;//
       fuzhi=0;
       
    }
   
    PWMDTY5=(unsigned char)fuzhi;
    //if(Stop_Flag==1) PWMDTY23=0;
    /*if(PWMDTY01>7400&&PWMDTY01<8400)       PWMDTY23=1400;//3000-(PWMDTY01-15500)/2;//3350 ~600
    else if(PWMDTY01>8700) PWMDTY23=1000+(PWMDTY01-7900)/2;
    else if(PWMDTY01<7100) PWMDTY23=1000+(7900-PWMDTY01)/2;
    else PWMDTY23=800;
    if(Ad_Value[0]+Ad_Value[1]+Ad_Value[2]<100) PWMDTY23=1100;  
    PWMDTY23=800;*/
    if(Run_Flag==0)         //起跑按键未按下
    {
       PWMDTY4=0;
       PWMDTY5=0;
    } 
    else if(Run_Flag==1)    //按下起跑按键
    {
       count_run++;
       if(count_run<300)     //起跑后n*5ms出发
       {
         PWMDTY4=0;
         PWMDTY5=0;
       }
       if(count_run>600)      //起跑后n*5ms检测起跑线
       {
          PTT_PTT5=0;
          count_run=600;
          Run_Flag1=1; 
       }
    }
    
    if(stop_flag==1)          //检测起跑线后刹车  
    {
      count_shache++;
      if(count_shache<200)     //反向转n*5ms
        { 
          PWMDTY4=150;
          PWMDTY5=0;
        } 
        else if(count_shache>=200)   //n*5ms后不给舵机复赋值
        { 
          count_stop=100;
          PWMDTY4=0;
          PWMDTY5=0;
        }
    }
}

void Write_SD(void)       //写SD卡
{
  #if active_sd
    if(Run_Flag==1)
    {
      for(a=0;a<4;a++)
      {
        BUFFER[b]=Ad_Value[a]%256;
        b++;
        BUFFER[b]=Ad_Value[a]/256;
        b++;
        
        if(b==512)
        {
          OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
          b=0;
          c+=512;  
        }
      }
      /////////////////////////////////////////////////////////////// 
      /*BUFFER[b]=D1%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=D1/256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      } */
      
      BUFFER[b]=duoji2%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=duoji2/256;
      b++; 
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=(value+1000)%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=(value+1000)/256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=speed%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=speed/256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=Speed_Now%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=Speed_Now/256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=PWMDTY4%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=PWMDTY4/256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=max%256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      BUFFER[b]=max/256;
      b++;
      if(b==512)
      {
        OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
        b=0;
        c+=512;  
      }
      
      ///////////////////////////////////////////////////////////////  
      //When finish data collect stop smartcar.
      if(c>80000)
      {
        PWMDTY4=0;
        PWMDTY5=0;
        Run_Flag=0;
      }
    }
    #endif
}
void PFM_Speed()         //PFM控制速度
{       
        PWMDTY23=PWMPER23/4;
}
         

#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 66 Pit0_interrupt(void) 
{               
        DisableInterrupts;
        Speed_Now=PACNT*2;  //读取编码器返回值
        PACNT=0;
        
        if(Run_Flag==1)     //定时停车
        {
        count_stop++;
        if(count_stop>=P*200) 
        {
          Run_Flag=0;
          count_stop=0;
        }
        
        }
        Read_Atd_Several();  // 读取 AD
        puodao_text();       //  坡道检测 
        Get_Track();         //失线判断
        Steer_Controller();  //舵机控制
        Wafing_Controller(); //摆尾舵机
        Get_Speed();         //分段设置速度
        //Fuzzy_Speed();     //速度的模糊控制
        Speed_controller(speed,Speed_Now); //PID控制速度
        //PFM_Speed();
        Write_SD();           //写SD卡
        PITTF_PTF0=1; 
        EnableInterrupts;   
}

/********************************************************/
/*************************主函数*************************/
void Init_ALL()
{
        long hh=0,c=0;
        Pll_Init(); 
        Pwm01_Init();
        Pwm23_Init();
        Pwm4_Init();
        Pwm5_Init();
        Pwm67_Init();
        ATD0_Init();
        Pit_Init();
        Ect7_Init();
        Pulse_Add_Init();
        init_key();
        DDRT=0XFF;   //液晶使能
        LCD_init();
        LCD_clear();  

        PTT_PTT5=1;   //液晶背光
        PUCR_PUPAE=1;
        PORTE_PE0=1;
    #if active_sd
    
    //Initialize SD card and create file.
    MODRR_MODRR4=1;
    DDRM=0x2e;
  
    SD_Init();
    while(SD_Reset())
      Dly_ms(500);
    SPI_High_Rate();
  
    ReadBPB();
    //FATInit();
    CreateFile(file_name,&file,&FileDir);

    #endif
     /*for(hh=0;hh<512;hh++) {
      BUFFER[hh]=hh;
    }
    for(hh=0;hh<4;hh++) {
      OperateFile(1,file_name,c,512,BUFFER,&file,&FileDir);
      c+=512;
    }
    ;*/
    
}

void main(void)
 {
        int ii;
        DisableInterrupts;
        Init_ALL();
        EnableInterrupts;
        //PWMDTY45=0;      
        //PWMPER23=800;       
       DDRA_DDRA1=0;
       //Run_Flag=1;
        
        
        for(;;)
        {
            if(Run_Flag==0) 
            {
              LCD_Display();
            }
            
            if(Run_Flag1==0)
            {
              
              LCD_Display(); 
            } 
            else if(Run_Flag1==1)
            {
              
                if(PORTA_PA1==0) 
                {
                  for(ii=0;ii<20;ii++)
                  {;}
                    if(PORTA_PA1==0) 
                    {
                      Run_Flag=0;
                      stop_flag=1;
                      //PTT_PTT5=1;
                    }
              
                 } 
            }
            
                        
        } 
  /* please make sure that you never leave main */
}