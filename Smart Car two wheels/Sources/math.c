/*****************************************************
   math.c - Float library: mathematical functions
 ----------------------------------------------------
   Copyright (c) Metrowerks, Basel, Switzerland
               All rights reserved
                  Do not modify!
 *****************************************************/
/*
 * Copyright (c) 1985 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/*************************************************************************
 Copyright (c) Metrowerks, Basel, Switzerland
               All rights reserved

Version: Double Precsion 1.1;
Date:    19.8.1992
Updates: 07.07.1993   Corrected 'is_special'.
         12.7.1995    Handling domain error in atan2.
         11/17/95     adaptions for little endian machines (XA)
************************************************************************/

#include <hidef.h>
#include <errno.h>
#include <math.h>

#ifdef __HIWARE__
#ifdef __HC12__
  #pragma MESSAGE DISABLE  C12056 /* WARNING C12056: SP debug info incorrect because of optimization or inline assembler */
#endif
  #pragma MESSAGE DISABLE C2705 /*  Possible loss of data */
#endif
/* values returned by is_special */
#if 0 /* the 'symbolic way */
  typedef enum {
    is_special_none = 0,
    is_special_nan = 1,
    is_special_inf = 2,
    is_special_minf = 3
  } is_special_return;
#else /* the (code/speed) efficient way */
  typedef unsigned char is_special_return;
  #define is_special_none ((unsigned char)0)
  #define is_special_nan  ((unsigned char)1)
  #define is_special_inf  ((unsigned char)2)
  #define is_special_minf ((unsigned char)3)
#endif

#ifdef __DOUBLE_IS_IEEE64__
  #define EXTENDED_PRECISION_ARCTAN 1   /* otherwise atan(0.7) is about 11 digits precise */
  #define EXTENDED_PRECISION_COSSIN 0   /* cos/sin is precise enough */
#else
  #define EXTENDED_PRECISION_ARCTAN 0   /* not needed for IEE32 and DSP */
  #define EXTENDED_PRECISION_COSSIN 0
#endif

#if defined(__DOUBLE_IS_IEEE32__)
#ifdef __XGATE__
  const intdouble x_infinity={0x7f800000UL};          /*lint !e708 initialization applied to the first named member of the union */
#else
  const intdouble infinity={0x7f800000UL};            /*lint !e708 initialization applied to the first named member of the union */
#endif  
  static const intdouble nan_union = {0x7fc00000UL};  /*lint !e708 initialization applied to the first named member of the union */
#elif defined(__DOUBLE_IS_IEEE64__)
  #if defined(__LITTLE_ENDIAN__)
    const intdouble infinity={0, 0x7ff00000};
    static const intdouble nan_union = {0, 0x7ff10000};
  #else
    const intdouble infinity={{0x7ff00000UL, 0UL}};           /*lint !e708 initialization applied to the first named member of the union */
    static const intdouble nan_union = {{0x7ff10000UL, 0UL}}; /*lint !e708 initialization applied to the first named member of the union */
  #endif
#endif

#ifdef __DOUBLE_IS_DSP__
  #define DOUBLE_NAN   0.0
  #define DOUBLE_INF   0.9999695E+9863
#else
  #define DOUBLE_NAN   nan_union.f
#ifdef __XGATE__
  #define DOUBLE_INF   x_infinity.f
#else  
  #define DOUBLE_INF   infinity.f
#endif  
#endif


static double copysign(double x, double y) {
#if defined(__DOUBLE_IS_IEEE32__)
  intfloat ix,iy;

  ix.f = x;
  iy.f = y;
  ix.i = (iy.i & 0x80000000UL) | (ix.i & 0x7fffffffUL);
  return (double)ix.f;
#elif defined(__DOUBLE_IS_IEEE64__)
    intdouble ix,iy;

    ix.f=x;
    iy.f=y;
  #if defined(__LITTLE_ENDIAN__)
    ix.i[3] &= 0x7FFF;
    ix.i[3] |= (iy.i[3] & 0x8000);
  #elif defined(__INT_IS_32BIT__)
    ix.l[0] &= 0x7FFFFFFF;
    ix.l[0] |= (iy.l[0] & 0x80000000);
  #else
    ix.i[0] &= (unsigned short)0x7FFF;
    ix.i[0] |= (iy.i[0] & 0x8000);
  #endif
    return ix.f;
#else
  if (x < 0.0 ) {
    x=-x;
  }
  if((y >= 0.0)) {
    return x;
  }
  return -x;
#endif
}

static int logb(double x) {
  int Exp;

  (void)frexp(x,&Exp);
  return Exp;
}
/************************************************************/
#ifdef fabs
  #undef fabs /* inline version is used in math.h */
