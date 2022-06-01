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
#include "Rom_fx.h"
#include "stl.h"
#include "constdef_fx.h"
#include "ereal_fft_fx.h"


/**********************************************************************
* FUNCTION      :  ereal_fft_fx ()                                    *
*---------------------------------------------------------------------*
* PURPOSE       :  FFT function for real-number sequences             *
*                  Done in-place                                      *
*---------------------------------------------------------------------*
* INPUT ARGUMENTS  :                                                  *
*   (short)    : size - the FFT size                                  *
*---------------------------------------------------------------------*
* OUTPUT ARGUMENTS :                                                  *
*                 None                                                *
*---------------------------------------------------------------------*
* INPUT/OUTPUT ARGUMENTS :                                            *
*   (short *)  : the FFT real input sequence and FFT output           *
*---------------------------------------------------------------------*
* RETURN ARGUMENTS :                                                  *
*                  None                                               *
**********************************************************************/
void ereal_fft_fx (Word16 *ereal_io_buff, 
				   Word16 *fft_exp_fx, 
				   Word16 size)

{
	Word16	ftmp1_real,ftmp1_imag,ftmp2_real,ftmp2_imag;
	Word16	i;
	Word16	*buff_ptr1,*buff_ptr2;
	Word16	*tbl_ptr1, *tbl_ptr2;
	Word16	size_by_four;
	Word32  L_temp;
#ifdef DRAM
	accDRAM(FRAME_COUNT, 12);
#endif
	/* Perform the complex FFT */
	*fft_exp_fx = complex_fft_mod_fx (ereal_io_buff, size);
	
	size_by_four = shr(size,2);

	/* Handling DC and foldover frequencies */
#ifdef WMOPS_FX
	move32();
	move32();
	move16();
	move16();
	move16();
#endif
	buff_ptr1 = ereal_io_buff;
	buff_ptr2 = ereal_io_buff+(size-2);	
	ftmp1_real = *buff_ptr1++;	
	ftmp2_real = *buff_ptr1--;
	*buff_ptr1++ = add(ftmp1_real, ftmp2_real); 	
	*buff_ptr1++ = sub(ftmp1_real, ftmp2_real); 	

	/* Handling the other frequencies */
#ifdef WMOPS_FX
	move32();
	move32();
#endif
	IF (sub(size, 128)==0)
	{
		tbl_ptr1 = FFT_phs_tbl_128_fx + 2;
		tbl_ptr2 = FFT_phs_tbl_128_fx + (size - 2);
	}
	ELSE
	{
		tbl_ptr1 = FFT_phs_tbl_256_fx + 2;
		tbl_ptr2 = FFT_phs_tbl_256_fx + (size - 2);
	}

	FOR (i=0; i<size_by_four; i++)
	{      
		ftmp1_real = add((*buff_ptr1++), (*buff_ptr2++));
		ftmp1_imag = sub((*buff_ptr1), (*buff_ptr2));
		ftmp2_real = add((*buff_ptr1--), (*buff_ptr2--));
		ftmp2_imag = sub((*buff_ptr2), (*buff_ptr1));
		L_temp = L_deposit_h(ftmp1_real);
		L_temp = L_mac(L_temp, (*tbl_ptr1++), ftmp2_real);
		L_temp = L_msu(L_temp, (*tbl_ptr1--), ftmp2_imag);
		(*buff_ptr1++) = round_fx(L_temp);
#ifdef WMOPS_FX
		move16();
#endif
		L_temp = L_deposit_h(ftmp1_imag);
		L_temp = L_mac(L_temp, (*tbl_ptr1++), ftmp2_imag);
		L_temp = L_mac(L_temp, (*tbl_ptr1++), ftmp2_real);
		(*buff_ptr1++) = round_fx(L_temp);
#ifdef WMOPS_FX
		move16();
#endif
		L_temp = L_deposit_h(ftmp1_real);
		L_temp = L_mac(L_temp, (*tbl_ptr2++), ftmp2_real);
		L_temp = L_mac(L_temp, (*tbl_ptr2--), ftmp2_imag);
		(*buff_ptr2++) = round_fx(L_temp);
#ifdef WMOPS_FX
		move16();
#endif
		L_temp = L_mult(ftmp1_imag,(Word16)0x8000);
		L_temp = L_msu(L_temp, (*tbl_ptr2++), ftmp2_imag);
		L_temp = L_mac(L_temp, (*tbl_ptr2--), ftmp2_real);
		(*buff_ptr2--) = round_fx(L_temp);
#ifdef WMOPS_FX
		move16();
#endif
		buff_ptr2 -= 2;	
		tbl_ptr2 -= 2;
#ifdef WMOPS_FX
		add(0,0); 
		add(0,0);
#endif
	}

	IF (sub(size , 128)==0)
	{
		*fft_exp_fx = sub(*fft_exp_fx, 7);
	}
	ELSE
	{
		*fft_exp_fx = sub(*fft_exp_fx, 8);
	}
#ifdef DRAM
	accDRAM(FRAME_COUNT, -12);
#endif
	return;
}       

                
Word16 complex_fft_mod_fx (Word16 *ereal_io_buff, Word16 size)
{
	Word16	i,j,k,ii,jj,kk,ji;
	Word32	ftmp32_real,ftmp32_imag;
	Word16	ftmp,ftmp16_real,ftmp16_imag;
	Word16	*buff_ptr1,*buff_ptr2;
	Word16	*tbl_ptr1,*tbl_ptr2;
	Word16	*phase_table_ptr,*phs_tbl_holder;
	Word16	k_loop_limit,num_stage;
	Word16  buff_exp_factor;
	Word16  shift_stage;
#ifdef DRAM
	accDRAM(FRAME_COUNT, 22);
#endif
#ifdef WMOPS_FX
	move16();
#endif
	ftmp = 0;
	FOR (i=0; i<size; i++) 
	{
#ifdef WMOPS_FX
		logic16();
#endif
		ftmp |= abs_s(ereal_io_buff[i]);
	}
	buff_exp_factor = norm_s(ftmp);

	ftmp = sub(buff_exp_factor, 7);
	FOR (i=0; i<size; i++)
	{
#ifdef WMOPS_FX
		move16();
#endif
		ereal_io_buff[i] = shl(ereal_io_buff[i], ftmp);
	}

	/* Rearrange the input array in bit reversed order */
#ifdef WMOPS_FX
	move32();
	move32();
	move32();
#endif
	IF (sub(size,128)==0)
	{
		tbl_ptr1 = FFT_i_table_128_fx;
		tbl_ptr2 = FFT_j_table_128_fx;
		k_loop_limit = 28;
	}
	ELSE
	{
		tbl_ptr1 = FFT_i_table_256_fx;
		tbl_ptr2 = FFT_j_table_256_fx;
		k_loop_limit = 56;
	}

	FOR (k=0; k<k_loop_limit; k++)
	{
#ifdef WMOPS_FX
		move32();
		move32();
		move16();
		move16();
		move16();
		move16();
		move16();
		move16();
#endif		
		buff_ptr1 = &ereal_io_buff[*tbl_ptr1++];
		buff_ptr2 = &ereal_io_buff[*tbl_ptr2++];		
		ftmp = *buff_ptr1;
		*buff_ptr1++ = *buff_ptr2;
		*buff_ptr2++ = ftmp;		
		ftmp = *buff_ptr1;
		*buff_ptr1 = *buff_ptr2;
		*buff_ptr2 = ftmp;
	}

	/* The FFT part */ 
#ifdef WMOPS_FX
	move32();
	move16();
	move16();
	move16();
#endif
	IF (sub(size, 128)==0)
	{
		phs_tbl_holder = FFT_phs_tbl_128_fx;
		num_stage = NUM_STAGE_128;
		shift_stage = 10;   /* no shift stage */
	}
	ELSE
	{
		phs_tbl_holder = FFT_phs_tbl_256_fx;
		num_stage = NUM_STAGE_256;
		shift_stage = 0;   /* shift at first stage */
	}
	ii = size;	

    FOR (i=0; i<sub(num_stage, 1); i++)
	{           
		jj = shl(2, i);                            /* FFT size */
		kk = shl(jj, 1);                           /* 2 * FFT size */
#ifdef WMOPS_FX
		move16();
#endif
		ji = 0;
		IF (sub(i, shift_stage)==0)
		{
			FOR (j=0; j<jj; j+=2)
			{         /* j is sample counter */
					
				FOR (k = j; k < size ; )
				{     /* k is butterfly top */
							
					/* Butterfly computations */
					ftmp32_real = L_mult(ereal_io_buff[jj + k], phs_tbl_holder[ji]);
					ftmp32_real = L_msu(ftmp32_real, ereal_io_buff[jj + k + 1], phs_tbl_holder[ji + 1]);			
					ftmp32_imag = L_mult(ereal_io_buff[jj + k + 1] , phs_tbl_holder[ji]);
					ftmp32_imag = L_mac(ftmp32_imag, ereal_io_buff[jj + k], phs_tbl_holder[ji + 1]);
					ftmp16_real = round_fx(ftmp32_real);
					ftmp16_imag = round_fx(ftmp32_imag);				
					ereal_io_buff[jj + k] = shr(sub(ereal_io_buff[k], ftmp16_real), 1);
					ereal_io_buff[jj + k + 1] = shr(sub(ereal_io_buff[k+1], ftmp16_imag), 1);
					ereal_io_buff[k] = shr(add(ereal_io_buff[k], ftmp16_real), 1);
					ereal_io_buff[k+1] = shr(add(ereal_io_buff[k+1], ftmp16_imag), 1);	
#ifdef WMOPS_FX
					move16();
					move16();
					move16();
					move16();
#endif
					k = add(k, kk);
				}
				ji = add(ji, ii);
			}
			ii = shr(ii, 1);                 /* 2 * number of FFT's */		
		}
		ELSE
		{
			FOR (j=0; j<jj; j+=2)  /* j is sample counter */
			{          		
				FOR (k=j; k<size; )  /* k is butterfly top */
				{     							
					/* Butterfly computations */
					ftmp32_real = L_mult(ereal_io_buff[jj + k], phs_tbl_holder[ji]);
					ftmp32_real = L_msu(ftmp32_real, ereal_io_buff[jj + k + 1], phs_tbl_holder[ji + 1]);			
					ftmp32_imag = L_mult(ereal_io_buff[jj + k + 1] , phs_tbl_holder[ji]);
					ftmp32_imag = L_mac(ftmp32_imag, ereal_io_buff[jj + k], phs_tbl_holder[ji + 1]);
					ftmp16_real = round_fx(ftmp32_real);
					ftmp16_imag = round_fx(ftmp32_imag);				
					ereal_io_buff[jj + k] = sub(ereal_io_buff[k], ftmp16_real);
					ereal_io_buff[jj + k + 1] = sub(ereal_io_buff[k + 1], ftmp16_imag);
					ereal_io_buff[k] = add(ereal_io_buff[k], ftmp16_real);
					ereal_io_buff[k + 1] = add(ereal_io_buff[k + 1], ftmp16_imag);	
#ifdef WMOPS_FX
					move16();
					move16();
					move16();
					move16();
#endif
					k = add(k, kk);
				}
				ji = add(ji, ii);
			}
			ii = shr(ii, 1);                 /* 2 * number of FFT's */		
		}
	}

	/* deepest loop */
	jj = shr(size, 1);	
#ifdef WMOPS_FX
	move32();
	move32();
	move32();
#endif
	phase_table_ptr = phs_tbl_holder;
	buff_ptr1 = &ereal_io_buff[jj];
	buff_ptr2 = &ereal_io_buff[0];

	FOR (j=0; j<jj; j=j+2)   /* j is sample counter */
	{         					
		/* Butterfly computations */
		ftmp32_real = L_mult((*buff_ptr1++), (*phase_table_ptr++));
		ftmp32_real = L_msu(ftmp32_real, *buff_ptr1, (*phase_table_ptr--));				
		ftmp32_imag = L_mult((*buff_ptr1--) , (*phase_table_ptr++));
		ftmp32_imag = L_mac(ftmp32_imag, *buff_ptr1, (*phase_table_ptr--));
		ftmp16_real = round_fx(ftmp32_real);
		ftmp16_imag = round_fx(ftmp32_imag);				
		(*buff_ptr1++) = sub((*buff_ptr2++), ftmp16_real);
		(*buff_ptr1++) = sub((*buff_ptr2--), ftmp16_imag);				
		(*buff_ptr2++) = add((*buff_ptr2), ftmp16_real);
		(*buff_ptr2++) = add((*buff_ptr2), ftmp16_imag);
#ifdef WMOPS_FX
		move16();
		move16();
		move16();
		move16();
#endif
		phase_table_ptr += ii;
#ifdef WMOPS_FX
		add(0,0); 
#endif
	}
#ifdef DRAM
	accDRAM(FRAME_COUNT, -22);
#endif	
	return(buff_exp_factor);
}                    

