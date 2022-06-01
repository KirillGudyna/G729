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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stl.h"
#include "vad_fx.h"
#include "parameters_fx.h"
#include "gsad_math_adv_fx.h"
#include "constdef_fx.h"
#include "Rom_fx.h"


/**********************************************************************
* FUNCTION     : parametersVAD_MusicBcgd_fx ()                        *
*---------------------------------------------------------------------*
* PURPOSE      : detect the presence of music background              *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)  SigFpw_fx : pointer to the spectrum power density of  *
*                           the input frame                           *
*   (Word16)    bcgUpdate : activity indication of the input frame    *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st_fx    : the vad state                             *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/
void parametersVAD_MusicBcgd_fx(vadState_fx *st_fx, 
								Word16 *SigFpw_fx, 
								Word16 bcgUpdate)
{
	Word16 tmp;	

	IF (sub(bcgUpdate, 1)==0)
	{
		st_fx->bgd_frm_cnt_fx = add(st_fx->bgd_frm_cnt_fx, 1);
		st_fx->sum_SP_fx = add(st_fx->sum_SP_fx, shr(st_fx->SP_fx, 9));
	}
	IF (sub(st_fx->bgd_frm_cnt_fx, 100) > 0)
	{			
		if (st_fx->hang_b_mus_fx > 0)
		{
			tmp = 1100;
		}
		else
		{
			tmp = 1200;
		}

		if (sub(st_fx->sum_SP_fx, tmp)>0)			
		{
			st_fx->hang_b_mus_fx = 1000;			
		}
		
		if (sub(st_fx->sum_SP_fx, 400)<0) 
		{
			st_fx->hang_b_mus_fx = 0;
		}		

		st_fx->sum_SP_fx = 0;
		st_fx->bgd_frm_cnt_fx = 0;
	}
}


/**********************************************************************
* FUNCTION     : parametersVAD_bcgUpdate_fx ()                        *
*---------------------------------------------------------------------*
* PURPOSE      : decide whether to update noise estimate or not       *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16)     mssnr_fx    : modified SSNR                          *
*   (Word16 *)   SigFpw_fx   : pointer to the spectrum power density  *
*                              of the input frame                     *
*   (Word16)     nb          : sampling rate flag                     *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st_fx       : the vad state                          *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word16)     update_flg : background estimate update flag         *
**********************************************************************/ 
Word16 parametersVAD_bcgUpdate_fx(vadState_fx *st_fx, 
								  Word16 mssnr_fx, 
								  Word16 *SigFpw_fx, 
								  Word16 nb)
{
	Word16 update_flg=0;                                                        
    Word16 tmp;
	Word16 i;
	Word16 reset_flg=0;
	Word32 L_temp;
	Word32 frm_cnt_fx_minus_four;
	Word16 tmp1;
    Word16 T_snr_fx;
	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 10);
#endif
#ifdef WMOPS_FX
	move16();
	move16();
	move16();
#endif	
	
	tmp = 3;
	if (sub(st_fx->lsnr_fx, 3840)>0)
	{
#ifdef WMOPS_FX
		move16();
#endif
		tmp = 1;                                                            
	}
	
	st_fx->burst_cnt_fx = add(st_fx->burst_cnt_fx, 1);                                                                        
    if (sub(mssnr_fx, 1600)<=0)
	{
#ifdef WMOPS_FX
		move16();
#endif
		st_fx->burst_cnt_fx = 0;                                                    
	}

#ifdef WMOPS_FX
	test();
#endif		
	IF (st_fx->lsnr_fx>4608 && st_fx->hang_b_mus_fx>0)
	{
#ifdef WMOPS_FX
	move16();
#endif	
		T_snr_fx = 1280;
		if (nb)
		{
#ifdef WMOPS_FX
	move16();
#endif	
			T_snr_fx = 1100;
		}
	}
	ELSE
	{
#ifdef WMOPS_FX
	move16();
#endif	
		T_snr_fx = 480;
	}
		
	IF (sub(mssnr_fx, T_snr_fx)<=0)
	{
		st_fx->bgd_update_cnt_fx = add(st_fx->bgd_update_cnt_fx, 1);                                                
	}
	ELSE
	{
		if (sub(st_fx->bgd_update_cnt_fx, tmp)>=0)
		{
#ifdef WMOPS_FX
			move16();
#endif  	
			st_fx->bgd_update_cnt_fx = tmp;                             
		}
	}  
	
	IF (st_fx->bgd_update_cnt_fx!=0)
	{                                                                       
		if (sub(st_fx->burst_cnt_fx, 10)>=0)
		{
#ifdef WMOPS_FX
			move16();
#endif 
			st_fx->bgd_update_cnt_fx = 0;                                          
		}
	}	
	
#ifdef WMOPS_FX
	test();
	test();
	test();
	test();
