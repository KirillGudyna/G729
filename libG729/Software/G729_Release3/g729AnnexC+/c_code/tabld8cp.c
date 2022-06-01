/* ITU-T G.729 Software Package Release 2 (November 2006) */
/*
   ITU-T G.729 Annex C+ - Reference C code for floating point
                         implementation of G.729 Annex C+
                         (integration of Annexes B, D and E)
                          Version 2.1 of October 1999
*/

/*
 File : TABLD8CP.C
*/

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "ld8k.h"
#include "ld8cp.h"
#include "tabld8cp.h"

/*-----------------------------------------------------*
 | Tables for routine bits().                          |
 -----------------------------------------------------*/
int bitsno_B[PRM_SIZE_SID] = {1, /* SID Lsp : MA  */
                     5, /* SID Lsp : 1st stage */
                     4, /* SID Lsp : 2nd stage */
                     5 };  /* SID gain */

int bitsno_D[PRM_SIZE_D] = {1+NC0_B, /* MA + 1st stage */
                                 NC1_B*2, /* 2nd stage */
                                 8,  9,2, 6, /* first subframe  */
                                 4,  9,2, 6}; /* second subframe */
int bitsno_E_fwd[PRM_SIZE_E_fwd-1] =
    {1+NC0_B,             /* MA + 1st stage */
     NC1_B*2,             /* 2nd stage */
     8,1, 7,7,7,7,7, 7,   /* first subframe  */
     5,   7,7,7,7,7, 7};  /* second subframe */
int bitsno_E_bwd[PRM_SIZE_E_bwd-1] =
    {
     8,1, 13,10,7,7,7, 7,   /* first subframe  */
     5, 13,10,7,7,7, 7};  /* second subframe */



/*--------------------------------------------------------------------------*
 * tables specific to G729E                                                 *
 *--------------------------------------------------------------------------*/
FLOAT lag_bwd[M_BWD] = {
(F) 0.999892, (F) 0.999869, (F) 0.999831, (F) 0.999777, (F) 0.999707,
(F) 0.999622, (F) 0.999522, (F) 0.999407, (F) 0.999276, (F) 0.999129,
(F) 0.998968, (F) 0.998790, (F) 0.998598, (F) 0.998390, (F) 0.998167,
(F) 0.997928, (F) 0.997674, (F) 0.997405, (F) 0.997121, (F) 0.996821,
(F) 0.996506, (F) 0.996175, (F) 0.995830, (F) 0.995469, (F) 0.995093,
(F) 0.994702, (F) 0.994295, (F) 0.993874, (F) 0.993437, (F) 0.992985,
};

