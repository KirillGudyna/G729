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
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "Rom_fx.h"
#include "vad_fx.h"
#include "parameters_fx.h"
#include "stl.h"
#include "gsad_math_adv_fx.h"


#ifdef WMOPS_FX
#ifdef DETAIL
extern int restId;
extern int zcrId;
extern int dzcrId;
extern int ssnrId;
extern int TonalityId;
extern int spdevId;
extern int bcgUpdtId;
extern int lsnrId;
extern int MusBcgdId;
extern int bcgStaId;
#else
extern int restId;
#endif
#endif


/**********************************************************************
* FUNCTION      :  vad_init_fx ()                                     *
*---------------------------------------------------------------------*
* PURPOSE       :  allocates and initializes vad state memory         *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*                 None                                                *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*   (smdState *)  st     : initialized vad state                      *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*                 None                                                *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word32)             : non-zero with error, zero for ok           *
**********************************************************************/
Word16 vad_init_fx(vadState_fx **state_fx)
{
	vadState_fx *s_fx;
	
    IF (state_fx == (vadState_fx **) NULL)
    {
        fprintf(stderr, "vad_init: invalid parameter\n");
        return -1;
    }
    *state_fx = NULL;                                                     
    
	/* allocate memory */	
    IF ((s_fx = (vadState_fx *) malloc(sizeof(vadState_fx)))==NULL)
    {
        fprintf(stderr, "vad_init: cannot allocate memory for state structure\n");
        return -1;
    }
    vad_reset_fx(s_fx);
    *state_fx = s_fx;                                                       
    return 0;
}


/**********************************************************************
* FUNCTION      :  vad_reset_fx ()                                    *
*---------------------------------------------------------------------*
* PURPOSE       :  initializes vad state memory                       *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*                 None                                                *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                 None                                                *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (smdState *)  st     : vad state                                  *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word32)             : non-zero with error, zero for ok           *
**********************************************************************/
Word16 vad_reset_fx(vadState_fx *state_fx)
{
	Word16 i;	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 1);
#endif

    IF (state_fx==(vadState_fx *) NULL)
    {
        fprintf(stderr, "vad_reset: invalid parameter\n");
        return -1;
    }
#ifdef WMOPS_FX
	move32();move32();
	move32();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
	move16();move16();
#endif

    /* reset vadState */
    state_fx->emp_smpl_fst_fx = 0;                                                                                 
	state_fx->frm_cnt_fx = 0;
	state_fx->bgd_update_cnt_fx = 0; 
    state_fx->hang_sp_fx = 0;
    state_fx->burst_cnt_fx = 0;                                                     
	state_fx->lsnr_fx = 3840;                 
	state_fx->rms_fgd_fx = 2400;            
	state_fx->rms_bgd_fx = 4800;   
	state_fx->fgd_lev_fx = -9216;           
	state_fx->bgd_lev_fx = -10496;               
	state_fx->flux_bgd_init_cnt_fx = 100;  
	state_fx->burst_cnt1_fx = 0; 
	state_fx->bgng_cnt_fx = 200; 
	state_fx->max_peak_idx_old_fx = 0;
	state_fx->tone_stb_cnt_fx = 0;
	state_fx->hang_f_mus_fx = 0;
	state_fx->hang_b_mus_fx = 0;
    state_fx->flux_bgd_fx = 4096;
	state_fx->SP_fx = 0;
	state_fx->bgd_frm_cnt_fx = 0;
	state_fx->sum_SP_fx = 0;
	state_fx->fq_stb_fx = 0; 
	state_fx->high_fst_cnt_fx = 0; 
	state_fx->idx_peak_glb_old_fx = 0; 
	state_fx->peak_flux_cnt_fx = 0; 
	state_fx->low_SP_cnt_fx = 0;
	state_fx->bgd_init_reset_cnt_fx = 0;
	
    for (i=0; i<NUM_BAND; i++)
    {
#ifdef WMOPS_FX
		move32();move32();
        move32();move32();
#endif		
		state_fx->band_enrg_mov_fx[i] = 30;
        state_fx->band_enrg_fx[i] = 30;
        state_fx->band_enrg_bgd_fx[i] = 30;                                            
		state_fx->min_band_enrg_fx[i] = 30;
    }	
	
	for (i=0; i<96; i++)
	{
#ifdef WMOPS_FX
		move16();
#endif	
		state_fx->pre_samples_fx[i] = 0;
	}	
