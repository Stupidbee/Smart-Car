#ifndef ATD_h
#define ATD_h

#define atd0_sres0 1 // 0 8-bit data
                     // 1 10-bit data
                     // 2 12-bit data
                    
#define atd0_ascie 0 // 0 atd interrupt disable
                     // 1 atd interrupt enable
                     
#define atd0_djm 1 // 0 Left 
                   // 1 Right 
                                     
#define atd0_sc_length 8// The number of channel used
                         // Note:0 for 16 channel 
                         
#define atd0_fifo 0 //
                    //
                    
#define atd0_smp 7 // Sample Time in Number of ATD Clock Cycles
                   // 0  4
                   // 1  6
                   // 2  8
                   // 3  10
                   // 4  12
                   // 5  16
                   // 6  20
                   // 7  24

#define atd0_prs 0 // 0 to 31

#define atd0_scan 0 // 0 Single conversion
                    // 1 Continuous conversion
                    // If it is disable your must initialize ATD again for the next conversion
                    
#define atd0_mult 1 // 0 one channel
                    // 1 several channels
                    
#define atd0_cd_cc_cb_ca 0 // channel select
                           // 0 15 for AN0 to AN15
                           // 





////////////////////////////////////////////////
//    ATD 初始化 
//
//////////////////////////////////////////////////

void ATD0_Init(void);   //	ATD		初始化


void Read_Atd_Several(void);


#endif

