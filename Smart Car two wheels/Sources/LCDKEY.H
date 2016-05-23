#ifndef  LCDKEY_H
#define  LCDKEY_H

unsigned char key;
unsigned char page=0;
unsigned char Set_y=0;

void LCD_Display(void)           
{
        key=keyscan();
        LCD_write_char(0,Set_y,43);
 
        if(key==1)
        {
         JIAODU_CENTER=(unsigned int)ad_value[0];
         JIAO_SPEED_CENTER=(unsigned int)ad_value[1];
        }
        if(key==2)
        {
                Set_y++;
                LCD_clear();
                if(Set_y==7)
                {
                   Set_y=0;
                }
        }      
      
        if(key==3)
        {
                page++;
                LCD_clear();
                Set_y=0; 
                if(page==5)
                {
                  page=0;
                }
        }
        if(key==8)
        {
         Run_Flag=1;
        }
         if(page==0)
        {       LCD_write_char(1,0,'A');
                LCD_write_char(2,0,'D');
                LCD_write_char(3,0,'0');
                LCD_write_char(1,1,'A');
                LCD_write_char(2,1,'D');
                LCD_write_char(3,1,'1');
                LCD_write_char(1,2,'A');
                LCD_write_char(2,2,'D');
                LCD_write_char(3,2,'2');
             /* LCD_write_char(1,3,'A');
                LCD_write_char(2,3,'D');
                LCD_write_char(3,3,'3');
                LCD_write_char(1,4,'A');
                LCD_write_char(2,4,'D');
                LCD_write_char(3,4,'4');*/  //电感无用值
                
                LCD_write_char(1,5,'A');
                LCD_write_char(2,5,'D');
                LCD_write_char(3,5,'5');
                
             /*   Write_LCD_Int(5,0,(int)JIAODU_CENTER);
                Write_LCD_Int(5,1,(int)JIAO_SPEED_CENTER);
                Write_LCD_Int(5,2,(int)angle);
                Write_LCD_Int(5,3,(int)angle_dot);
                Write_LCD_Int(5,4,(int)LeftMotorOut);
                Write_LCD_Int(5,5,(int)CarSpeed);
              */ 
                Write_LCD_Int(10,0,(int)ad_value[0]);
                Write_LCD_Int(10,1,(int)ad_value[1]);
                Write_LCD_Int(10,2,(int)ad_value[2]);
               /*
                Write_LCD_Int(10,3,(int)ad_value[3]);
                Write_LCD_Int(10,4,(int)ad_value[4]);
               *///  无用电感显示值
                
                
                Write_LCD_Int(10,5,(int)ad_value[5]);
                
                if(Set_y==0)                       
                   {
                        if(key==4)
                         {
                           JIAODU_CENTER+=1;     
                        } else if(key==5)
                        {
                          JIAODU_CENTER-=1;      ;
                        } else if(key==6)
                        {
                          JIAODU_CENTER+=10;         
                        } else if(key==7)
                        {
                          JIAODU_CENTER-=10;         
                        }
                 } 
                 else if(Set_y==1)                       
                   {
                        if(key==4)
                        {
                              JIAO_SPEED_CENTER+=1;   
                        } 
                        else if(key==5)
                        {
                              JIAO_SPEED_CENTER-=1;   
                        } 
                        else if(key==6)
                        {
                              JIAO_SPEED_CENTER+=10;    
                        }
                        else if(key==7)
                        {
                              JIAO_SPEED_CENTER-=10;    
                        }
                        
                   }  
                
        } 
        else if(page==1)
        {
                LCD_write_char(1,0,'Z');
                LCD_write_char(3,0,'P');
                LCD_write_char(1,1,'Z');
                LCD_write_char(3,1,'D');
                LCD_write_char(1,2,'S');
                LCD_write_char(3,2,'P');
                LCD_write_char(1,3,'S');
                LCD_write_char(2,3,'P');
                LCD_write_char(3,3,'W');
                LCD_write_char(1,4,'D');
                LCD_write_char(3,4,'P');
                LCD_write_char(1,5,'D');
                LCD_write_char(3,5,'D');
                Write_LCD_Int(8,0,(int)ZHILI_PID_P);
                Write_LCD_Int(8,1,(int)ZHILI_PID_D);
                Write_LCD_Int(8,2,(int)Sudu_PID_P);
                Write_LCD_Int(8,3,(int)Speed_want);
                Write_LCD_Int(8,4,(int)DIR_CONTROL_P);
                Write_LCD_Int(8,5,(int)DIR_CONTROL_D);
               
               
                 if(Set_y==0)                       
                   {
                        if(key==4)
                         {
                           ZHILI_PID_P+=1;     
                         } 
                         else if(key==5)
                         {
                          ZHILI_PID_P-=1;      ;
                         }
                         else if(key==6)
                         {
                          ZHILI_PID_P+=10;         
                         }
                         else if(key==7)
                         {
                          ZHILI_PID_P-=10;         
                         }
                 } 
                 else if(Set_y==1)                       
                   {
                        if(key==4)
                        {
                              ZHILI_PID_D+=1;   
                        } 
                        else if(key==5)
                        {
                              ZHILI_PID_D-=1;   
                        } 
                        else if(key==6)
                        {
                              ZHILI_PID_D+=10;    
                        }
                        else if(key==7)
                        {
                              ZHILI_PID_D-=10;    
                        }
                        
                   }  
                 else if(Set_y==2) 
                 {
                   if(key==4)
                        {
                          Sudu_PID_P+=1;     
                        } 
                        else if(key==5)
                        {
                          Sudu_PID_P-=1;      ;
                        }
                        else if(key==6)
                        {
                          Sudu_PID_P+=10;         
                        } 
                        else if(key==7)
                        {
                          Sudu_PID_P-=10;         
                        }
                 }
                  else  if(Set_y==3)                       
                    {
                        if(key==4)
                        {
                          Speed_want+=1;     
                        } 
                        else if(key==5)
                        {
                          Speed_want-=1;      ;
                        } 
                        else if(key==6)
                        {
                          Speed_want+=10;         
                        } 
                        else if(key==7)
                        {
                          Speed_want-=10;         
                        }
                    } 
                    
                  else if(Set_y==4) 
                 { 
                        if(key==4)
                        {
                          DIR_CONTROL_P+=1;     
                        } 
                        else if(key==5)
                        {
                          DIR_CONTROL_P-=1;      ;
                        } 
                        else if(key==6)
                        {
                          DIR_CONTROL_P+=10;         
                        } 
                        else if(key==7)
                        {
                          DIR_CONTROL_P-=10;         
                        }
                 }
                 else if(Set_y==5)                       
                   {
                        if(key==4)
                        {
                          DIR_CONTROL_D+=1;   
                        } 
                        else if(key==5)
                        {
                          DIR_CONTROL_D-=1;   
                        } 
                        else if(key==6)
                        {
                          DIR_CONTROL_D+=10;    
                        }
                        else if(key==7)
                        {
                          DIR_CONTROL_D -=10;    
                        }  
                   }  
                 
        } 
        else if(page==2) 
        {
                LCD_write_char(1,0,'P');
                LCD_write_char(2,0,'W');
                LCD_write_char(3,0,'7');
                LCD_write_char(1,1,'P');
                LCD_write_char(2,1,'W');
                LCD_write_char(3,1,'1');
                LCD_write_char(1,2,'P');
                LCD_write_char(2,2,'W');
                LCD_write_char(3,2,'5');
                LCD_write_char(1,3,'P');
                LCD_write_char(2,3,'W');
                LCD_write_char(3,3,'3');
                LCD_write_char(1,4,'L');
                LCD_write_char(2,4,'B');
                LCD_write_char(3,4,'M');
                LCD_write_char(1,5,'R');
                LCD_write_char(2,5,'B');
                LCD_write_char(3,5,'W');
                
                Write_LCD_Int(8,0,PWMDTY7);
                Write_LCD_Int(8,1,PWMDTY1);
                Write_LCD_Int(8,2,PWMDTY5);
                Write_LCD_Int(8,3,PWMDTY3);
                Write_LCD_Int(8,4,(int)SpeedLeft_now);
                Write_LCD_Int(8,5,(int)SpeedRight_now);
        
        }
        else if(page==3) 
        {
                
                LCD_write_char(1,0,'S');
                LCD_write_char(2,0,'P');
                LCD_write_char(3,0,'0');
                LCD_write_char(1,1,'S');
                LCD_write_char(2,1,'P');
                LCD_write_char(3,1,'1');
                LCD_write_char(1,2,'S');
                LCD_write_char(2,2,'P');
                LCD_write_char(3,2,'W');
                LCD_write_char(1,3,'X');
                LCD_write_char(2,3,'S');
                LCD_write_char(3,3,'J');
                
               
                Write_LCD_Int(8,0,(int)Speed[0]);
                Write_LCD_Int(8,1,(int)Speed[1]);
                Write_LCD_Int(8,2,(int)Speed_want);
                Write_LCD_Int(8,3,(int)xiaosjudge);
                
                
            
                  if(Set_y==0)                       
                    {
                        if(key==4)
                        {
                          Speed[0]+=1;     
                        } 
                        else if(key==5)
                        {
                          Speed[0]-=1;      ;
                        } 
                        else if(key==6)
                        {
                          Speed[0]+=10;         
                        } 
                        else if(key==7)
                        {
                          Speed[0]-=10;         
                        }
                    }
                    else  if(Set_y==1)                       
                    {
                        if(key==4)
                        {
                          Speed[1]+=1;     
                        } 
                        else if(key==5)
                        {
                          Speed[1]-=1;      ;
                        } 
                        else if(key==6)
                        {
                          Speed[1]+=10;         
                        } 
                        else if(key==7)
                        {
                          Speed[1]-=10;         
                        }
                    } 
                    
                      
        }
         else if(page==4) 
        {
                Write_LCD_shu(1,0,PWMDTY7);
                Write_LCD_shu(1,2,PWMDTY5);
                Write_LCD_shu(4,4,(int)CarSpeed);
                
                Write_LCD_shu(6,0,PWMDTY1);
                Write_LCD_shu(6,2,PWMDTY3);
        }  
            
}        
#endif