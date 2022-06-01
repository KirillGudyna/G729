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


Word16 parametersVAD_dzcr_fx(Word16 zcr_fx, vadState_fx *st_fx);
void   parametersCommon_rms_fx(Word16 *Input, Word16 nb, vadState_fx *st_fx);
Word16 parametersVAD_zcr_fx(Word16 *Input, Word16 nb);
void   parametersVAD_lsnr_fx(vadState_fx *st_fx, Word16 bcgUpdate);
Word16 parametersVAD_mssnr_fx(vadState_fx *st_fx, Word16 *SigFpw_fx, Word16 *fft_exp_fx, Word16 nb);
void   parametersVAD_FQ_Stability_fx(vadState_fx *st_fx);
void   parametersVAD_background_flux_fx(vadState_fx *st_fx, Word16 mssnr_fx, Word16 bcgUpdate);
void   parametersVAD_Tonality_fx(vadState_fx *st_fx, Word16 *SigFpw_fx, Word16 nb);
Word16 parametersVAD_bcgUpdate_fx(vadState_fx *st_fx, Word16 mssnr_fx, Word16 *SigFpw_fx, Word16 nb);
void   parametersVAD_MusicBcgd_fx(vadState_fx *st_fx, Word16 *SigFpw_fx, Word16 bcgUpdate);
Word16 extract_var_fx(Word16 *seri_fx, Word16 mean_fx, Word16 len);


