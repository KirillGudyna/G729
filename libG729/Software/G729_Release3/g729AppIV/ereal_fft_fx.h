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

void ereal_fft_fx (Word16 *ereal_io_buff, Word16 *fft_exp_fx, Word16 size);
Word16 complex_fft_mod_fx (Word16 *ereal_io_buff, Word16 size);