#endif
double fabs(double x) {
/*        if( x < 0.0 )
                return -x;
        return x; */

#if defined(__DOUBLE_IS_IEEE32__)
  intfloat ix;

  ix.f=x;
  ix.i &= 0x7fffffffUL;
  return (double)ix.f;
#elif defined(__DOUBLE_IS_IEEE64__)
    intdouble ix;

    ix.f=x;
  #if defined(__LITTLE_ENDIAN__)
    ix.i[3] &= (unsigned short)0x7fff;
  #elif defined(__INT_IS_32BIT__)
    ix.l[0] &= 0x7fffffffUL;            /* INT and LONG have the same size */
  #else
    ix.i[0] &= (unsigned short)0x7fff;
  #endif
    return ix.f;
#else
  if (x < 0.0) {
    return -x;
  } else {
    return x;
  }
#endif
}

/**************************************************************/


static int isint(double x,long int *iy) {
  double y;

  (void)modf(x, &y);
  if(x==y) {         /*lint !e777 the purpose of the test is to compare the bit patterns for an exact match */
    *iy=(long int)y;
    return(1);
  }
  return 0;
}


static is_special_return is_special(double x) {
#ifdef __DOUBLE_IS_IEEE32__
  intfloat ugly;

  ugly.f=x;
  if(ugly.i == 0xff800000) {
    return is_special_minf;
  } 
  if(ugly.i == 0x7f800000UL) {
    return is_special_inf;
  } 
  if((ugly.i & 0x7f800000UL)==0x7f800000UL) {
    return is_special_nan;
  }
  return is_special_none;
#else
  intdouble ugly;

  ugly.f=x;
#ifdef __LITTLE_ENDIAN__
  if((ugly.l[1] ==0xfff00000) && (ugly.l[0]==0x00)) {
    return is_special_minf;
  } else if((ugly.l[1] == 0x7ff00000) && (ugly.l[0]==0x00)) {
    return is_special_inf;
  } else if((ugly.l[1] & 0x7ff00000)==0x7ff00000){
    return is_special_nan;
  }
#else
  if((ugly.l[0] ==0xfff00000UL) && (ugly.l[1]==0x00UL)) {
    return is_special_minf;
  } else if((ugly.l[0] == 0x7ff00000UL) && (ugly.l[1]==0x00UL)) {
    return is_special_inf;
  } else if((ugly.l[0] & 0x7ff00000UL)==0x7ff00000UL){
    return is_special_nan;
  } else { /* do nothing */
  }
#endif
  return is_special_none;
#endif
}

/***************************************************************/
/* Method:
 *        Right to left binary method
 *
 * Complexity: Approx: log2(arg) multiplications
 */

double pow_i(double x, int y) {
  double z,r;
  
  if(y < 0) {
    x=1.0/x;
    y=-y;
  }
  z=x;r=1.0 ;
  switch(y) {
    case 0:return 1.0 ;
    case 1:return x;
    case 2:return x*x;
    case 3:return x*x*x;
    case 4:{r=x*x;return r*r;}
    default: break;
  }
  while( y > 0) {
    unsigned int tmp; /* necessary for compliance with MISRA 10.4 REQ */
    if( (unsigned int)y & 1U) {
      r*=z;
    }
    tmp = (unsigned int)y >> 1U;
    y = (int)tmp;
    if(y != 0) {
      z*=z;
    }
  }
  return r;
}


/*******************************************************/
#define    ln2hi   6.9314718036912381649E-1
#define    ln2lo    1.9082149292705877000E-10
#define   invln2    1.4426950408889633870E0
#define       p1    1.6666666666666601904E-1
#define       p2    -2.7777777777015593384E-3
#define       p3    6.6137563214379343612E-5
#define       p4    -1.6533902205465251539E-6
#define       p5    4.1381367970572384604E-8
/* Method:
 *      1. Argument Reduction: given the input x, find r and integer k such
 *         that
 *                         x = k*ln2 + r,  |r| <= 0.5*ln2 .
 *         r will be represented as r := z+c for better accuracy.
 *
 *      2. Compute exp(r)
 *
 *              exp(r) = 1 + r + r*R1/(2-R1),
 *         where
 *              R1 = x - x^2*(p1+x^2*(p2+x^2*(p3+x^2*(p4+p5*x^2)))).
 *
 *      3. exp(x) = 2^k * exp(r) .
 */

