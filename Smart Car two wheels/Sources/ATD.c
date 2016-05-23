#include <mc9s12xs128.h>
#include "ATD.H"


void ATD0_Init(void)
{
        ATD0CTL1_SRES0=atd0_sres0;	
        ATD0CTL2=atd0_ascie<<1;	
        ATD0CTL3=(atd0_djm<<7)+(atd0_sc_length<<3)+(atd0_fifo<<2);
        ATD0CTL4_SMP=atd0_smp;
        ATD0CTL4_PRS=atd0_prs;	
        ATD0CTL5=(atd0_scan<<5) + (atd0_mult<<4) + atd0_cd_cc_cb_ca;
}