#endif
	IF ((sub(st_fx->bgd_update_cnt_fx, tmp)>0 && st_fx->dtmf_flg==0 && st_fx->hang_f_mus_fx<=0) || (L_sub(st_fx->frm_cnt_fx,4)<=0 && st_fx->dtmf_flg==0)) 
	{
#ifdef WMOPS_FX
		move16();
		move16();
#endif
		update_flg = 1;	                                                
		reset_flg = 1;
	}
	ELSE
	{
#ifdef WMOPS_FX
		test();
#endif
		IF (st_fx->dtmf_flg==0 && st_fx->hang_f_mus_fx<=0)
		{
			st_fx->con_frm_cnt_fx = add(st_fx->con_frm_cnt_fx, 1);			
#ifdef WMOPS_FX
		move16();
#endif
			tmp1 = 6500;
			if (nb)
			{
#ifdef WMOPS_FX
		move16();
#endif
				tmp1 = 7680;
			}
			if (sub(st_fx->SP_fx, tmp1)<0)
			{
				st_fx->low_SP_cnt_fx = add(st_fx->low_SP_cnt_fx, 1);
			}
			
#ifdef WMOPS_FX
		move16();
#endif
			tmp1 = 10;
			if (nb)
			{
#ifdef WMOPS_FX
		move16();
#endif
				tmp1 = 12;
			}
			if (L_sub(st_fx->fq_stb_fx, tmp1)<0)
			{
				st_fx->high_fst_cnt_fx = add(st_fx->high_fst_cnt_fx, 1);
			}
			
			FOR (i=0; i<16; i++)
			{
				if (L_sub(st_fx->band_enrg_fx[i], st_fx->min_band_enrg_fx[i])<0)
				{
#ifdef WMOPS_FX
					move32();
#endif	
					st_fx->min_band_enrg_fx[i] = st_fx->band_enrg_fx[i];
				}
			}
					
			IF (st_fx->con_frm_cnt_fx==30)
			{		
				IF (sub(st_fx->low_SP_cnt_fx, 15)>0)
				{
#ifdef WMOPS_FX
					test();
					test();
					test();
#endif					
				    IF (sub(st_fx->high_fst_cnt_fx,28)>0 && L_sub(st_fx->fq_stb_fx,12)<0 && sub(st_fx->peak_flux_cnt_fx,15)>0 && sub(st_fx->tone_stb_cnt_fx,12)<0)
					{
#ifdef WMOPS_FX
						move16();
#endif			
						update_flg = 1;
					}
					ELSE
					{ 
#ifdef WMOPS_FX
						test();
#endif
						IF (sub(st_fx->peak_flux_cnt_fx,15)>0 && sub(st_fx->tone_stb_cnt_fx,12)<0)
						{					
							tmp = negate(add((Word16)0x8000, (Word16)0x7999));							
							FOR (i=0; i<16; i++)
							{
#ifdef WMOPS_FX
								move32();
#endif
								L_temp = L_mls(st_fx->min_band_enrg_fx[i], tmp);
								st_fx->band_enrg_bgd_fx[i] = L_add(L_temp, L_mls(st_fx->band_enrg_bgd_fx[i], 0x7999));
								st_fx->min_band_enrg_fx[i] = 10000;	
							}
#ifdef WMOPS_FX
						move16();
#endif
							reset_flg = 1;
						}
						ELSE
						{
#ifdef WMOPS_FX
						move16();
#endif
							reset_flg = 1;
						}
					}
				}
				ELSE
				{
#ifdef WMOPS_FX
					move16();
#endif		
					reset_flg = 1;
				}
#ifdef WMOPS_FX
				test();
#endif
				IF (sub(st_fx->high_fst_cnt_fx,28)>0 && sub(st_fx->low_SP_cnt_fx,5)<0)
				{
#ifdef WMOPS_FX
					move16();
#endif
					st_fx->hang_f_mus_fx = 10;
				}
			}
			ELSE
			{
				IF (sub(st_fx->con_frm_cnt_fx, 30)>0)
				{
					IF (L_sub(st_fx->fq_stb_fx, 12)<0)
					{
#ifdef WMOPS_FX
						move16();
#endif
						update_flg = 1;
					}
					ELSE
					{
#ifdef WMOPS_FX
						move16();
#endif
						reset_flg = 1;
					}
				}	
			}
			
		}
		ELSE
		{
#ifdef WMOPS_FX
			move16();
#endif
			reset_flg = 1;
		}
	}

    if (reset_flg)
	{
#ifdef WMOPS_FX
		move16();
		move16();
		move16();
		move16();
		move16();
#endif
		st_fx->con_frm_cnt_fx = 0;
		st_fx->tone_stb_cnt_fx = 0;
		st_fx->high_fst_cnt_fx = 0;
		st_fx->peak_flux_cnt_fx = 0;
		st_fx->low_SP_cnt_fx = 0;
	}
	
	frm_cnt_fx_minus_four = L_sub(st_fx->frm_cnt_fx, 4);                                                                       
	IF (frm_cnt_fx_minus_four<=0)
	{                                                                       
		if (sub(st_fx->rms_Input_fx, 2000)>0)
		{
			st_fx->bgd_init_reset_cnt_fx = add(st_fx->bgd_init_reset_cnt_fx, 1);                                            
		}                                                                   
		IF (frm_cnt_fx_minus_four==0)
		{                                                                   
			if (sub(st_fx->bgd_init_reset_cnt_fx, 3)>=0)
			{                                                               
#ifdef WMOPS_FX
				move16();
				move16();
#endif
				FOR (i=0; i<16; i++)
				{
#ifdef WMOPS_FX
					move32();
#endif
					st_fx->band_enrg_bgd_fx[i] = MIN_BAND_ENRG;                       
				}
				st_fx->rms_bgd_fx = 160;                                    
				st_fx->bgd_lev_fx = (Word16)0xBA00;
			}                                  
		}
	}		
#ifdef DRAM
	accDRAM(FRAME_COUNT, -10);
#endif   
	return (update_flg);
}