#ifdef DRAM
    accDRAM(FRAME_COUNT, -1);
#endif	
	return (0);
}


/**********************************************************************
* FUNCTION      :  vad_exit_fx ()                                     *
*---------------------------------------------------------------------*
* PURPOSE       :  frees vad state memory                             *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (smdState *)  st     : vad state                                  *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                 None                                                *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*                 None                                                *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                 None                                                *
**********************************************************************/
void vad_exit_fx(vadState_fx **state_fx)
{
	if (state_fx==NULL || *state_fx==NULL)
	{
		return;
	}
    /* deallocate memory */
    free(*state_fx);
    *state_fx = NULL;
    return;
}


/**********************************************************************
* FUNCTION      :  vad_fx ()                                          *
*---------------------------------------------------------------------*
* PURPOSE       :  performs the voice activity detection and outputs  *
*                  activity indicator                                 *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)   Input   : pointer to the frame of input signal       *
*   (Word16)     op      : operating point flag                       *
*   (Word16)     nb      : sampling rate flag                         *
*   (Float32 *)  SigFpw  : pointer to the spectrum power density of   *
*                          the speech frame                           *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st      : the vad state                              *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word16)     ivad    : voice activity indicator                   *
**********************************************************************/
Word16 vad_fx(Word16 *Input, vadState_fx *st_fx,  
			  Word16 *SigFpw_fx, Word16 *fft_exp_fx, 
			  Word16 op, Word16 nb, 
			  Word16 *mssnr_ptr_fx)
{
	Word16 i, tmp;
	Word16 zcr_fx;
	Word16 ivad;
	Word16 snr_idx;
	Word16 noi_idx;
	Word16 bcgUpdate;
	Word16 mssnr_fx;
	Word16 dzcr_fx;
	Word16 alpha;
	Word32 L_temp;
	Word16 level_tmp;
	Word16 lev_tmp, thr;
	Word16 fgd_lev_fx_minus_0xE100;
	Word16 fgd_lev_fx_minus_0xEB00;	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 16);
#endif
	
	/* Increment frame counter */	
    st_fx->frm_cnt_fx = L_add(st_fx->frm_cnt_fx, 1);                                                                               
	
#ifdef DETAIL
    setCounter(zcrId); 
#endif
	zcr_fx=parametersVAD_zcr_fx(Input, nb);		
#ifdef DETAIL
    setCounter(dzcrId); 
#endif
    dzcr_fx=parametersVAD_dzcr_fx(zcr_fx, st_fx);	
#ifdef DETAIL
    setCounter(ssnrId); 
#endif
    mssnr_fx=parametersVAD_mssnr_fx(st_fx, SigFpw_fx, fft_exp_fx, nb);
#ifdef DETAIL
    setCounter(restId); 
#endif  
#ifdef WMOPS_FX
	move16();
#endif	
	*mssnr_ptr_fx = mssnr_fx;	
#ifdef WMOPS_FX
	move16();
