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

#ifndef VAD_FX_H
#define VAD_FX_H
#include "typedef.h"
#include "constdef_fx.h"

/* state of the vad */
typedef struct
{
	Word32 band_enrg_fx[NUM_BAND];
	Word32 band_enrg_bgd_fx[NUM_BAND]; 
	Word32 band_enrg_mov_fx[NUM_BAND];
	Word32 min_band_enrg_fx[NUM_BAND];
    Word32 frm_cnt_fx; 
	Word32 fq_stb_fx;   
	Word16 pre_samples_fx[96]; 	
	Word16 MeanZC_fx; 
	Word16 rms_Input_fx;
	Word16 lsnr_fx;                  
    Word16 fgd_lev_fx;             
    Word16 bgd_lev_fx;               
    Word16 rms_fgd_fx;        
    Word16 rms_bgd_fx;	   
    Word16 emp_smpl_fst_fx;	
	Word16 flux_bgd_fx;
	Word16 flux_bgd_init_cnt_fx; 
	Word16 hang_sp_fx;
    Word16 burst_cnt_fx;
	Word16 burst_cnt1_fx;	
	Word16 bgng_cnt_fx;
	Word16 hang_f_mus_fx;
	Word16 hang_b_mus_fx;   		
	Word16 high_fst_cnt_fx;
	Word16 SP_fx;   
    Word16 bgd_update_cnt_fx;
	Word16 con_frm_cnt_fx;
    Word16 low_SP_cnt_fx;	
    Word16 peak_flux_cnt_fx;
	Word16 tone_stb_cnt_fx;
	Word16 idx_peak_glb_old_fx;
	Word16 max_peak_idx_old_fx;
	Word16 sum_SP_fx;
	Word16 bgd_frm_cnt_fx;
    Word16 bgd_init_reset_cnt_fx;
	Word16 dtmf_flg;
}vadState_fx;

Word16 vad_init_fx(vadState_fx **state_fx);
Word16 vad_reset_fx(vadState_fx *state_fx);
void vad_exit_fx(vadState_fx **state_fx);
Word16 vad_fx(Word16 *Input, vadState_fx *st_fx, Word16 *SigFpw_fx, Word16 *fft_exp_fx, Word16 op, Word16 nb, Word16 *mssnr_ptr_fx);

#endif
