/* ITU-T G.729 Software Package Release 2 (November 2006) */
/*
   ITU-T G.729 Annex B     ANSI-C Source Code
   Version 1.3    Last modified: August 1997

   Copyright (c) 1996, France Telecom, Rockwell International,
                       Universite de Sherbrooke.
   All rights reserved.
*/
/* 
   ITU-T G.729 Appendix IV - Reference C code for fixed-point implementation          
   Version 1.0

   Copyright (c) 2012,
   Huawei Technologies
*/

/* SID LSF quantization */
extern Word16 noise_fg[MODE][MA_NP][M];
extern Word16 noise_fg_sum[MODE][M];
extern Word16 noise_fg_sum_inv[MODE][M];
extern Word16 PtrTab_1[32];
extern Word16 PtrTab_2[2][16];
extern Word16 Mp[MODE];

/* SID gain quantization */
extern Word16 fact[NB_GAIN+1];
extern Word16 marg[NB_GAIN+1];
extern Word16 tab_Sidgain[32];



