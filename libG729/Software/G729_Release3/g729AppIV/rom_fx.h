/*
 --------------------------------------------------------------------------
 ITU-T G.720.1 Source Code
 Software Release 1.0a (2010-03)
 (C) 2009 Huawei Technologies Co., LTD. 
 --------------------------------------------------------------------------
*/
/* 
   ITU-T G.729 Appendix IV - Reference C code for fixed-point implementation          
   Version 1.0

   Copyright (c) 2012,
   Huawei Technologies
*/

#include <stdlib.h>
#include "typedef.h"
#include "constdef_fx.h"

extern  Word16 band_tbl_16k[NUM_BAND*2];
extern  Word16 band_tbl_8k[NUM_BAND*2];
extern  Word16 vad_thr_tbl_8k_fx[4][3][3];
extern  Word16 vad_thr_tbl_16k_fx[4][3][3];
extern  Word16 burst_thr_tbl_fx[3][4][2];
extern  Word16 hang_sp_tbl_fx[3][4][2];                          
extern  Word16 hang_sp_tbl_fx[3][4][2];
extern  Word16 Sqrt_Taylor_coeff_fx[6] ;
extern  Word16 Sqrt_exp_factor_fx[14] ;
extern  Word16 log10_Taylor_coeff_fx[6];
extern  Word16 log10_exp_factor_fx[15]; 
extern  Word16 log10_exp_factor_internal_fx[16]; 
extern  Word16 ssnr_seg_len[16];
extern  Word16 ssnr_seg_len_8k[16];
extern  Word16 Sqrt_exp_factor_32bit_fx[31];
extern  Word16 window_16k_fx[FFTSIZE16k];
extern  Word16 FFT_phs_tbl_128_fx[128];
extern  Word16 FFT_phs_tbl_256_fx[256]; 
extern  Word16 FFT_i_table_128_fx[28];
extern  Word16 FFT_j_table_128_fx[28];
extern  Word16 FFT_i_table_256_fx[56];
extern  Word16 FFT_j_table_256_fx[56];
extern  Word16 Inv_len[5];