double exp_r(double x) {
  double  z,hi,lo,c;
  int k;

  if (x >= _M_EXP_MAX) {
    errno = ERANGE; return DOUBLE_INF;
  }
  if (fabs(x) < 2.220446049e-16) {
    return 1.0;
  }
  if( x >= -7.5137154372698068983E2  ) {
    /* argument reduction : x --> x - k*ln2 */
    double tmp; /* necessary for compliance with MISRA 10.4 REQ */
    tmp=(invln2*x)+copysign(0.5 ,x);
    k=(int)tmp; /* k=NINT(x/ln2) */
    /* express x-k*ln2 as hi-lo and let x=hi-lo rounded */
    hi=x-((double)k*ln2hi);
    x=hi-(lo=(double)k*ln2lo);
    /* return 2^k*[1+x+x*c/(2+c)]  */
    z=x*x;
    c= x - (z*(p1+(z*(p2+(z*(p3+(z*(p4+(z*p5)))))))));
    return  ldexp(1.0 +(hi-(lo-((x*c)/(2.0 -c)))),k);
  } else {
    return(ldexp(1.0,-5000));
  }
}


double exp (double x)
/* Special cases:
 *      exp(INF) is INF, exp(NaN) is NaN;
 *      exp(-INF)=  0;
 *      for finite argument, only exp(0)=1 is exact.
 */

{
  is_special_return rR;

  rR=is_special(x);
  if(rR != is_special_none) {
    switch(rR) {
      case is_special_nan: return x;
      case is_special_inf:
                errno = ERANGE;
                return DOUBLE_INF;
      case is_special_minf:return 0.0;
      default: break;
    }
  }
  return exp_r(x);
}

/******************************************************/

double log_r(double x) {
  int n;
  double f, znum, zden, z, w, aw, bw, rz2, rz, xn;


  f = frexp(x, &n);
  if (f > _M_S1_2) {
    znum = (f - 1.0  );
    zden = ldexp(f, -1) + .5  ;
  } else {
    --n;
    znum = f - .5  ;
    zden = ldexp(znum,-1) + .5  ;
  }
  z = znum/zden;
  w = z * z;
  aw = (((-.789561128874912572767  * w) + 16.383943563021534222 ) * w) -
       64.124943423745581147  ;
  bw = ((((w + -35.667977739034646171 ) * w) + 312.03222091924532844 ) * w) -
       769.49932108494879777  ;
  rz2 = (aw/bw) * w;
  rz = z + (z * rz2);
  xn = (double)n;
  return (((xn * -2.121944400546905827679E-4)  + rz) + (xn * 0.693359375) );
}


double log (double x)
{
  is_special_return rR;

  rR=is_special(x);
  if(rR != is_special_none) {
    switch(rR) {
      case is_special_nan: return x;
      case is_special_inf:
                errno = ERANGE;
                return DOUBLE_INF;
      case is_special_minf:
                errno = EDOM;
                return DOUBLE_NAN;
      default: break;
    }
  }
  if (x <= 0.0 ) {
    if (x == 0.0 ) {
      errno = ERANGE;
      return -DOUBLE_INF;
    } else{
      errno = EDOM;
      return DOUBLE_NAN;
    }
  }
  return log_r(x);
}


double log10(double x) {
  return(4.3429448190325181667E-1*log(x));
}

/*************************************************************************/
/* Method
 *       exp(y*log(x)); for all legal cases with x > 0.0
 *       x**y using pow_i for integer y
 */
double pow(double x, double y)

{
  long int iy;
  is_special_return rR;

  if((y == 0.0) && (x != 0.0)) {
    return 1.0;
  } 
  if(y == 1.0) {
    return x;
  }
  rR=is_special(y);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan: return y;
      case is_special_inf:{
        if(fabs(x) < 1.0 ) {
          return 0.0;
        } else  {
          errno=ERANGE;
          return DOUBLE_INF;
        }
      }
      case is_special_minf:{
        if(fabs(x) < 1.0 ) {
          errno=ERANGE;
          return DOUBLE_INF;
        } else {
          return 0.0;
        }
      }
      default: break; /* this should never be reached */
    }
  }
  rR=is_special(x);
  if(rR != is_special_none) {
    switch(rR) {
      case is_special_nan: return x;
      case is_special_inf:
                if(y > 0.0 ) {
                  return x;
                } else {
                  return 0.0;
                }
      case is_special_minf:
                if (isint(y,&iy)) {
                  if (((unsigned long)iy & 1UL) != 0UL) {/*odd y */
                    return x;
                  } else {
                    return -x;
                  }
                }
                return DOUBLE_NAN;
       default: break; /* this should never be reached */
    }
  } else if (x == 0.0 ) {
    if (y <= 0.0 ) {
      errno = EDOM;
      return DOUBLE_INF;
    }
    return x;
  } else if (x < 0.0F) {
    if (!isint(y,&iy)) {
      errno = EDOM;
      return DOUBLE_NAN;
    } else {
      if (fabs(y) < 32767.0) {
        return pow_i(x,(int)iy);
      } else {
        x=exp_r(y*log_r(-x));
      }
      if (((unsigned long)iy & 1UL) != 0UL) { /* if odd */
        return -x;
      }
      return x;
    }
  } else {
    /*lint -e{934} no dynamic linking, an absolute address is obtained */
    /*lint -estring(960, "side effects on right hand of logical operator*") , MISRA 12.4 REQ, no impact if 'fabs' is not called during expression evaluation */
    if(isint(y,&iy) && (fabs(y) < 32767.0)) {
      return pow_i(x,(int)iy);
    }
    /*lint +estring(960, "side effects on right hand of  logical operator") */
  }
  return exp_r(y*log_r(x));
}