/**********************************************************************
* FUNCTION     : parametersVAD_Tonality_fx ()                         *
*---------------------------------------------------------------------*
* PURPOSE      : Determine the tonality of the input frame            *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)   SigFpw_fx : pointer to the spectrum power density of *
*                            the input frame                          *
*   (Word16)     nb        : sampling rate flag                       * 
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st_fx     : the vad state                            *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/
void parametersVAD_Tonality_fx(vadState_fx *st_fx, 
							   Word16 *SigFpw_fx, 
							   Word16 nb)
{
	Word16 i,j,k,m;
	Word16 max_t[3],max_n[3],tmp1[3],tmp2[3],peak[48],peak_idx[48],max_peak_idx[3];
	Word16 vl,vr,avg;
	Word16 tmp,tmp3,tmp4,idx_peak_glb,sum_tmp;
	Word32 avg_sum,avg_sum1;
    Word16 exp_num,exp_den,norm_num,norm_den,exp_tmp;
	Word16 tonl_sum_three;
	Word16 *ptr_minus,*ptr_plus;
	Word16 cond1,cond2;
	Word32 avg_sum_minus_three;
	Word16 avg_minus_three;
	Word16 avg_rec;
	Word16 tmp_dtmf;
	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 142);
#endif	
#ifdef WMOPS_FX
	move16();	move16();
	move16();	move16();
	move16();	move16();
	move16();	move16();
	move16();	move16();
	move16();	move16();
	move16();	move16();
	move16();	move16();
#endif
	max_n[0]=max_n[1]=max_n[2]=0;
	max_t[0]=max_t[1]=max_t[2]=0;
	tmp2[0]=tmp2[1]=tmp2[2]=0;
	max_peak_idx[0]=max_peak_idx[1]=max_peak_idx[2]=0;
    tmp3 = 0; 
	idx_peak_glb = 0; 
	avg_sum = 0;
    k = 0;                                                               
#ifdef WMOPS_FX
	move32();
	move32();
#endif
	ptr_minus = &SigFpw_fx[3];
	ptr_plus = &SigFpw_fx[5];

    FOR (i=4; i<52; i++)
	{
#ifdef WMOPS_FX
		test();
#endif
		cond1 = sub(SigFpw_fx[i], *ptr_minus++);
		cond2 = sub(SigFpw_fx[i], *ptr_plus++);
		IF (cond1>0 && cond2>0)
		{
#ifdef WMOPS_FX
			move16();
			move16();
#endif
			peak[k] = SigFpw_fx[i];                                      
			peak_idx[k] = i;                                           
			k = add(k, 1);                                                       
		}
	}                                                                  

	FOR (i=0; i<k; i++) 
	{
		if (sub(peak[i], tmp3)>0) 
		{
#ifdef WMOPS_FX
			move16();
			move16();
#endif
			tmp3 = peak[i];
			idx_peak_glb = peak_idx[i];
		}
	}

	FOR (i=0; i<k; i++)
	{     
		tmp = sub(shl(SigFpw_fx[peak_idx[i]], 2), SigFpw_fx[sub(peak_idx[i], 1)]);
		sum_tmp = add(SigFpw_fx[add(peak_idx[i], 1)], SigFpw_fx[sub(peak_idx[i], 2)]);
		tmp = sub(tmp, sum_tmp);
		tmp = sub(tmp, SigFpw_fx[add(peak_idx[i], 2)]);
		
		IF (sub(tmp, tmp2[0])>0)
		{
#ifdef WMOPS_FX
			move16();
			move16();
			move16();
			move16();
			move16();
			move16();
#endif	
			tmp2[2] = tmp2[1];                                         
			tmp2[1] = tmp2[0];
			tmp2[0] = tmp;
			max_peak_idx[2] = max_peak_idx[1];
			max_peak_idx[1] = max_peak_idx[0];
			max_peak_idx[0] = peak_idx[i];
		}
		ELSE
		{
			IF (sub(tmp, tmp2[1])>0)
			{
#ifdef WMOPS_FX
				move16();
				move16();
				move16();
				move16();
#endif	
				tmp2[2] = tmp2[1];                                         
				tmp2[1] = tmp;
				max_peak_idx[2] = max_peak_idx[1];
				max_peak_idx[1] = peak_idx[i];
			}
			ELSE
			{
				if (sub(tmp, tmp2[2])>0)
				{
#ifdef WMOPS_FX
					move16();
					move16();
#endif	
					tmp2[2] = tmp;                                             
					max_peak_idx[2] = peak_idx[i];
				}
			}
		}
		IF (sub(peak_idx[i], 24)>0)
		{
			IF (sub(max_n[0], tmp)<0)
			{
#ifdef WMOPS_FX
				move16();
				move16();
#endif
				max_n[1] = max_n[0];
				max_n[0] = tmp;
			}
			ELSE
			{
				if (sub(max_n[1], tmp)<0)
				{
#ifdef WMOPS_FX
					move16();
#endif
					max_n[1] = tmp;
				}
			}
		}
	}
#ifdef WMOPS_FX
	move16();
#endif
	tmp1[0] = abs_s(sub(max_peak_idx[0], st_fx->max_peak_idx_old_fx));             
	tmp1[1] = abs_s(sub(max_peak_idx[1], st_fx->max_peak_idx_old_fx));
	tmp1[2] = abs_s(sub(max_peak_idx[2], st_fx->max_peak_idx_old_fx));
	st_fx->max_peak_idx_old_fx = max_peak_idx[0];                            
#ifdef WMOPS_FX
	test();
#endif	                                                                   
	IF (sub(tmp1[1],tmp1[0])<0 && sub(tmp1[1],tmp1[2])<0)
	{
#ifdef WMOPS_FX
		move16();
#endif
		tmp = tmp1[1];                                                 
	}
	ELSE
	{
#ifdef WMOPS_FX
		test();
		move16();
#endif
		IF (sub(tmp1[2],tmp1[0])<0 && sub(tmp1[2],tmp1[1])<0)
		{
			tmp = tmp1[2];                                                 
		}
		ELSE
		{
			tmp = tmp1[0];                                                 
		}
	}
	
	if (sub(tmp, 1)<=0)
	{		
		st_fx->tone_stb_cnt_fx = add(st_fx->tone_stb_cnt_fx, 1);		
	}
	if (sub(idx_peak_glb, st_fx->idx_peak_glb_old_fx)!=0)
	{
		st_fx->peak_flux_cnt_fx = add(st_fx->peak_flux_cnt_fx, 1);
	}
#ifdef WMOPS_FX
	move16();
#endif
	st_fx->idx_peak_glb_old_fx = idx_peak_glb;
		
	FOR (i=2; i<64; i++)
	{
		avg_sum = L_mac0(avg_sum, SigFpw_fx[i], 0x0001);
	}
	avg_sum1 = L_shl(avg_sum, 10);
	avg = round_fx(L_shl(L_mls(avg_sum1, 16913), 2));
	
#ifdef WMOPS_FX
	move16();
#endif
	st_fx->SP_fx = 0;
	IF(avg != 0)
	{
		FOR (i=0; i<k; i++)
		{
			IF (peak_idx[i] > 10)
			{
				FOR (j=0; j<5; j++)
				{
#ifdef WMOPS_FX
					move16();
#endif
					m = sub(peak_idx[i], j);
					vl = SigFpw_fx[m];
					IF (sub(SigFpw_fx[m], SigFpw_fx[sub(m, 1)])<=0)
					{
#ifdef WMOPS_FX
						move16();
#endif
						vl = SigFpw_fx[m];
						BREAK;
					}
				}
				FOR (j=0; j<5; j++)
				{
#ifdef WMOPS_FX
					move16();
#endif
					m = add(peak_idx[i], j);
					vr = SigFpw_fx[m];
					IF (sub(SigFpw_fx[m], SigFpw_fx[add(m, 1)])<=0)
					{
#ifdef WMOPS_FX
						move16();
#endif
						vr = SigFpw_fx[m];
						BREAK;
					}
				}
				tmp4 = sub(shl(peak[i], 1), vl);
				tmp4 = sub(tmp4, vr);
				
				IF (sub(tmp4, max_t[0]) > 0)
				{
#ifdef WMOPS_FX
					move16();
					move16();
					move16();
#endif
					max_t[2] = max_t[1];
					max_t[1] = max_t[0];
					max_t[0] = tmp4;
				}
				ELSE
				{
					IF (sub(tmp4, max_t[1])>0)
					{
#ifdef WMOPS_FX
						move16();
						move16();
#endif
						max_t[2] = max_t[1];
						max_t[1] = tmp4;
					}
					ELSE 
					{
						if (sub(tmp4, max_t[2])>0)
						{
#ifdef WMOPS_FX
							move16();
#endif					
							max_t[2] = tmp4;
						}
					}
				}
				
			}
		}
		tonl_sum_three = add(add(max_t[0], max_t[1]), max_t[2]);
		
		exp_num = norm_s(tonl_sum_three);
		norm_num = shl(tonl_sum_three, exp_num);
		
		exp_den= norm_s(avg);
		norm_den = shl(avg, exp_den);
		IF(sub(norm_num, norm_den)>0)
		{
			norm_num = shr(norm_num, 1);
			exp_num = sub(exp_num, 1);
			exp_tmp = sub(exp_num, exp_den);
			tmp4 = shr(div_s(norm_num, norm_den), add(5, exp_tmp));
		}
		ELSE
		{
			exp_tmp = sub(exp_num, exp_den);
			tmp4 = shr(div_s(norm_num, norm_den), add(5, exp_tmp));
		}
#ifdef WMOPS_FX
		move16();
#endif
		st_fx->SP_fx = tmp4;
	}
	
	avg_sum_minus_three = L_msu0(avg_sum, tonl_sum_three, 0x0001);
    avg_sum_minus_three = L_shl(avg_sum_minus_three, 10);
	avg_minus_three = round_fx(L_shl(L_mls(avg_sum_minus_three, 17772), 2));
    if (avg_minus_three==0)
	{
#ifdef WMOPS_FX
		move16();
#endif
		avg_minus_three = 1;
    }
	IF (avg_minus_three==1)
	{
#ifdef WMOPS_FX
		move16();
#endif
		avg_rec = 1;
	}
	ELSE
	{
        avg_rec = shr(div_s(1, avg_minus_three), 7);
	}
#ifdef WMOPS_FX
	move16();
	move16();
	move16();
#endif    
	max_t[0] = round_fx(L_shl(L_mult(max_t[0], avg_rec), 8));
	max_t[1] = round_fx(L_shl(L_mult(max_t[1], avg_rec), 8));
	max_t[2] = round_fx(L_shl(L_mult(max_t[2], avg_rec), 8));
	
#ifdef WMOPS_FX
		move16();
#endif
	tmp = 25;
	if (nb)
	{
#ifdef WMOPS_FX
		move16();
#endif
		tmp = 30;
	}
	IF (sub(max_t[0], tmp)>0)
	{
#ifdef WMOPS_FX
		move16();
#endif
		st_fx->dtmf_flg = 1;
	}
	ELSE
	{
#ifdef WMOPS_FX
		move16();
#endif
		tmp = 35;
		if (nb)
		{
#ifdef WMOPS_FX
		move16();
#endif
			tmp = 42;
		}
		IF (sub(add(max_t[0], max_t[1]), tmp)>0)
		{
#ifdef WMOPS_FX
			move16();
#endif
			st_fx->dtmf_flg = 1;
		}
		ELSE
		{
#ifdef WMOPS_FX
			move16();
#endif
			tmp_dtmf = add(add(max_t[0], max_t[1]), max_t[2]);
#ifdef WMOPS_FX
		move16();
#endif
			tmp = 42;
			if (nb)
			{
#ifdef WMOPS_FX
		move16();
#endif
				tmp = 50;
			}
			IF (sub(tmp_dtmf, tmp)>0)
			{
				st_fx->dtmf_flg = 1;
			}
			ELSE
			{
				st_fx->dtmf_flg = 0;
			}
		}
	}	
#ifdef DRAM
    accDRAM(FRAME_COUNT, -142);
#endif
}


/**********************************************************************
* FUNCTION     : parametersVAD_zcr_fx ()                              *
*---------------------------------------------------------------------*
* PURPOSE      : calculates the zero crossing rate of the input frame *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)   Input   : pointer to the input frame samples         *
*   (Word16)     nb      : sampling rate flag                         *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*                None                                                 *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word16)     zcr_fx  : calculated zero crossing rate              *
**********************************************************************/
Word16  parametersVAD_zcr_fx(Word16 *Input, Word16 nb)
{
    Word16 i,tmp;
    Word16 zcr_fx;
	Word16 Frame_len;
	Word32 L_tmp;
	Word16 *input_ptr;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 7);
