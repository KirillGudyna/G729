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

#include "vad_fx.h"

/*********************************************************************************
*                         DECLARATION OF PROTOTYPES
*********************************************************************************/
void preproc_pre_emp_windowing_fx(Word16* SigTpw_fx, Word16* Input, Word16 nb, vadState_fx *st_fx);
void preproc_fx(Word16 *Input, Word16 *SigFpw_fx, Word16 *fft_exp_fx, Word16 nb, vadState_fx * st_fx);