#define _SIN_HUGE  1 /* sin of more than MAXINT * Pi is not currect,  if set to 0 (old version) */
/******************************************************************************************/
double sincos(double x, int sgn, int flg) {
/* Method:
     1. reduce argument on -Pi/2..Pi/2
     2. taylor expansion of sin at x0=0;
          precision, computed with maple, 40 decimal digits
          "f:= (a,x,n)->evalf(subs(y=x,convert(taylor(sin(y),y=a,n),polynom))-sin(x));"
          "i:=0: while (i < 30) do x:=f(0, Pi/2,i): print(i, x, -evalf(log(abs(x))/log(10.0),4)); i:=i+1: od:"
              1, -1., 0
              3, .570796326794896619231321691639751442099, .2435
              5, -751677707113496344244348722581942912805.0E-40, 1.124
              7, 4524855534817410696070677232283731240.0E-39, 2.344
              9, -1568986005012774046147867016697066195.0E-40, 3.804
              11, 3542584286082417257874168493766710.0E-39, 5.449
              13, -562589491296680825843715244416794.0E-40, 7.247
              15, 662780090011185533381028594242.0E-39, 9.175
              17, -60234209699611898667940393363.0E-40, 11.22
              19, 43514761145005800307406329.0E-39, 13.36
              21, -2558935281316224644363841.0E-40, 15.59
              23, 1248761154424922179002.0E-39, 17.90
              25, -51383861105354863381.0E-40, 20.29
              27, 18069066044795996.0E-39, 22.74
              29, -549270836928788.0E-40, 25.25

  so we use a polynom of degree 23 for IEEE64 and one of degree 17 for IEEE32

                  taylor(sin(x),x=0, 30) ==
                x-.1666666666666666666666666666666666666667*x^3+
                8333333333333333333333333333333333333333.0E-42*x^5-
                1984126984126984126984126984126984126984.0E-43*x^7+
                2755731922398589065255731922398589065256.0E-45*x^9-
                2505210838544171877505210838544171877505.0E-47*x^11+
                1605904383682161459939237717015494793273.0E-49*x^13-
                7647163731819816475901131985788070444155.0E-52*x^15+
                2811457254345520763198945583010320016233.0E-54*x^17-
                8220635246624329716955981236872280749221.0E-57*x^19+
                1957294106339126123084757437350543035529.0E-59*x^21-
                3868170170630684037716911931522812323179.0E-62*x^23+
                6446950284384473396194853219204687205299.0E-65*x^25-
                9183689863795546148425716836473913397862.0E-68*x^27+
                1130996288644771693155876457693831699244.0E-70*x^29
  */

#if !_SIN_HUGE
  int n;
#endif
  double y, frac, xn, f, t1, rv;

  y = x;
  if (flg) {
    y +=  _M_PI2;
  }

  frac = modf(y *  _M_1PI, &xn);
#if !_SIN_HUGE
  n = xn;
  if (frac > 0.5 ) {
    xn = ++n;
  }
  if (frac < -0.5 ) {
    xn = --n;
  }
  if ((n & 1) != 0) {
    sgn = !sgn;
  }
#else
  if (frac > 0.5 ) {
    xn++;
  }
  if (frac < -0.5 ) {
    xn++;
  }
  if (((unsigned long)xn & 1UL) != 0UL) {
    sgn = !sgn;
  }
#endif
  if (flg) {
    xn -= 0.5 ;
  }
#ifdef __DOUBLE_IS_IEEE32__
  f = (x - (xn * 3.1416016) ) + (xn * 8.908910206E-6) ; /* correction term for Pi rounding */
#else
  f = x - (xn * _M_PI);
#endif
  if (fabs(f) <  _M_EPS) {
    rv = f;
  } else {
     t1 = f*f;
     
     
#if !_SIN_HUGE /* more summands for higher precision */
     rv = (((((((0.27204790957888846175e-14  * t1 - 0.76429178068910467734e-12 ) * t1 +
             0.16058936490371589114e-9 ) * t1 - 0.25052106798274584544e-7 )  * t1 +
             0.27557319210152756119e-5 ) * t1 - 0.19841269841201840457e-3 )  * t1 +
             0.83333333333331650314e-2 ) * t1 - 0.16666666666666665052 )     * t1;
#else
    rv = (((((((((((((((
#if EXTENDED_PRECISION_COSSIN
           (((
                 3868170170630684037716911931522812323179.0E-62) * t1 +
                 1957294106339126123084757437350543035529.0E-59) * t1 -
                 8220635246624329716955981236872280749221.0E-57) * t1 +
#endif
                 2811457254345520763198945583010320016233.0E-54) * t1) -
                 7647163731819816475901131985788070444155.0E-52) * t1) +
                 1605904383682161459939237717015494793273.0E-49) * t1) -
                 2505210838544171877505210838544171877505.0E-47) * t1) +
                 2755731922398589065255731922398589065256.0E-45) * t1) -
                 1984126984126984126984126984126984126984.0E-43) * t1) +
                 8333333333333333333333333333333333333333.0E-42) * t1) -
                 0.1666666666666666666666666666666666666667 )    * t1;

#endif     
      rv=f+(rv*f);
  }
  if(sgn) {
    return (-rv);
  }
  return (rv);
}




