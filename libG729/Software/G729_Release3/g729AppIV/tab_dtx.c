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

/*********************************************************************/
/******             Tables used for VAD/DTX/CNG                 ******/
/*********************************************************************/
#include <stdio.h>

#include "typedef.h"
#include "ld8k.h"
#include "dtx.h"
#include "tab_dtx.h"

/* Quantization of SID gain */
Word16 fact[NB_GAIN+1] = {410, 26, 13};
Word16 marg[NB_GAIN+1] = {0, 0, 1};
Word16 tab_Sidgain[32] = {
    2,    5,    8,   13,   20,   32,   50,   64,
   80,  101,  127,  160,  201,  253,  318,  401,
  505,  635,  800, 1007, 1268, 1596, 2010, 2530,
 3185, 4009, 5048, 6355, 8000,10071,12679,15962 };

/* Quantization of LSF vector */
Word16 noise_fg[MODE][MA_NP][M];

Word16 noise_fg_sum[MODE][M] = {
  {7798, 8447, 8205, 8293, 8126, 8477, 8447, 8703, 9043, 8604},
  {10514, 12402, 12833, 11914, 11447, 11670, 11132, 11311, 11844, 11447}
};

Word16 noise_fg_sum_inv[MODE][M] = {
  {17210, 15888, 16357, 16183, 16516, 15833, 15888, 15421, 14840, 15597},
  {12764, 10821, 10458, 11264, 11724, 11500, 12056, 11865, 11331, 11724}
};

Word16 PtrTab_1[32] = {96,52,20,54,86,114,82,68,36,121,48,92,18,120,
                         94,124,50,125,4,100,28,76,12,117,81,22,90,116,
                         127,21,108,66};
Word16 PtrTab_2[2][16]= {{31,21,9,3,10,2,19,26,4,3,11,29,15,27,21,12},
                           {16,1,0,0,8,25,22,20,19,23,20,31,4,31,20,31}};

Word16 Mp[MODE] = {8644, 16572};









