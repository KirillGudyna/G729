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
#include "Rom_fx.h"
#include "gsad_math_adv_fx.h"


/*===================================================================*/
/* FUNCTION      :  Sqrt_Q15().                                      */
/*-------------------------------------------------------------------*/
/* PURPOSE       :  Calulating the sqrt() of a Word16 number in Q15  */
/*-------------------------------------------------------------------*/
/* INPUT ARGUMENTS  :                                                */
/*    (Word16)  : in_arg  (Q15)                                      */
/*-------------------------------------------------------------------*/
/* OUTPUT ARGUMENTS :                                                */
/*         _ None.                                                   */
/*-------------------------------------------------------------------*/
/* INPUT/OUTPUT ARGUMENTS :                                          */
/*         _ None.                                                   */
/*-------------------------------------------------------------------*/
/* RETURN ARGUMENTS :                                                */
/*    (Word16)  : out_val=sqrt(in_arg) (Q15)                         */
/*===================================================================*/
Word16 Sqrt_Q15(Word16 in_arg)
{
	Word16 x_fx;
	Word16 x_power_fx;
	Word32 L_temp;
	Word16 out_val_fx;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 5);
#endif
	IF (in_arg == 0)
	{
#ifdef DRAM
    accDRAM(FRAME_COUNT, -5);
#endif
		return(0);
	}

	x_fx = add(in_arg, (Word16)0x8000);
#ifdef WMOPS_FX
	move16();
    move32();
#endif
	x_power_fx = x_fx;
	L_temp = 0x7FFFFFFF;  

	L_temp = L_mac(L_temp, Sqrt_Taylor_coeff_fx[0], x_power_fx);          
	x_power_fx = round_fx(L_mult(x_power_fx, x_fx));                         
	L_temp = L_mac(L_temp, Sqrt_Taylor_coeff_fx[1], x_power_fx);          
	x_power_fx = round_fx(L_mult(x_power_fx, x_fx));                         
	L_temp = L_mac(L_temp, Sqrt_Taylor_coeff_fx[2], x_power_fx);          
	x_power_fx = round_fx(L_mult(x_power_fx, x_fx));                         
	L_temp = L_mac(L_temp, Sqrt_Taylor_coeff_fx[3], x_power_fx);          
	x_power_fx = round_fx(L_mult(x_power_fx, x_fx));                         
	L_temp = L_mac(L_temp, Sqrt_Taylor_coeff_fx[4], x_power_fx);         

	out_val_fx = round_fx(L_temp);                                          

#ifdef DRAM
    accDRAM(FRAME_COUNT, -5);
#endif
	return(out_val_fx);
}


/*===================================================================*/
/* FUNCTION      :  Sqrt_Q0().                                       */
/*-------------------------------------------------------------------*/
/* PURPOSE       :  Calulating the sqrt() of a Word16 number in Q0   */
/*-------------------------------------------------------------------*/
/* INPUT ARGUMENTS  :                                                */
/*    (Word16)  : in_arg  (Q0)                                       */
/*-------------------------------------------------------------------*/
/* OUTPUT ARGUMENTS :                                                */
/*         _ None.                                                   */
/*-------------------------------------------------------------------*/
/* INPUT/OUTPUT ARGUMENTS :                                          */
/*         _ None.                                                   */
/*-------------------------------------------------------------------*/
/* RETURN ARGUMENTS :                                                */
/*    (Word16)  : out_val=sqrt(in_arg) (Q0)                          */
/*===================================================================*/
Word16 Sqrt_Q0(Word16 in_arg)
{
	Word32 L_temp;
	Word16 temp;
	Word16 out_val_fx;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 4);
#endif

	temp = Sqrt_Q15(in_arg);
	L_temp = L_mult(temp, 181);					
	out_val_fx = round_fx(L_temp);   	
#ifdef DRAM
    accDRAM(FRAME_COUNT, -4);
#endif
	return(out_val_fx);
}


/*===================================================================*/
/* FUNCTION      :  log10_Q15().                                     */
/*-------------------------------------------------------------------*/
/* PURPOSE       :  Calulating the log10() of a Word16 number in Q15  */
/*-------------------------------------------------------------------*/
/* INPUT ARGUMENTS  :                                                */
/*    (Word16)  : in_arg  (Q15)                                      */
/*-------------------------------------------------------------------*/
/* OUTPUT ARGUMENTS :                                                */
/*         _ None.                                                   */
/*-------------------------------------------------------------------*/
/* INPUT/OUTPUT ARGUMENTS :                                          */
/*         _ None.                                                   */
/*-------------------------------------------------------------------*/
/* RETURN ARGUMENTS :                                                */
/*    (Word16)  : out_val=log10(in_arg) (Q12)                         */
/*===================================================================*/
Word16 log10_Q15(Word16 in_arg)
{
	Word16 x_fx;
	Word16 exp;
	Word16 norm_in_arg;
	Word16 x_power_fx;
	Word32 L_temp;
	Word16 out_val_fx;
#ifdef DRAM
    accDRAM(FRAME_COUNT, 8);
#endif 
	
	IF (in_arg<=0)
	{
#ifdef DRAM
    accDRAM(FRAME_COUNT, -8);
#endif
		return(0);
	}

    exp = norm_s(in_arg);
	norm_in_arg = shl(in_arg,exp);
	x_fx = add(norm_in_arg, (Word16)0x8000);
#ifdef WMOPS_FX
	move16();
#endif
	x_power_fx = x_fx;
	L_temp=L_mult(x_power_fx, log10_Taylor_coeff_fx[0]);
	x_power_fx = mult_r(x_power_fx, x_fx);
    L_temp=L_mac(L_temp, x_power_fx, log10_Taylor_coeff_fx[1]);
	x_power_fx = mult_r(x_power_fx, x_fx);
	L_temp=L_mac(L_temp, x_power_fx, log10_Taylor_coeff_fx[2]);
	x_power_fx = mult_r(x_power_fx, x_fx);
	L_temp=L_mac(L_temp, x_power_fx, log10_Taylor_coeff_fx[3]);
	x_power_fx = mult_r(x_power_fx,x_fx);
	L_temp=L_mac(L_temp, x_power_fx, log10_Taylor_coeff_fx[4]);
	x_power_fx = mult_r(x_power_fx, x_fx);
	L_temp=L_mac(L_temp, x_power_fx, log10_Taylor_coeff_fx[5]); 		
	out_val_fx = mult_r(round_fx(L_temp), 0x06F3); 
	out_val_fx=sub(out_val_fx, log10_exp_factor_internal_fx[exp]); 
#ifdef DRAM
    accDRAM(FRAME_COUNT, -8);
#endif
	return(out_val_fx);
}