/*****************************************************/
double tan(double x) {
  is_special_return nR;
  double frac, xn, f, xnum, xden, g;

  nR=is_special(x);
  if(nR!=is_special_none) {
    switch(nR) {
      case is_special_nan:return x;
      case is_special_inf:return 0.0 ;
      case is_special_minf:return 0.0 ;
      default: break;
    }
  }
#if !_SIN_HUGE
  {
    double y;
    
    y=fabs(x);
    if (y > 149078413.0 ) {
      errno = ERANGE; 
      return DOUBLE_INF;
    }
  }
#endif
  frac = modf( x * 0.63661977236758134308 , &xn);
#if !_SIN_HUGE
  n = xn;
  if (frac > 0.5) {
    xn = ++n;
  }
  if (frac < -0.5) {
    xn = --n;
  }
#else
  if (frac > 0.5) {
    xn++;
  }
  if (frac < -0.5) {
    xn--;
  }
#endif

#ifdef __DOUBLE_IS_IEEE32__
  f = (x - (xn * 1.57080078125) ) - (xn * -4.454455103380768678308e-6) ;
#else
  f = x - (xn * (_M_PI/2.0));
#endif
  if (fabs(f) < _M_EPS) {
    xnum = f;
    xden = (double)1;
  } else {
    g = f * f;
    xnum = (((((-0.17861707342254426711e-4  * g) + 0.34248878235890589960e-2 ) * g) -
              0.13338350006421960681 )* g * f) + f;
    xden = (((((((0.49819433993786512270e-6  * g) - 0.31181531907010027307e-3 ) * g) +
              0.25663832289440112864e-1 )* g) - 0.46671683339755294240 ) * g) + 1.0 ;
  }
  if((unsigned long)xn & 1UL) {
    xnum = -xnum;
    return (xden/xnum);
  } else {
    return (xnum/xden);
  }
}


double sin(double x)
{
  int sgn;
  is_special_return rR;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan:  return x;
      case is_special_inf:  return 0.0;
      case is_special_minf: return 0.0;
      default: break; /* this is never reached */
    }
  }
  if (x == 0.0 ) {
    return 0.0 ;
  }
  if (x < 0.0 ) {
    sgn = 1;
    x = -x;
  } else {
    sgn = 0;
  }
  return (sincos(x,sgn,0));
}

/*****************************************************/

double cos(double x) {
  is_special_return rR;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan:return x;
      case is_special_inf:return 0.0;
      case is_special_minf:return 0.0;
      default: break; /* this is never reached */
    }
  } 
  if(x == 0.0 ) {
    return 1.0 ;
  }

  return (sincos(fabs(x),0,1));
}

/*****************************************************/
/* Method:
 *       repetitiv approx. using Heron's method
 */
double sqrt_r(double x)
{
  int n;
  double f, z, y0, y2, y3;


  if (x == 0.0 ) {
    return (x);
  }
  f = frexp(x,&n);
  y0 = .41731  + (.59016  * f);
  
  z = y0 + (f/y0);
  y2 = ldexp(z,-2) + (f/z);
  y3 = ldexp(y2 + (f/y2),-1);
  if ((unsigned int)n & 1U) {/* exponent is odd */
    y3 = y3 * _M_S1_2;
    ++n;
  } 
  {
    unsigned long tmp = (unsigned long)(long)n >> 1U; /* necessary for compliance with MISRA 10.3 REQ */
    n = (int)tmp;
  }
  return (ldexp(y3,n));
}