FLOAT hw[NRP+L_FRAME+M_BWD] = {
(F) 0.047765, (F) 0.095421, (F) 0.142859, (F) 0.189970, (F) 0.236649,
(F) 0.282787, (F) 0.328279, (F) 0.373022, (F) 0.416914, (F) 0.459853,
(F) 0.501743, (F) 0.542488, (F) 0.581994, (F) 0.620172, (F) 0.656934,
(F) 0.692196, (F) 0.725879, (F) 0.757904, (F) 0.788199, (F) 0.816695,
(F) 0.843326, (F) 0.868033, (F) 0.890757, (F) 0.911449, (F) 0.930060,
(F) 0.946547, (F) 0.960874, (F) 0.973008, (F) 0.982920, (F) 0.990588,
(F) 0.995995, (F) 0.999129, (F) 0.999982, (F) 0.998552, (F) 0.994842,
(F) 0.988862, (F) 0.981775, (F) 0.974740, (F) 0.967754, (F) 0.960819,
(F) 0.953934, (F) 0.947098, (F) 0.940311, (F) 0.933572, (F) 0.926882,
(F) 0.920240, (F) 0.913645, (F) 0.907098, (F) 0.900597, (F) 0.894143,
(F) 0.887735, (F) 0.881374, (F) 0.875058, (F) 0.868787, (F) 0.862561,
(F) 0.856379, (F) 0.850242, (F) 0.844149, (F) 0.838100, (F) 0.832094,
(F) 0.826131, (F) 0.820211, (F) 0.814333, (F) 0.808497, (F) 0.802703,
(F) 0.796951, (F) 0.791240, (F) 0.785569, (F) 0.779940, (F) 0.774351,
(F) 0.768801, (F) 0.763292, (F) 0.757822, (F) 0.752391, (F) 0.747000,
(F) 0.741646, (F) 0.736332, (F) 0.731055, (F) 0.725816, (F) 0.720614,
(F) 0.715450, (F) 0.710323, (F) 0.705233, (F) 0.700179, (F) 0.695161,
(F) 0.690180, (F) 0.685234, (F) 0.680323, (F) 0.675448, (F) 0.670607,
(F) 0.665802, (F) 0.661030, (F) 0.656293, (F) 0.651590, (F) 0.646921,
(F) 0.642285, (F) 0.637682, (F) 0.633112, (F) 0.628575, (F) 0.624070,
(F) 0.619598, (F) 0.615158, (F) 0.610750, (F) 0.606373, (F) 0.602027,
(F) 0.597713, (F) 0.593430, (F) 0.589177, (F) 0.584955, (F) 0.580763,
(F) 0.576601, (F) 0.572469, (F) 0.568367, (F) 0.564294, (F) 0.560250,
(F) 0.556235, (F) 0.552249, (F) 0.548291, (F) 0.544362, (F) 0.540461,
(F) 0.536588, (F) 0.532742, (F) 0.528925, (F) 0.525134, (F) 0.521371,
(F) 0.517635, (F) 0.513925, (F) 0.510242, (F) 0.506586, (F) 0.502956,
(F) 0.499351, (F) 0.495773, (F) 0.492220, (F) 0.488693, (F) 0.485190,
(F) 0.481713, (F) 0.478261, (F) 0.474834, (F) 0.471431, (F) 0.468053,
(F) 0.464699, (F) 0.461369, (F) 0.458062, (F) 0.454780, (F) 0.451521
};

int ipos[16]={0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0};

/*--------------------------------------------------------------------------*
 * tables specific to G729D                                                *
 *--------------------------------------------------------------------------*/
/* tables of positions for each track */
int trackTable0[16] = {1, 3, 6, 8, 11, 13, 16, 18, 21, 23, 26, 28, 31, 33, 36, 38};
int trackTable1[32] = {0, 2, 4, 5, 7, 9, 10, 12, 14, 15, 17, 19, 20, 22, 24, 25, 27, 29, 30, 32, 34, 35, 37, 39, 1, 6, 11, 16, 21, 26, 31, 36};
int posSearched[2] = { 16, 32}; /* number of positions search for each track */

int grayEncode[32] = {
 0,  1,  3,  2,  6,  7, 5, 4, 12, 13, 15, 14, 10, 11, 9, 8,
24, 25, 27, 26, 30, 31, 29, 28, 20, 21, 23, 22, 18, 19, 17, 16};
int grayDecode[32] = {
0, 1, 3, 2, 7, 6, 4, 5, 15, 14, 12, 13, 8, 9, 11, 10,
31, 30, 28, 29, 24, 25, 27, 26, 16, 17, 19, 18, 23, 22, 20, 21};

/* anti-sparseness post-processing */
FLOAT ph_imp_low[L_SUBFR]={
 (F)0.4483, (F)0.3515, (F)0.0387,-(F)0.0843,-(F)0.1731, (F)0.2293,-(F)0.0011,
 (F)-0.0857,(F)-0.0928, (F)0.1472, (F)0.0901,(F)-0.2571, (F)0.1155, (F)0.0444,
 (F)0.0665,(F)-0.2636, (F)0.2457,(F)-0.0642,(F)-0.0444, (F)0.0237, (F)0.0338,
 (F)-0.0728, (F)0.0688,(F)-0.0111,(F)-0.0206,(F)-0.0642, (F)0.1845,(F)-0.1734,
 (F)0.0327, (F)0.0953,(F)-0.1544, (F)0.1621,(F)-0.0711,(F)-0.1138, (F)0.2113,
 (F)-0.1187, (F)0.0206,(F)-0.0542, (F)0.0009,(F)0.3096};