#endif
#ifdef WMOPS_FX
	move16();
    move16();
#endif	
	
	Frame_len = SAMPLE16k;
	if (nb)
	{
#ifdef WMOPS_FX
		move16();
#endif
		Frame_len = SAMPLE8k;
	}
	
	zcr_fx = 0;
	tmp=sub(Frame_len, 1);
#ifdef WMOPS_FX
	move32();
#endif
	input_ptr = &Input[1];

    FOR (i=0; i<tmp; i++)
    {                                                          
		L_tmp = L_mult0(Input[i], *input_ptr++); 
		if (L_tmp<0)
		{
			zcr_fx = add(zcr_fx, 1);                                        
		}
	}
#ifdef DRAM
    accDRAM(FRAME_COUNT, -7);
#endif
	return (zcr_fx);
}


/**********************************************************************
* FUNCTION     : parametersVAD_dzcr_fx ()                             *
*---------------------------------------------------------------------*
* PURPOSE      : calculates the delta zero crossing rate of the       *
*                input frame                                          *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16)     zcr_fx   : zero crossing rate of the input frame     *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState_fx *) st_fx : the vad state                             *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word16)    dzcr_fx   : calculated delta zero crossing rate       *
**********************************************************************/
Word16 parametersVAD_dzcr_fx(Word16 zcr_fx, vadState_fx *st_fx)
{
	Word16 dzcr_fx;
	Word32 L_temp;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 3);