double sqrt(double x) {
  is_special_return rR;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan: return x;
      case is_special_inf: return DOUBLE_INF;
      case is_special_minf:
                errno = EDOM;
                return DOUBLE_NAN;
      default: break; /* this is never reached */
    }
  } 
  if (x < 0.0 ) {
    errno = EDOM;
    return DOUBLE_NAN;
  }
  
  return sqrt_r(x);
}
/*************************************************************/
#define MATH_athfhi  4.6364760900080611433E-1
#define MATH_athflo  1.9338828231967579916E-19
#define MATH_PIo4    7.8539816339744830676E-1
#define MATH_at1fhi  9.8279372324732906796E-1
#define MATH_at1flo  -3.5540295636764633916E-18
#define MATH_PIo2    1.5707963267948966135E0
#define MATH_PI      3.1415926535897932270E0

/*lint --e{750} suppress the messages on several local macros not being referenced */

/* polynom coefficient calculated with maple taylor(arctan(x). x=0, 30); */
/* here the negated coefficients are given */
/* #define a0         -1 */ /* not used */
#define MATH_a1          0.33333333333333333333333333333333333333333333333333333333333333333333333333
#define MATH_a2         -0.20000000000000000000000000000000000000000000000000000000000000000000000000
#define MATH_a3          0.14285714285714285714285714285714285714285714285714285714285714285714285714
#define MATH_a4         -0.11111111111111111111111111111111111111111111111111111111111111111111111111
#define MATH_a5          0.09090909090909090909090909090909090909090909090909090909090909090909090909
#define MATH_a6         -0.07692307692307692307692307692307692307692307692307692307692307692307692307
#define MATH_a7          0.06666666666666666666666666666666666666666666666666666666666666666666666666
#define MATH_a8         -0.05882352941176470588235294117647058823529411764705882352941176470588235294
#define MATH_a9          0.05263157894736842105263157894736842105263157894736842105263157894736842105
#define MATH_a10        -0.04761904761904761904761904761904761904761904761904761904761904761904761904
#define MATH_a11         0.04347826086956521739130434782608695652173913043478260869565217391304347826
#define MATH_a12        -0.04000000000000000000000000000000000000000000000000000000000000000000000000
#define MATH_a13         0.03703703703703703703703703703703703703703703703703703703703703703703703703
#define MATH_a14        -0.03448275862068965517241379310344827586206896551724137931034482758620689655
#define MATH_a15         0.03225806451612903225806451612903225806451612903225806451612903225806451612
#define MATH_a16        -0.03030303030303030303030303030303030303030303030303030303030303030303030303
#define MATH_a17         0.02857142857142857142857142857142857142857142857142857142857142857142857142
#define MATH_a18        -0.02702702702702702702702702702702702702702702702702702702702702702702702702
#define MATH_a19         0.02564102564102564102564102564102564102564102564102564102564102564102564102
#define MATH_a20        -0.02439024390243902439024390243902439024390243902439024390243902439024390243
#define MATH_a21         0.02325581395348837209302325581395348837209302325581395348837209302325581395
/* so for x close to 0, arctan(x ) == - (MATH_a0*x + MATH_a1*x^3+MATH_a2*x^5+MATH_a3*x^7+...MATH_an*x^(2*n+1) */

/* Method :
 *      1. Reduce y to positive by atan2(y,x)=-atan2(-y,x).
 *      2. Reduce x to positive by (if x and y are unexceptional):
 *              ARG (x+iy) = arctan(y/x)           ... if x > 0,
 *              ARG (x+iy) = pi - arctan[y/(-x)]   ... if x < 0,
 *      3. According to the integer k=4t+0.25 truncated , t=y/x, the argument
 *         is further reduced to one of the following intervals and the
 *         arctangent of y/x is evaluated by the corresponding formula:
 *
 *         [0,7/16]        atan(y/x) = t - t^3*(MATH_a1+t^2*(a2+...(MATH_a10+t^2*MATH_a11)...)
 *         [7/16,11/16]    atan(y/x) = atan(1/2) + atan( (y-x/2)/(x+y/2) )
 *         [11/16.19/16]   atan(y/x) = atan( 1 ) + atan( (y-x)/(x+y) )
 *         [19/16,39/16]   atan(y/x) = atan(3/2) + atan( (y-1.5x)/(x+1.5y) )
 *         [39/16,INF]     atan(y/x) = atan(INF) + atan( -x/y )
 *
 */

static double SetSign(double x, signed char isNeg) {
  x= fabs(x);
  if (isNeg) {
    x= -x;
  }
  return x;
}