#endif
	IF (sub(st_fx->flux_bgd_fx, 0x1C00)<0)
	{
		noi_idx = 0;
	}
	ELSE
	{
		noi_idx = 2;
		if (sub(st_fx->flux_bgd_fx, 0x3400)<0)
		{
			noi_idx = 1;
		}	
	}
	
    IF (nb) 
	{		
		IF (sub(st_fx->lsnr_fx, 0x2300)>0)
		{
#ifdef WMOPS_FX
			move16();
			move16();
#endif 
			snr_idx = 3;
			IF (sub(mssnr_fx, vad_thr_tbl_8k_fx[snr_idx][noi_idx][op])>0)
			{
				ivad = 1;
			}
			ELSE
			{
				ivad = 0;
			}
		}
		ELSE
		{
			IF (sub(st_fx->lsnr_fx, 0x1900)>0)
			{
#ifdef WMOPS_FX
				move16();
				move16();
				test();
#endif 
				snr_idx = 2;
				IF (sub(mssnr_fx,vad_thr_tbl_8k_fx[snr_idx][noi_idx][op])>0 || sub(mssnr_fx, add(mult_r(0x58E4, dzcr_fx), vad_thr_tbl_8k_fx[snr_idx][noi_idx][op]))>0)
				{
					ivad = 1; 
				}
				ELSE 
				{
					ivad = 0;
				}
			}
			ELSE
			{
				IF (sub(st_fx->lsnr_fx, 0x0F00)>0)
				{
#ifdef WMOPS_FX
					move16();
					move16();
					test();
#endif 
					snr_idx = 1;
					IF (sub(mssnr_fx, vad_thr_tbl_8k_fx[snr_idx][noi_idx][op])>0 || sub(mssnr_fx, add(mult_r(0x471C, dzcr_fx), vad_thr_tbl_8k_fx[snr_idx][noi_idx][op]))>0)
					{
						ivad = 1;
					}
					ELSE 
					{
						ivad = 0;
					}
				}
				ELSE
				{
#ifdef WMOPS_FX
					move16();
#endif 
					snr_idx = 0;					
					lev_tmp = round_fx(L_shl(L_mult(sub((Word16)0xEA00, st_fx->fgd_lev_fx), 0x0CCD), 5));					
					if (sub(lev_tmp, 0x2000) > 0)
					{
#ifdef WMOPS_FX
						move16();
#endif	
						lev_tmp = 0x2000;
					}
					if (sub(lev_tmp, negate(0x2000))<0)
					{
#ifdef WMOPS_FX
						move16();
#endif
						lev_tmp = negate(0x2000);
					}					
					thr = add(0x7FFF, mult_r(lev_tmp, 0x6666)); 
					thr = mult_r(thr, vad_thr_tbl_8k_fx[snr_idx][noi_idx][op]);
					
					IF (snr_idx==0)
					{
						IF (st_fx->fgd_lev_fx>-4096)
						{
							thr = shl(mult_r(vad_thr_tbl_8k_fx[snr_idx][noi_idx][op], 0x5666), 1);
						}
						ELSE
						{
							if (st_fx->fgd_lev_fx>-6656)
							{
					    		thr = mult_r(vad_thr_tbl_8k_fx[snr_idx][noi_idx][op], 0x4CCC);
							}
						}
					}
#ifdef WMOPS_FX
					move16();
					test();
#endif 					

					IF (sub(mssnr_fx, thr)>0 || sub(mssnr_fx, add(mult_r(0x3558, dzcr_fx), thr))>0)
                    {
						ivad = 1; 
					}
					ELSE 
					{
						ivad = 0;
					}
				}
			}
		}
	}
	ELSE 
	{
		IF (sub(st_fx->lsnr_fx, 0x2300)>0)
		{
#ifdef WMOPS_FX
					move16();
					move16();
#endif 	
			snr_idx = 3;
			IF (sub(mssnr_fx, vad_thr_tbl_16k_fx[snr_idx][noi_idx][op])>0)
			{
				ivad = 1;
			}
			ELSE
			{
				ivad = 0;
			}
		}
		ELSE
		{
#ifdef WMOPS_FX
					move16();
					move16();
#endif 
			IF (sub(st_fx->lsnr_fx, 0x1900)>0)
			{
				snr_idx = 2;
				IF (sub(mssnr_fx, vad_thr_tbl_16k_fx[snr_idx][noi_idx][op])>0 || sub(mssnr_fx, add(mult_r(0x58E4, dzcr_fx), vad_thr_tbl_16k_fx[snr_idx][noi_idx][op]))>0)
				{
					ivad = 1; 
				}
				ELSE 
				{
					ivad = 0;
				}
			}
			ELSE
			{
				IF (sub(st_fx->lsnr_fx, 0x0F00)>0)
				{
#ifdef WMOPS_FX
					move16();
					move16();
#endif 	
					snr_idx = 1;
					IF (sub(mssnr_fx, vad_thr_tbl_16k_fx[snr_idx][noi_idx][op])>0 || sub(mssnr_fx, add(mult_r(0x471C, dzcr_fx), vad_thr_tbl_16k_fx[snr_idx][noi_idx][op]))>0) 
					{
						ivad = 1;
					}
					ELSE 
					{
						ivad = 0;
					}
				}
				ELSE
				{
#ifdef WMOPS_FX
					move16();
#endif 		
					snr_idx = 0;
					lev_tmp = round_fx(L_shl(L_mult(sub((Word16)0xEA00, st_fx->fgd_lev_fx), 3277), 5));
					
					if (sub(lev_tmp, 0x2000)>0)
					{
#ifdef WMOPS_FX
						move16();
#endif
						lev_tmp = 0x2000;
					}
					if (sub(lev_tmp, (Word16)0xE000)<0)
					{
#ifdef WMOPS_FX
						move16();
#endif
						lev_tmp = (Word16)0xE000;
					}
					thr = add(0x7FFF, mult_r(lev_tmp, 0x6666)); 
					thr = mult_r(thr, vad_thr_tbl_16k_fx[snr_idx][noi_idx][op]);

					if (snr_idx==0)
					{
						if (st_fx->fgd_lev_fx>-4096)
						{
							thr = shl(mult_r(vad_thr_tbl_16k_fx[snr_idx][noi_idx][op], 0x4CCD), 1); 
						}
						ELSE
						{
							if (st_fx->fgd_lev_fx>-6656)
							{
						    	thr = mult_r(vad_thr_tbl_16k_fx[snr_idx][noi_idx][op], 0x6666);
							}
						}
					}
#ifdef WMOPS_FX
					test();
					move16();
#endif 
					IF (sub(mssnr_fx, thr)>0 || sub(mssnr_fx, add(mult_r(0x3558, dzcr_fx), thr))>0)
					{
						ivad = 1;
					}
					ELSE 
					{
						ivad = 0;
					}
				}
			}
		}
	}
	
    /* VAD hangover */
    st_fx->bgng_cnt_fx = sub(st_fx->bgng_cnt_fx, 1);
	
    IF (ivad==0)
	{
#ifdef WMOPS_FX
		test();
		test();
		test();
#endif
		IF (st_fx->hang_sp_fx>0 || st_fx->bgng_cnt_fx>0 || st_fx->hang_f_mus_fx>0 || st_fx->hang_b_mus_fx>0)
		{
#ifdef WMOPS_FX
			move16();
#endif		
			ivad = 1; 
			st_fx->hang_sp_fx = sub(st_fx->hang_sp_fx, 1);
			st_fx->hang_f_mus_fx = sub(st_fx->hang_f_mus_fx, 1);
			st_fx->hang_b_mus_fx = sub(st_fx->hang_b_mus_fx, 1);                                          			
		}
		
		if (st_fx->hang_sp_fx<0)
		{
#ifdef WMOPS_FX
			move16();
#endif
			st_fx->hang_sp_fx = 0;                                            
		}                                                                  
		if (st_fx->bgng_cnt_fx<0)                                                 
		{
#ifdef WMOPS_FX
			move16();
#endif
			st_fx->bgng_cnt_fx = 0;                                            
		}
		if (st_fx->hang_f_mus_fx<0)
		{
#ifdef WMOPS_FX
			move16();
#endif
			st_fx->hang_f_mus_fx = 0;
		}
		if (st_fx->hang_b_mus_fx<0)
		{
#ifdef WMOPS_FX
			move16();
#endif
			st_fx->hang_b_mus_fx = 0;
		}
	}
	
    IF (sub(st_fx->burst_cnt1_fx, 5)>0) 
	{ 
		st_fx->hang_sp_fx = hang_sp_tbl_fx[op][snr_idx][1];	
	}
	
    /* update Input burst counter */	
    IF (sub(mssnr_fx, burst_thr_tbl_fx[op][snr_idx][0])>0)
	{
		st_fx->burst_cnt1_fx = add(st_fx->burst_cnt1_fx, 1);                                              
	}
    ELSE
	{
#ifdef WMOPS_FX
		move16();
#endif	
		st_fx->burst_cnt1_fx = 0;                                            
	}
	