#endif	
	SWITCH (st_fx->frm_cnt_fx)
	{		
        case 1: st_fx->MeanZC_fx = shl(zcr_fx, 7); BREAK;
        case 2: st_fx->MeanZC_fx = add(shr(st_fx->MeanZC_fx, 1), shl(zcr_fx, 6)); BREAK;
        case 3: L_temp = L_shl(L_mult(zcr_fx, 0x2AAA), 7);			
			st_fx->MeanZC_fx = round_fx(L_add(L_mult(st_fx->MeanZC_fx, 0x5555), L_temp)); BREAK;
        case 4: st_fx->MeanZC_fx = add(mult_r(st_fx->MeanZC_fx, 24576), shl(zcr_fx, 5)); BREAK;			
	}
	
	dzcr_fx = sub(shl(zcr_fx, 7), st_fx->MeanZC_fx);                                        
#ifdef DRAM
    accDRAM(FRAME_COUNT, -3);
#endif	
	return (dzcr_fx);	
}


/**********************************************************************
* FUNCTION      :  parametersCommon_rms_fx ()                         *
*---------------------------------------------------------------------*
* PURPOSE       :  computes the root mean square of the input samples *
*                  per frame                                          *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)   Input   : pointer to the frame of speech             *
*   (Word16)     nb      : sampling rate flag                         *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st_fx   :  the vad state                             *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/
void parametersCommon_rms_fx(Word16 *Input, 
							 Word16 nb, 
							 vadState_fx *st_fx)
{
	Word16 i;
	Word16 tmp;
	Word16 exp; 
	Word16 s_16;
	Word32 s;
	Word32 s_norm;
#ifdef WMOPS_FX
	move32();
	move16();
#endif 
	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 7);
#endif
	s = 0;
	IF (nb) 
	{                                                                
		FOR (i=0; i<SAMPLE8k; i++)
		{
			tmp = shr(Input[i], 2);
            s = L_mac0(s, tmp, tmp);
		}
		exp = norm_l(s);
		s_16 = round_fx(L_shl(s, exp));
		s_norm = L_mult(Sqrt_Q0(s_16), Sqrt_exp_factor_32bit_fx[exp]);
        st_fx->rms_Input_fx = round_fx(L_shl(L_mls(s_norm, 14654), 8));
	}
	ELSE
	{                                                   
		FOR (i=0; i<SAMPLE16k; i++) 
		{
			tmp = shr(Input[i], 2);
			s = L_mac0(s, tmp, tmp);                 
		}
		exp = norm_l(s);
		s_16 = round_fx(L_shl(s, exp));
		s_norm = L_mult(Sqrt_Q0(s_16), Sqrt_exp_factor_32bit_fx[exp]);
        st_fx->rms_Input_fx = round_fx(L_shl(L_mls(s_norm, 10362), 8));		
	}
	if (st_fx->rms_Input_fx==0) 
	{
		st_fx->rms_Input_fx = 1;
	}
#ifdef DRAM
    accDRAM(FRAME_COUNT, -7);
#endif	
}


/**********************************************************************
* FUNCTION     : parametersVAD_lsnr_fx ()                             *
*---------------------------------------------------------------------*
* PURPOSE      : calculates the long term SNR of the input signal     *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16)     bcgUpdate : noise estimate update flag               *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS   :                                          *
*   (vadState *) st_fx     : the vad state                            *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/  
void parametersVAD_lsnr_fx(vadState_fx *st_fx, Word16 bcgUpdate)
{	
	Word16 beta;
	Word32 L_temp;
	Word32 L_tmp;
	Word32 L_tmp1;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 5);
#endif
	
	IF (bcgUpdate)
	{	
		SWITCH (st_fx->frm_cnt_fx)
		{
		case 1: st_fx->rms_bgd_fx = shl(st_fx->rms_Input_fx, 4); BREAK;
		case 2: st_fx->rms_bgd_fx  = add(shr(st_fx->rms_bgd_fx, 1), shl(st_fx->rms_Input_fx, 3)); BREAK;
		case 3: L_temp=L_shl(L_mult(st_fx->rms_Input_fx,0x2AAA),4);
			st_fx->rms_bgd_fx = round_fx(L_add(L_mult(st_fx->rms_bgd_fx, 0x5555), L_temp));  BREAK;
		case 4: L_temp=L_shl(L_mult(st_fx->rms_Input_fx, 0x2000), 4);
			st_fx->rms_bgd_fx = round_fx(L_add(L_mult(st_fx->rms_bgd_fx, 0x5FFF), L_temp));  BREAK;
		}
		
		IF (L_sub(st_fx->frm_cnt_fx, 4L)>0)
		{ 
			L_tmp = L_sub(L_deposit_h(st_fx->rms_Input_fx), L_mult(st_fx->rms_bgd_fx, 2893));
			L_tmp1 = L_sub(L_deposit_h(st_fx->rms_bgd_fx), L_shl(L_mult(st_fx->rms_Input_fx, 0x5A66), 5));						
#ifdef WMOPS_FX
			test();
			move16();
#endif
			IF (L_tmp>0 || L_tmp1>0)
			{
				beta = 0x799A;
			}
			ELSE
			{
				beta = 0x7EB8;
			}

			L_temp = L_shl(L_mult(negate(add((Word16)0x8000, beta)), st_fx->rms_Input_fx), 4);
			st_fx->rms_bgd_fx = round_fx(L_add(L_mult(beta, st_fx->rms_bgd_fx), L_temp));
		}		
		L_temp = L_mult(sub(log10_Q15(st_fx->rms_bgd_fx), 0x1344), 0x2800);
		st_fx->bgd_lev_fx = round_fx(L_shl(L_temp, 2));
	}
	ELSE
	{
        beta = 0x7FFF;
		IF (st_fx->burst_cnt1_fx!=0) 
		{
#ifdef WMOPS_FX
			move16();
#endif			
    		beta = 0x7FFF;
			if (sub(st_fx->rms_Input_fx, round_fx(L_mult0(st_fx->rms_fgd_fx, 0x6666)))>0)
			{
#ifdef WMOPS_FX
				move16();
#endif
				beta = 0x7FDF;
			}
			if (sub(st_fx->rms_Input_fx, shr(st_fx->rms_fgd_fx, 1))>0)
			{
#ifdef WMOPS_FX
				move16();
#endif
				beta = 0x7EB7; 			
			}			
		}		

		IF (sub(beta, 0x7FFF)!=0) 
		{
			L_temp = L_shl(L_mult(negate(add(beta, (Word16)0x8000)), st_fx->rms_Input_fx), 2);
			st_fx->rms_fgd_fx = round_fx(L_add(L_mult(beta, st_fx->rms_fgd_fx), L_temp));
		}
		L_temp = L_mult(sub(log10_Q15(st_fx->rms_fgd_fx), 0x09A2), 0x2800);
		st_fx->fgd_lev_fx = round_fx(L_shl(L_temp, 2));
		
		/* Calculate long term SNR */                                       
		st_fx->lsnr_fx = sub(st_fx->fgd_lev_fx, st_fx->bgd_lev_fx);
		st_fx->lsnr_fx = mult_r(st_fx->lsnr_fx, 0x6CCD);
	}
