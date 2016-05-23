#ifndef Fuzzy_control_h
#define Fuzzy_control_h

long Fuzzy(long value1,long Dvalue1,unsigned char flag,unsigned char flag1)
{
  unsigned char x1,y1,x2,y2;
  long Error_tmp,DError_tmp,U_tmp[4],UF_tmp[4],UF[4];
  Error_tmp=value1;
  DError_tmp=Dvalue1;
  
  if(Error_tmp<0)Error_tmp=-Error_tmp;
  if(DError_tmp<0)DError_tmp=-DError_tmp;
  
  //Membership function
  if(flag1==0)
  {
    if(Error_tmp<=PFF[1])
    {
     x1=0;
     x2=1;
     UF_tmp[1]=100*(Error_tmp-PFF[0])/(PFF[1]-PFF[0]);
     UF_tmp[0]=100*(PFF[1]-Error_tmp)/(PFF[1]-PFF[0]);
    }
    else if(Error_tmp<=PFF[2])
    {
      x1=1;
      x2=2;
      UF_tmp[1]=100*(Error_tmp-PFF[1])/(PFF[2]-PFF[1]);
      UF_tmp[0]=100*(PFF[2]-Error_tmp)/(PFF[2]-PFF[1]);
    }
    else if(Error_tmp<=PFF[3])
    {
      x1=2;
      x2=3;
      UF_tmp[1]=100*(Error_tmp-PFF[2])/(PFF[3]-PFF[2]);
      UF_tmp[0]=100*(PFF[3]-Error_tmp)/(PFF[3]-PFF[2]);
    }
    else 
    {
      x1=3;
      x2=3;
      UF_tmp[1]=100;
      UF_tmp[0]=100;
    }
  
    if(DError_tmp<=DFF[1])
    {
      y1=0;
      y2=1;
      UF_tmp[3]=100*(DError_tmp-DFF[0])/(DFF[1]-DFF[0]);
      UF_tmp[2]=100*(DFF[1]-DError_tmp)/(DFF[1]-DFF[0]);
    }
    else if(DError_tmp<=DFF[2])
    {
      y1=1;
      y2=2;
      UF_tmp[3]=100*(DError_tmp-DFF[1])/(DFF[2]-DFF[1]);
      UF_tmp[2]=100*(DFF[2]-DError_tmp)/(DFF[2]-DFF[1]);
    }
    else if(DError_tmp<=DFF[3])
    {
      y1=2;
      y2=3;
      UF_tmp[3]=100*(DError_tmp-DFF[2])/(DFF[3]-DFF[2]);
      UF_tmp[2]=100*(DFF[3]-DError_tmp)/(DFF[3]-DFF[2]);
    }
    else 
    {
      y1=3;
      y2=3;
      UF_tmp[3]=100;
      UF_tmp[2]=100;
    }
  
    x1+=3;
    y1+=3;
    x2+=3;
    y2+=3;

    if(UF_tmp[0]<=UF_tmp[2]) UF[0]=UF_tmp[0]; else UF[0]=UF_tmp[2]; 
    if(UF_tmp[0]<=UF_tmp[3]) UF[1]=UF_tmp[0]; else UF[1]=UF_tmp[3]; 
    if(UF_tmp[1]<=UF_tmp[2]) UF[2]=UF_tmp[1]; else UF[2]=UF_tmp[2]; 
    if(UF_tmp[1]<=UF_tmp[3]) UF[3]=UF_tmp[1]; else UF[3]=UF_tmp[3]; 
  
    //Look up the form and calculate the focus.
    if(flag==1)
    {
    U_tmp[0]=UP[P_rule[y1][x1]];
    U_tmp[1]=UP[P_rule[y2][x1]];
    U_tmp[2]=UP[P_rule[y1][x2]];
    U_tmp[3]=UP[P_rule[y2][x2]];
    
    return (U_tmp[0]*UF[0]+U_tmp[1]*UF[1]+U_tmp[2]*UF[2]+U_tmp[3]*UF[3])/(UF[0]+UF[1]+UF[2]+UF[3]);
    }
    else
    {
      U_tmp[0]=UD[D_rule[x1][y1]];
      U_tmp[1]=UD[D_rule[x2][y1]];
      U_tmp[2]=UD[D_rule[x1][y2]];
      U_tmp[3]=UD[D_rule[x2][y2]];
    
    return (U_tmp[0]*UF[0]+U_tmp[1]*UF[1]+U_tmp[2]*UF[2]+U_tmp[3]*UF[3])/(UF[0]+UF[1]+UF[2]+UF[3]);
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////  
/* if(flag1==1)
   {
      if(Error_tmp<=pff[1])
    {
     x1=0;
     x2=1;
     UF_tmp[1]=100*(Error_tmp-pff[0])/(pff[1]-pff[0]);
     UF_tmp[0]=100*(pff[1]-Error_tmp)/(pff[1]-pff[0]);
    }
    else if(Error_tmp<=pff[2])
    {
      x1=1;
      x2=2;
      UF_tmp[1]=100*(Error_tmp-pff[1])/(pff[2]-pff[1]);
      UF_tmp[0]=100*(pff[2]-Error_tmp)/(pff[2]-pff[1]);
    }
    else if(Error_tmp<=pff[3])
    {
      x1=2;
      x2=3;
      UF_tmp[1]=100*(Error_tmp-pff[2])/(pff[3]-pff[2]);
      UF_tmp[0]=100*(pff[3]-Error_tmp)/(pff[3]-pff[2]);
    }
    else 
    {
      x1=3;
      x2=3;
      UF_tmp[1]=100;
      UF_tmp[0]=100;
    //UF_tmp[0]=0;
    }
  
    if(DError_tmp<=dff[1])
    {
      y1=0;
      y2=1;
      UF_tmp[3]=100*(DError_tmp-dff[0])/(dff[1]-dff[0]);
      UF_tmp[2]=100*(dff[1]-DError_tmp)/(dff[1]-dff[0]);
    }
    else if(DError_tmp<=dff[2])
    {
      y1=1;
      y2=2;
      UF_tmp[3]=100*(DError_tmp-dff[1])/(dff[2]-dff[1]);
      UF_tmp[2]=100*(dff[2]-DError_tmp)/(dff[2]-dff[1]);
    }
    else if(DError_tmp<=dff[3])
    {
      y1=2;
      y2=3;
      UF_tmp[3]=100*(DError_tmp-dff[2])/(dff[3]-dff[2]);
      UF_tmp[2]=100*(dff[3]-DError_tmp)/(dff[3]-dff[2]);
    }
    else 
    {
      y1=3;
      y2=3;
      UF_tmp[3]=100;
      UF_tmp[2]=100;
    }
  
    x1+=3;
    y1+=3;
    x2+=3;
    y2+=3;

    if(UF_tmp[0]<=UF_tmp[2])UF[0]=UF_tmp[0]; else UF[0]=UF_tmp[2]; 
    if(UF_tmp[0]<=UF_tmp[3])UF[1]=UF_tmp[0]; else UF[1]=UF_tmp[3]; 
    if(UF_tmp[1]<=UF_tmp[2])UF[2]=UF_tmp[1]; else UF[2]=UF_tmp[2]; 
    if(UF_tmp[1]<=UF_tmp[3])UF[3]=UF_tmp[1]; else UF[3]=UF_tmp[3]; 
  
    //Look up the form and calculate the focus.
    if(flag==1)
    {
    U_tmp[0]=up[p_rule[y1][x1]];
    U_tmp[1]=up[p_rule[y2][x1]];
    U_tmp[2]=up[p_rule[y1][x2]];
    U_tmp[3]=up[p_rule[y2][x2]];
    
    return (U_tmp[0]*UF[0]+U_tmp[1]*UF[1]+U_tmp[2]*UF[2]+U_tmp[3]*UF[3])/(UF[0]+UF[1]+UF[2]+UF[3]);
    }
    else
    {
      U_tmp[0]=ud[d_rule[x1][y1]];
      U_tmp[1]=ud[d_rule[x2][y1]];
      U_tmp[2]=ud[d_rule[x1][y2]];
      U_tmp[3]=ud[d_rule[x2][y2]];
    
    return (U_tmp[0]*UF[0]+U_tmp[1]*UF[1]+U_tmp[2]*UF[2]+U_tmp[3]*UF[3])/(UF[0]+UF[1]+UF[2]+UF[3]);
    }
   }  */


#endif