double atan2(double y,double x)
{
  static const double small=1.0E-9 , big=1.0E18 ;
  double t,z,hi,lo;
  signed char yIsNeg, xIsNeg;
  int k,m;
  is_special_return mR;

  if ((x == 0.0) && (y == 0.0)) {
    errno = EDOM;
    return 0.0;
  }
  /* copy down the sign of y and x */
  yIsNeg = (signed char)(y < 0.0);
  xIsNeg = (signed char)(x < 0.0);
  mR= is_special(x);
  if (mR != is_special_none) {
    switch (mR) {
      case is_special_nan:return y;
      case is_special_inf:
      case is_special_minf:
                if (is_special(y) != is_special_none) {
                  return (SetSign(!xIsNeg ? MATH_PIo4 : (3.0 * MATH_PIo4), yIsNeg));
                } else {
                  return (SetSign(!xIsNeg ? 0.0 : MATH_PI, yIsNeg));
                }
      default: break;
    }
  }
  mR=is_special(y);
  if(mR!=is_special_none) {
    switch(mR) {
      case is_special_nan:return y;
      case is_special_inf:
      case is_special_minf:return(SetSign(MATH_PIo2, yIsNeg));
      default: break;
    }
  }

  /* if x is 1.0 */
  if(x==1.0 ) {
    y=SetSign(y,0); t=y;
  } else {
    /* when y = 0 */
    if(y==0.0) {
      if(!xIsNeg) {
        return(y);
      } else {
        return(SetSign(MATH_PI,yIsNeg));
      }
    }
    if(x==0.0) {
      return(SetSign(MATH_PIo2,yIsNeg));
    }
    x=fabs(x);
    y=fabs(y);
    k=logb(y);
    m=k-logb(x);
    if(m > 60) {
      t=big+big;
    } else if(m < -80 ) {
      t=y/x;
    } else {
      t = y/x;
      y = ldexp(y,-k);
      x=ldexp(x,-k);
    }
  }
  /* begin argument reduction */
  if (t < 2.4375 ) {
    /* truncate 4(t+1/16) to integer for branching */
    double tmp; /* necessary for compliance with MISRA 10.4 REQ */
    tmp = 4.0 * (t+0.0625 );
    k = (int)tmp;
    switch (k) {
        /* t is in [0,7/16] */
      case 0:
      case 1:
        if (t < small) {
          return (SetSign(!xIsNeg?t:(MATH_PI-t),yIsNeg));
        }
        hi = 0.0;  lo = 0.0;  break;

        /* t is in [7/16,11/16] */
      case 2:
        hi = MATH_athfhi; lo = MATH_athflo;
        z = x+x;
        t = ( (y+y) - x ) / ( z +  y ); break;

        /* t is in [11/16,19/16] */
      case 3:
      case 4:
        hi = MATH_PIo4; lo = 0.0;
        t = ( y - x ) / ( x + y ); break;

        /* t is in [19/16,39/16] */
      default:
        hi = MATH_at1fhi; 
        lo = MATH_at1flo;
        z = y-x; 
        y=y+y+y; 
        t = x+x;
        t = ( (z+z)-x ) / ( t + y ); 
        break;
    }
  /* end of if (t < 2.4375) */
  } else {
    hi = MATH_PIo2; 
    lo = 0.0;

    /* t is in [2.4375, big] */
    if (t <= big)  {
      t = - x / y;
    /* t is in [big, INF] */
    } else {
      t = 0.0;
    }
  }
  /* end of argument reduction */

  /* compute atan(t) for t in [-.4375, .4375] */
  z = t*t;
  z = t*(z*(MATH_a1+(z*(
    MATH_a2+(z*(
    MATH_a3+(z*(
    MATH_a4+(z*(
    MATH_a5+(z*(
    MATH_a6+(z*(
    MATH_a7+(z*(
    MATH_a8+(z*(
    MATH_a9+(z*(
    MATH_a10+(z*(
#if EXTENDED_PRECISION_ARCTAN
    MATH_a11+(z*(
    MATH_a12+(z*(
    MATH_a13+(z*(
    MATH_a14+(z*(
    MATH_a15+(z*(
    MATH_a16+(z*(
    MATH_a17+(z*(
    MATH_a18+(z*(
    MATH_a19+(z*(
    MATH_a20+(z*(
    MATH_a21
    ))))))))))))))))))))
#else
   MATH_a11
#endif
    ))))))))))))))))))))));

  z = lo - z; z += t; z += hi;
  return (SetSign(!xIsNeg ? z : (MATH_PI - z), yIsNeg));
}


double asin(double x) {
  is_special_return rR;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan: return x;
      case is_special_inf:
                errno = EDOM;
                return DOUBLE_NAN;
      case is_special_minf:
                errno = EDOM;
                return DOUBLE_NAN;
      default: break;
    }
  }
  if(fabs(x) > 1.0 ) {
    errno = EDOM;
    return DOUBLE_NAN; 
  }
  return(atan2(x,sqrt(1.0 -(x*x))));
}