#ifdef DRAM
    accDRAM(FRAME_COUNT, -5);
#endif	
}


/**********************************************************************
* FUNCTION      :  parametersVAD_mssnr_fx ()                          *
*---------------------------------------------------------------------*
* PURPOSE       :  calculates the segmental SNR of the input frame    *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)  SigFpw_fx  : pointer to the spectrum power density of *
*                            the input frame                          *
*   (Word16)    nb         : sampling rate flag                       *
*   (Word16 *)  fft_exp_fx : pointer to the the left shift factor of  *
*                            FFT output                               *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                   None                                              *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *)st_fx      : the vad state                            *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*   (Word16)    mssnr      : modified SSNR                            *
**********************************************************************/
Word16 parametersVAD_mssnr_fx(vadState_fx *st_fx, 
							 Word16 *SigFpw_fx, 
							 Word16 *fft_exp_fx, 
							 Word16 nb)
{
	Word16 i,j,j1,j2;
	Word16 snr;
	Word16 alpha;
	Word16 one_minus_alpha;
	Word32 frm_cnt_fx_minus_1000;
	Word16 fft_exp_fx_shift_left_by_one;
	Word16 fgd_lev_fx_minus_0xE100;
	Word16 fgd_lev_fx_minus_0xEB00;
	Word32 enrg;
	Word16 tmp,tmp1;
	Word16 snrt;
	Word16 mssnr;
	Word16 *seg_len;
	Word16 exp_ch_enrg;
	Word16 exp_ch_noise;
	Word16 ch_enrg_tmp;
	Word16 ch_noise_tmp;
	Word16 temp;
	Word16 *band_tbl_ptr;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 25);
#endif
	
#ifdef WMOPS_FX
	move16();