#ifdef DETAIL
    setCounter(spdevId); 
#endif
    parametersVAD_FQ_Stability_fx(st_fx);
#ifdef DETAIL
    setCounter(TonalityId); 
#endif
	parametersVAD_Tonality_fx(st_fx, SigFpw_fx, nb);
	
#ifdef DETAIL
    setCounter(bcgUpdtId); 
#endif	
	bcgUpdate = parametersVAD_bcgUpdate_fx(st_fx, mssnr_fx, SigFpw_fx, nb);
#ifdef DETAIL
    setCounter(lsnrId); 
#endif
    parametersVAD_lsnr_fx(st_fx,bcgUpdate);
		
	/* Detect the presence of music background */
#ifdef DETAIL
    setCounter(MusBcgdId); 
#endif
	parametersVAD_MusicBcgd_fx(st_fx, SigFpw_fx, bcgUpdate);
#ifdef DETAIL
    setCounter(restId); 
#endif 
	
	/* Update the noise estimates */           	
	IF (bcgUpdate)
    {      	 		                                                     	  
		
#ifdef WMOPS_FX
		move16();
#endif
		alpha = 0x799A ;
		IF(st_fx->frm_cnt_fx>4)
		{       		                                                               
			/* Update the noise estimates for dzcr */
			L_temp = L_shl(L_mult(negate(add((Word16)0x8000, alpha)), zcr_fx), 7);
			st_fx->MeanZC_fx = round_fx(L_mac(L_temp, alpha, st_fx->MeanZC_fx));        
					
			/* Update the noise estimates for ssnr */
			tmp = negate(add((Word16)0x8000, 0x0CCD)); 
			fgd_lev_fx_minus_0xE100 = sub(st_fx->fgd_lev_fx, (Word16)0xE100);
			fgd_lev_fx_minus_0xEB00 = sub(st_fx->fgd_lev_fx, (Word16)0xEB00);
			FOR (i=LO_BAND; i<=HI_BAND; i++)
			{
				L_temp= L_mls(st_fx->band_enrg_bgd_fx[i], tmp);
				st_fx->band_enrg_bgd_fx[i] = L_add(L_temp, L_mls(st_fx->band_enrg_fx[i], 0x0CCD));              
				
				IF (fgd_lev_fx_minus_0xE100<0)
				{				
					if (L_sub(st_fx->band_enrg_bgd_fx [i], 30)<0) 
					{
#ifdef WMOPS_FX
						move32();
#endif	
						st_fx->band_enrg_bgd_fx [i] = 30;                           
					}				
				}
				ELSE
				{
					IF (fgd_lev_fx_minus_0xEB00<0)
					{
						if (L_sub(st_fx->band_enrg_bgd_fx[i], 100)<0)
						{
#ifdef WMOPS_FX
							move32();
#endif			
							st_fx->band_enrg_bgd_fx[i] = 100;                         
						}
					}
					ELSE
					{				
						if (L_sub(st_fx->band_enrg_bgd_fx[i], 150)<0) 
						{
#ifdef WMOPS_FX
							move32();
#endif	
							st_fx->band_enrg_bgd_fx[i] = 150;                          
						}				
					}
				}
			}                                                          
		}
	}
	ELSE
	{			
		tmp = negate(add((Word16)0x8000, 0x0148));
		FOR (i=0; i<NUM_BAND; i++)
		{
#ifdef WMOPS_FX
			move32();
#endif
			IF (L_sub(st_fx->band_enrg_fx[i], st_fx->band_enrg_bgd_fx[i])<0)
			{          				                                     
				L_temp = L_mls(st_fx->band_enrg_bgd_fx[i], tmp);
				st_fx->band_enrg_bgd_fx[i] = L_add(L_temp, L_mls(st_fx->band_enrg_fx[i], 0x0148));
			}
		}
	}
#ifdef DETAIL
    setCounter(bcgStaId); 
#endif	
	parametersVAD_background_flux_fx(st_fx, mssnr_fx, bcgUpdate);                              	
#ifdef DETAIL
    setCounter(restId); 
#endif 		
	level_tmp = log10_Q15(st_fx->rms_Input_fx);
	
#ifdef LC	
	if (ivad==1) 
	{
		ivad = 3;
	}
#endif	
#ifdef DRAM
    accDRAM(FRAME_COUNT, -16);
#endif
    return (ivad);	
}

