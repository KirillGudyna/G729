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
#include "vad_fx.h"
#include "parameters_fx.h"
#include "gsad_math_adv_fx.h"
#include "preproc_fx.h" 
#include "stl.h"
#include "Rom_fx.h"
#include "ereal_fft_fx.h"


/**********************************************************************
* FUNCTION      :  preproc_pre_emp_windowing_fx ()                    *
*---------------------------------------------------------------------*
* PURPOSE       :  pre-emphasizes the input frame and adds asymmetric *
*                  window to the pre-emphasized frame                 *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)   Input   : pointer to the frame of speech             *
*   (Word16)     nb      : sampling rate flag                         *
*   (vadState *) st_fx   : the classifier state                       *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                None                                                 *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (Float32 *)  SigTpw  : pointer to the processed frame of speech   *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/
void  preproc_pre_emp_windowing_fx(Word16* SigTpw_fx, 
								   Word16* Input, 
								   Word16 nb, 
								   vadState_fx *st_fx)
{
    Word16 i;
	Word16 nfft; 
	Word16 sample;
	Word16 header;
    Word16 tmp,tmp1;
	Word16 *ptr_window_16k_fx;
	Word16 *ptr_Input;
	Word16 *ptr_SigTpw_fx;


    IF (nb)
	{
		nfft = FFTSIZE8k;                                  
		sample = SAMPLE8k;                              
		header = 48;                                    
	}
	ELSE
	{
		nfft = FFTSIZE16k;
		sample = SAMPLE16k;
		header = 96;
	}
		                                                    
	FOR (i=0; i<header; i++)
	{
		SigTpw_fx[i] = st_fx->pre_samples_fx[i];            
	}

	SigTpw_fx[header] = sub(Input[0], mult_r(0x6666, st_fx->emp_smpl_fst_fx)); 
	
	ptr_Input = &Input[0] ;                                                 
	tmp1 = add(header, 1);
	FOR (i=tmp1; i<nfft; i++)
	{
	    tmp = mult_r(0x6666, *ptr_Input++);
	    SigTpw_fx[i] = sub(*ptr_Input, tmp);
	}

	st_fx->emp_smpl_fst_fx = Input[sub(sample, 1)];                 
	
	tmp = sub(nfft, header); 
	ptr_SigTpw_fx = &SigTpw_fx[tmp];
	FOR (i=0; i<header; i++)
	{
		st_fx->pre_samples_fx[i] = *ptr_SigTpw_fx++;  
	}
	
    IF (nb)
	{
		ptr_window_16k_fx = &window_16k_fx[1];
		FOR (i=0; i<nfft; i++)
		{
			SigTpw_fx[i] = mult_r(SigTpw_fx[i], *ptr_window_16k_fx++);
			ptr_window_16k_fx++;
		}
	}
	ELSE
	{
		FOR (i=0; i<nfft; i++)
		{
			SigTpw_fx[i] = mult_r(SigTpw_fx[i], window_16k_fx[i]);     
		}
	}
}


/**********************************************************************
* FUNCTION      :  preproc ()                                         *
*---------------------------------------------------------------------*
* PURPOSE       :  preprocesses the input signal and performs the     *
*                  FFT afterwards                                     *
*                                                                     *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (Word16 *)   Input   : pointer to the frame of speech             *
*   (Word16)     nb      : sampling rate flag                         *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*   (Float32 *)  SigFpw  : pointer to the spectrum power density of   *
*                          the speech frame                           *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (vadState *) st      : the vad state                              *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                None                                                 *
**********************************************************************/
void preproc_fx(Word16 *Input, Word16 *SigFpw_fx, 
				Word16 *fft_exp_fx, Word16 nb, 
				vadState_fx *st_fx)
{
	Word16 i;
	Word16 exp_SigFpw;
	Word16 tmp_SigFpw;
	Word32 L_temp;
    Word16 nfft, fftsize;
	Word16 r_fft_buf[FFTSIZE16k];
    Word16 *ptr_fft_buf;                                                   
                                                                                
    
	IF (nb)                                                                               
    {
		nfft = FFTSIZE8k;
	    fftsize = HALF_FFTSIZE8k;
    }
    ELSE
    {
    	nfft = FFTSIZE16k;
	    fftsize = HALF_FFTSIZE16k;
    }

	parametersCommon_rms_fx (Input, nb, st_fx);

	preproc_pre_emp_windowing_fx(r_fft_buf, Input, nb, st_fx);

	ereal_fft_fx(r_fft_buf, fft_exp_fx, shl(fftsize, 1));

	SigFpw_fx[0] = mult_r(3277, abs_s(r_fft_buf[0]));
    ptr_fft_buf = &r_fft_buf[2];

	FOR (i=1; i<fftsize; i++)
	{	
    	L_temp = L_mult0(*ptr_fft_buf, *ptr_fft_buf);
		ptr_fft_buf++;
		L_temp = L_mac0(L_temp, *ptr_fft_buf, *ptr_fft_buf);
		ptr_fft_buf++;
	    exp_SigFpw = norm_l(L_temp);
		tmp_SigFpw = round_fx(L_shl(L_temp, exp_SigFpw));
		L_temp = L_mult(Sqrt_Q0(tmp_SigFpw), Sqrt_exp_factor_32bit_fx[exp_SigFpw]);
		SigFpw_fx[i] = round_fx(L_shl(L_mls(L_temp, 26214), 4));		
	}
}