#endif                                                                            
	IF (sub(st_fx->lsnr_fx, 0x2300)>0)
	{
		alpha = 0x6000;
		if(L_sub(st_fx->frm_cnt_fx, 1)==0)
		{
#ifdef WMOPS_FX
			move16();
#endif 
			alpha = 0x7FFF;
		}
	}
	ELSE
	{
		alpha = 0x4666;
		if(L_sub(st_fx->frm_cnt_fx, 1)==0)
		{
#ifdef WMOPS_FX
			move16();
#endif
			alpha = 0x7FFF;
		}
	}
	one_minus_alpha = negate(add((Word16)0x8000, alpha));
	frm_cnt_fx_minus_1000 = L_sub(st_fx->frm_cnt_fx, 1000);
	fft_exp_fx_shift_left_by_one = shl(*fft_exp_fx, 1);
	fgd_lev_fx_minus_0xE100 = sub(st_fx->fgd_lev_fx, (Word16)0xE100);
	fgd_lev_fx_minus_0xEB00 = sub(st_fx->fgd_lev_fx, (Word16)0xEB00);

	IF (nb) 
	{
		band_tbl_ptr = band_tbl_8k;
		seg_len = ssnr_seg_len_8k;
	}
	ELSE
	{
		band_tbl_ptr = band_tbl_16k;
		seg_len = ssnr_seg_len;
	}

    FOR (i=LO_BAND; i<=HI_BAND; i++)
    {
#ifdef WMOPS_FX
		move32();
#endif  	
        enrg = 0;                                                         		
#ifdef WMOPS_FX
		move16();
		move16();
#endif 
		j1 = *band_tbl_ptr++;
		j2 = *band_tbl_ptr++;		                                                                        
        FOR (j=j1; j<=j2; j++)
		{
			enrg = L_mac0(enrg, SigFpw_fx[j], SigFpw_fx[j]);                                    
		}
        enrg = L_mls(enrg, seg_len[i]);
		enrg = L_shr(enrg, fft_exp_fx_shift_left_by_one);
#ifdef WMOPS_FX
		move32();
#endif 	    
		st_fx->band_enrg_fx[i] = L_add(L_mls(st_fx->band_enrg_fx[i], one_minus_alpha), L_mls(enrg, alpha));  
		IF (frm_cnt_fx_minus_1000>0)
		{ 
			IF (fgd_lev_fx_minus_0xE100<0)
			{                                                               
				if (L_sub(st_fx->band_enrg_fx[i], 30)<0) 
				{
#ifdef WMOPS_FX
		move32();
#endif 	
					st_fx->band_enrg_fx[i] = 30;                                   
				}				
			}                                                                                                 
			ELSE
			{
				IF (fgd_lev_fx_minus_0xEB00<0)
				{                                                               
					if (L_sub(st_fx->band_enrg_fx[i], 100)<0) 
					{
#ifdef WMOPS_FX
	    move32();
#endif 	
						st_fx->band_enrg_fx[i] = 100;                                   
					}
				}
				ELSE
				{				                                                
					if (L_sub(st_fx->band_enrg_fx[i], 150)<0) 
					{
#ifdef WMOPS_FX
		move32();
#endif 	
						st_fx->band_enrg_fx[i] = 150;                                  
					}				
				}
			}
		}
		ELSE
		{
            if (L_sub(st_fx->band_enrg_fx[i], MIN_BAND_ENRG)<0) 
			{
#ifdef WMOPS_FX
		move32();
#endif 
				st_fx->band_enrg_fx[i] = MIN_BAND_ENRG;                            
			}
		}
	}
	
	IF (L_sub(st_fx->frm_cnt_fx, 4)<=0) 
    {   	
		FOR (i=LO_BAND; i<=HI_BAND; i++)
		{
#ifdef WMOPS_FX
			move32();
#endif 
			st_fx->band_enrg_bgd_fx[i] = INE;
			if(L_sub(st_fx->band_enrg_fx[i], INE)>0)
			{
#ifdef WMOPS_FX
			move32();
#endif
				st_fx->band_enrg_bgd_fx[i] = st_fx->band_enrg_fx[i];
			}    
		}		
    } 
	                                   
#ifdef WMOPS_FX
	move16();
    move16();
#endif 
	mssnr = 0;                                                             
	FOR (i = LO_BAND; i <= HI_BAND; i++) 
	{
		exp_ch_enrg = norm_l(st_fx->band_enrg_fx[i]);		
		exp_ch_noise = norm_l(st_fx->band_enrg_bgd_fx[i]);
		IF (sub(exp_ch_enrg, 15)>0)
		{
			ch_enrg_tmp = log10_Q15(extract_l(st_fx->band_enrg_fx[i]));
		}
		ELSE
		{
			temp = log10_Q15(round_fx(L_shl(st_fx->band_enrg_fx[i], exp_ch_enrg)));
			ch_enrg_tmp = add(temp, log10_exp_factor_fx[sub(15, exp_ch_enrg)]);
		}
		
		IF(sub(exp_ch_noise, 15)>0)
		{
			ch_noise_tmp = log10_Q15(extract_l(st_fx->band_enrg_bgd_fx[i]));
		}
		ELSE
		{
			temp = log10_Q15(round_fx(L_shl(st_fx->band_enrg_bgd_fx[i], exp_ch_noise)));
			ch_noise_tmp = add(temp, log10_exp_factor_fx[sub(15, exp_ch_noise)]);
		}		
		snr = mult_r(20480, sub(ch_enrg_tmp, ch_noise_tmp));	
		tmp = mult_r(snr, shl(mult_r(snr, 0x051F), 6)); 
		tmp1 = mult_r(snr, snr);		
		
		if (sub(tmp, 128)>0) 
		{
#ifdef WMOPS_FX
			move16();
#endif 
			tmp = 128;                                                            
		} 
		
		if (sub(tmp1, 128)>0) 
		{
#ifdef WMOPS_FX
			move16();
#endif
			tmp1 = 128;                                                           
		}                                                                       
#ifdef WMOPS_FX
		test();
#endif
		IF (sub(i, 12)<=0 && sub( i, 1)>0)
		{
			snrt = round_fx(L_shl(L_mult(snr, tmp1), 5));			
		}
		ELSE
		{
			snrt = round_fx(L_shl(L_mult(snr, tmp), 5));			
		}
		
		if (snrt<0) 
		{
#ifdef WMOPS_FX
			move16();
#endif
			snrt = 0;
		}
		mssnr = add(mssnr, snrt);                                                                                                                               
	}	
#ifdef DRAM
    accDRAM(FRAME_COUNT, -25);
#endif
	return (mssnr);
}


