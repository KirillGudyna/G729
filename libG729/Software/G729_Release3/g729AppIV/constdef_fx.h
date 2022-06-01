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

/* Definitions of all GSAD constants */

#define	NUM_STAGE_128	6
#define NUM_STAGE_256	7
#define SAMPLE16k       160
#define SAMPLE8k        80
#define HALF_FFTSIZE16k 128
#define HALF_FFTSIZE8k  64
#define FFTSIZE16k      256 
#define FFTSIZE8k       128                                           
#define	MIN_BAND_ENRG   100
#define	INE	200 
#define	LO_BAND			0
#define	HI_BAND			15 
#define	NUM_BAND		16 
#define HS_PEAK_MAX     5  
#define C_mssnr_BW_8K_fx   0x4000
#define C_mssnr_BL_8K_fx   0x4000
#define C_mssnr_QT_8K_fx   0x399A
#define C_mssnr_BW_16K_fx  0x4000
#define C_mssnr_BL_16K_fx  0x4000
#define C_mssnr_QT_16K_fx  0x399A
#define K_offset_BW_8K_fx   0x0
#define K_offset_BL_8K_fx   0x0666
#define K_offset_QT_8K_fx   0x0CCD
#define K_offset_BW_16K_fx  0x0
#define K_offset_BL_16K_fx  0x0666
#define K_offset_QT_16K_fx  0x0CCD
#define a_BW_8K_fx 0x3BBC
#define b_BW_8K_fx 0x147B
#define a_BL_8K_fx 0x3CCD
#define b_BL_8K_fx 0x1429
#define a_QT_8K_fx 0x3DDE
#define b_QT_8K_fx 0x13D7
#define a_BW_16K_fx 0x3BBC
#define b_BW_16K_fx 0x147B
#define a_BL_16K_fx 0x3CCD
#define b_BL_16K_fx 0x1429
#define a_QT_16K_fx 0x3DDE
#define b_QT_16K_fx 0x13D7
#define Start_diff_hist           300
#define Stable_alpha_fx           0x7333
#define One_minus_Stable_alpha_fx 0x0CCD
#define SNR_SUMT_HIST_SIZE_ACTUAL 512 
#define X_limit_fx                0x2666
#define T_SEARCH_RANGE            120
#define FLUX_F2_fx                48	
#define FLUXBUFSIZ_fx             20	
#define SMD_HIST_BUF_SIZE_fx      120	
#define SNR_SUMT_HIST_SIZE_fx     512
#define SNR_SUMT_BUFF_SIZE_fx     75