double acos(double x)
{
  double t;
  is_special_return rR;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan:   return x;
      case is_special_inf:
                  errno = EDOM;
                  return DOUBLE_NAN;
      case is_special_minf:
                  errno = EDOM;
                  return DOUBLE_NAN;
      default: break; /* this is never reached */
    }
  }
  if(fabs(x) > 1.0 ) {
    errno = EDOM;
    return DOUBLE_NAN;
  }
  if( x != -1.0 ) {
    t=atan2(sqrt((1.0 -x)/(1.0 +x)),1.0 );
  } else {
    t=MATH_PIo2;
  }
  return(t+t);
}

double atan(double x) {
  return(atan2(x,1.0 ));
}

/*************************************************************/
double sncsh(double y, int flg)
{
  double w, z;

  if (y > _M_EXP_MAX){ 
    w = y - 0.69316101074218750000 ;
    if (w > _M_EXP_MAX) {
      errno = ERANGE;
    }
    /* range err */
    z = exp(w);
    return (z + (0.13830277879601902638e-4  * z));
  }
  z = exp(y);
  if(flg) {
    return ((z - (1.0/z))/2.0);
  }
  return ((z + (1.0/z))/2.0);
}

/*****************************************************/

double sinh(double x)
{
  is_special_return rR;
  double y, f, pf, qf, rf, sign;

  rR=is_special(x);
  if(rR!=is_special_none) {
     switch(rR) {
       case is_special_nan: return x;
       case is_special_inf:
                 errno = ERANGE;
                 return DOUBLE_INF;
       case is_special_minf:
                 errno = ERANGE;
                 return -DOUBLE_INF;
       default: break;
     }
  }
  sign=copysign(1.0,x);
  y=copysign(x,1.0);
  if (y > 1.0) {
    return copysign(sncsh(y,1),sign);
  }
  if (y < _M_EPS) {
    return (x);
  }
  f = y * y;
  pf = (((((-0.78966127417357099479  * f) - 0.16375798202630751372e+3 ) * f)
        - 0.11563521196851768270e+5 ) * f) - 0.35181283430177117881e+6 ;

  qf = ((((f - 0.27773523119650701667e+3 ) * f)
           + 0.36162723109421836460e+5 ) * f)
           - 0.21108770058106271242e+7 ;

  rf = f * (pf/qf);

  return (x + (x * rf));
}
/*****************************************************/
double cosh(double x) {
  is_special_return rR;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan:return x;
      case is_special_inf:
        errno = ERANGE;
        return DOUBLE_INF;
      case is_special_minf:
        errno = ERANGE;
        return DOUBLE_INF;
      default: break;
    }
  }
  x=fabs(x);
  return (sncsh(x,0));
}

/*****************************************************/

double tanh(double x)
{
  is_special_return rR;
  double sign,f, g, gpg, qg, rg, rv;

  rR=is_special(x);
  if(rR!=is_special_none) {
    switch(rR) {
      case is_special_nan: return x;
      case is_special_inf:
                return 1.0;
      case is_special_minf:
                return -1.0;
      default: break;
     }
  }
  sign=copysign(1.0,x);
  f=copysign(x,1.0);
  if (f < _M_EPS) {
    return (x);
  }

  if (f > 19.06154747 ) {
    return copysign(1.0,sign);
  }
  if (f > 0.54930614433405484570 ) {
    rv = 0.5  - (1.0 /(exp(f + f) + 1.0 ));
    rv = ldexp(rv,1);
    return copysign(rv,sign);
  }

  g = f * f;

  gpg = ((((-0.96437492777225469787  * g)
         - 0.99225929672236083313e+2 ) * g)
         - 0.16134119023996228053e+4 ) * g;

  qg = ((((g + 0.11274474380534949335e+3 ) * g)
           + 0.22337720718962312926e+4 ) * g)
           + 0.48402357071988688686e+4 ;

  rg = gpg / qg;
  rv = f + (f * rg);
  return copysign(rv,sign);
}

/***********************************************************************/
double ceil(double x) {
  double y;

  (void)modf(x, &y);
  if ((x != y) && (x > 0.0) ) { /*lint !e777 the purpose of the test is to compare the bit patterns for an exact match */
    return (y + 1.0 );
  }
  return(y);
}

double floor(double x) {
  double y;

  (void)modf(x, &y);
  if ((x != y) && (x < 0.0) ) { /*lint !e777 the purpose of the test is to compare the bit patterns for an exact match */
    return (y - 1.0 );
  }
  return(y);
}


double fmod(double x, double y) {
  double z;

  if (y == 0.0 ) {
    return (0.0 );
  }
  (void) modf (x / y, &z);
  return (x - (z * y));
}

/****************************************************************/

/*lint --e{766} hidef.h contains conditionally compiled code */