/**********************************************************************
* FUNCTION     : parametersVAD_FQ_Stability_fx ()                     *
*---------------------------------------------------------------------*
* PURPOSE      : calculate the frequency stability of the input frame *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st_fx          : the vad state                       *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/                                  
void parametersVAD_FQ_Stability_fx(vadState_fx *st_fx)
{                                                                           
    Word32 tmp1[16],sum,sum_s,L_temp;
	Word16 avg,i,tmp;
	Word16 exp_enrg;
	Word16 exp_enrg_long;
	Word16 norm_enrg;
	Word16 norm_enrg_long;
    Word16 exp_tmp,norm_tmp;	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 47);
#endif
	
	IF (L_sub(st_fx->frm_cnt_fx, 1) == 0)
	{
		FOR (i=0; i<16; i++)
		{
#ifdef WMOPS_FX
			move32();
#endif
			st_fx->band_enrg_mov_fx[i] = st_fx->band_enrg_fx[i];
		}
	}

	tmp = negate(add((Word16)0x8000, 0x7333));
    FOR (i=0; i<16; i++)
	{
		exp_enrg = norm_l(st_fx->band_enrg_fx[i]);
		exp_enrg_long = norm_l(st_fx->band_enrg_mov_fx[i]);
		norm_enrg = round_fx(L_shl(st_fx->band_enrg_fx[i], exp_enrg));
		norm_enrg_long = round_fx(L_shl(st_fx->band_enrg_mov_fx[i], exp_enrg_long));
        IF (sub(norm_enrg, norm_enrg_long)>=0)
		{
			norm_enrg = shr(norm_enrg, 1);
			exp_enrg = sub(exp_enrg, 1);
			exp_tmp = sub(exp_enrg, exp_enrg_long);
			norm_tmp = div_s(norm_enrg, norm_enrg_long);
#ifdef WMOPS_FX
			move32();
#endif
            tmp1[i] = L_shr(L_deposit_l(norm_tmp), add(exp_tmp, 2));
		}
		ELSE
		{
			exp_tmp = sub(exp_enrg, exp_enrg_long);
			norm_tmp = div_s(norm_enrg, norm_enrg_long);
#ifdef WMOPS_FX
			move32();
#endif
            tmp1[i] = L_shr(L_deposit_l(norm_tmp), add(exp_tmp, 2));
		}
				
		L_temp = L_mls(st_fx->band_enrg_fx[i], tmp);
#ifdef WMOPS_FX
		move32();
#endif
        st_fx->band_enrg_mov_fx[i] = L_add(L_temp, L_mls(st_fx->band_enrg_mov_fx[i], 0x7333));		
	}
#ifdef WMOPS_FX
	move32();
	move32();
    move32();
#endif	
	sum = 0;
	sum_s = 0;
	FOR (i=0; i<16; i++)
	{
		sum = L_add(sum, L_shr(tmp1[i], 1));
		exp_tmp = norm_l(tmp1[i]);
		norm_tmp = round_fx(L_shl(tmp1[i], exp_tmp));
		exp_tmp = sub(shl(exp_tmp, 1), 5);
		sum_s = L_add(sum_s, L_shr(L_mult(norm_tmp, norm_tmp), exp_tmp));		
	}
	avg = round_fx(L_shl(L_mls(sum, 8192), 3));
	
	IF(L_sub(sum, 32768) < 0)
	{
		L_temp = L_shr(L_mult(extract_l(sum), avg), 14);
	}
	ELSE
	{
		L_temp = L_mls(sum, shl(avg, 2));
	}
	st_fx->fq_stb_fx = L_sub(sum_s, L_temp);	
#ifdef DRAM
    accDRAM(FRAME_COUNT, -47);
#endif    
}


/**********************************************************************
* FUNCTION     : parametersVAD_background_flux_fx ()                  *
*---------------------------------------------------------------------*
* PURPOSE      : detects the background fluctuation                   *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16)     mssnr_fx  : Modified SSNR                            *
*   (Word16)     bcgUpdate : noise estimate update flag               *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st_fx     : the vad state                            *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/ 
void parametersVAD_background_flux_fx(vadState_fx *st_fx, 
									  Word16 mssnr_fx, 
									  Word16 bcgUpdate)
{
	Word32 L_temp;	
#ifdef DRAM
    accDRAM(FRAME_COUNT, 2);
#endif

    IF (bcgUpdate)
	{
		IF (sub(mssnr_fx, 480)<=0)
		{             			
			IF (st_fx->flux_bgd_init_cnt_fx>0)
			{
				IF(sub(shl(mssnr_fx, 6), st_fx->flux_bgd_fx)>0)
				{
					L_temp = L_mult(negate(add((Word16)0x8000, 0x7A3D)), mssnr_fx);
					L_temp = L_shl(L_temp, 6);
					st_fx->flux_bgd_fx = mac_r(L_temp, 0x7A3D, st_fx->flux_bgd_fx);      
				}
				ELSE
				{
					L_temp = L_mult(negate(add((Word16)0x8000, 0x7F5C)), mssnr_fx);
					L_temp = L_shl(L_temp, 6);
					st_fx->flux_bgd_fx = mac_r(L_temp, 0x7F5C, st_fx->flux_bgd_fx);
				}
				
				st_fx->flux_bgd_init_cnt_fx = sub(st_fx->flux_bgd_init_cnt_fx, 1);                                                   
				if (st_fx->flux_bgd_init_cnt_fx<0)
				{
#ifdef WMOPS_FX
					move16();
#endif
					st_fx->flux_bgd_init_cnt_fx = 0;                                             
				}
			}
			ELSE
			{
				IF(sub(shl(mssnr_fx, 6), st_fx->flux_bgd_fx)>0)
				{
					if (sub(shl(mssnr_fx, 6), st_fx->flux_bgd_fx)>6144)
					{
						if (st_fx->flux_bgd_fx<7168)
						{
							mssnr_fx = add(shr(st_fx->flux_bgd_fx, 6), 64);
						}
					}
					L_temp = L_mult(negate(add((Word16)0x8000, 0x7F9E)), mssnr_fx);

					L_temp = L_shl(L_temp, 6);
					st_fx->flux_bgd_fx = mac_r(L_temp, 0x7F9E, st_fx->flux_bgd_fx);			
				}
				ELSE
				{
					L_temp = L_mult(negate(add((Word16)0x8000, 0x7FF6)), mssnr_fx);
					L_temp = L_shl(L_temp, 6);
					st_fx->flux_bgd_fx = mac_r(L_temp, 0x7FF6, st_fx->flux_bgd_fx);					
				}
			}
			
		}
	}
#ifdef DRAM
    accDRAM(FRAME_COUNT, -2);
#endif
}