FLOAT ph_imp_mid[L_SUBFR]={
 (F)0.9239, (F)0.1169, (F)-0.1232, (F)0.0907, (F)-0.0320, (F)-0.0306, (F)0.0756,
 (F)-0.0929, (F)0.0859, (F)-0.0681, (F)0.0535, (F)-0.0492, (F)0.0523, (F)-0.0542,
 (F)0.0471, (F)-0.0308, (F)0.0131, (F)-0.0052, (F)0.0144, (F)-0.0386, (F)0.0664,
 (F)-0.0826, (F)0.0770, (F)-0.0495, (F)0.0105, (F)0.0252, (F)-0.0467, (F)0.0526,
 (F)-0.0506, (F)0.0519, (F)-0.0630, (F)0.0807, (F)-0.0934, (F)0.0884, (F)-0.0604,
 (F)0.0170, (F)0.0238, (F)-0.0418, (F)0.0257, (F)0.0200};

FLOAT ph_imp_high[L_SUBFR]={
 (F)1.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0,
 (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0,
 (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0,
 (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0, (F)0.0};

/* 6.4k (for NTT CS-VQ)*/
FLOAT gbk1_6k[NCODE1_6K][2] = {
{ (F)0.357003, (F)0.00000},
/*{ (F)0.357003, (F)0.000010},*/
{ (F)0.178752, (F)0.065771},
{ (F)0.575276, (F)0.166704}, 
{ (F)0.370335, (F)0.371903}, 
{ (F)0.220734, (F)0.411803}, 
{ (F)0.193548, (F)0.566385}, 
{ (F)0.238962, (F)0.785625}, 
{ (F)0.304379, (F)1.360714}  
};
FLOAT gbk2_6k[NCODE2_6K][2] = {
/*{ (F)0.000010, (F)0.254841},*/
{ (F)0.00000, (F)0.254841},
{ (F)0.243384, (F)0.00000},
/*{ (F)0.243384, (F)0.000010},*/
{ (F)0.273293, (F)0.447009}, 
{ (F)0.480707, (F)0.477384}, 
{ (F)0.628117, (F)0.694884}, 
{ (F)0.660905, (F)1.684719}, 
{ (F)0.729735, (F)0.655223}, 
{ (F)1.002375, (F)0.959743}  
};

FLOAT coef_6k[2][2] = {
{ (F)36.632507, (F)2.514171},
{ (F) 0.399259, (F)0.073709}
};
 
FLOAT thr1_6k[NCODE1_6K-NCAN1_6K] = {
  (F)1.210869,
  (F)2.401702
};
 
FLOAT thr2_6k[NCODE2_6K-NCAN2_6K] = {
  (F)0.525915,
  (F)0.767320
};
 
int map1_6k[NCODE1_6K] = { 0, 4, 6, 5, 2, 1, 7, 3 };
 
int imap1_6k[NCODE1_6K] = { 0, 5, 4, 7, 1, 3, 2, 6 };
 
int map2_6k[NCODE2_6K] = { 0, 4, 3, 7, 5, 1, 6, 2 };
 
int imap2_6k[NCODE2_6K] = { 0, 5, 7, 2, 1, 4, 6, 3 };

FLOAT freq_prev_reset[M] = {  /* previous LSP vector(init) */
 (F)0.285599,  (F)0.571199,  (F)0.856798,  (F)1.142397,  (F)1.427997,
 (F)1.713596,  (F)1.999195,  (F)2.284795,  (F)2.570394,  (F)2.855993
};     /* PI*(float)(j+1)/(float)(M+1) */

FLOAT lwindow[M+2] = {
(F)0.99879038,
(F)0.99546894,
(F)0.98995779,
(F)0.98229335,
(F)0.97252620,
(F)0.96072035,
(F)0.94695264,
(F)0.93131180,
(F)0.91389754,
(F)0.89481964,
(F)0.87419660,
(F)0.85215437
};
