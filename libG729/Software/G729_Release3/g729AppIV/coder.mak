# G.729 with ANNEX B   Version 1.5    Last modified: October 2006 

# makefile for ANSI-C version of G.729 Annex B.
# NOTE: Edit these options to reflect your particular system

# Options for GCC C compiler
CC= gcc
CFLAGS = -O2 -Wall

# macro noms des objets
OBJETS = \
 acelp_co.o basop32.o bits.o cod_ld8k.o coder.o\
 dspfunc.o filter.o gainpred.o lpc.o lpcfunc.o\
 lspgetq.o oper_32b.o p_parity.o pitch.o pre_proc.o\
 pred_lt3.o pwf.o qua_gain.o qua_lsp.o tab_ld8k.o\
 util.o taming.o dtx.o qsidgain.o\
 qsidlsf.o calcexc.o tab_dtx.o dec_sid.o\
 rom_fx.o vad_fx.o preproc_fx.o parameters_fx.o ereal_fft_fx.o\
 gsad_math_adv_fx.o 	

# edition de liens
coder : $(OBJETS)
	$(CC) -o coder $(OBJETS) -lm

# Compilations si changements

acelp_co.o : acelp_co.c typedef.h basop32.h ld8k.h
	$(CC) $(CFLAGS) -c acelp_co.c

basop32.o : basop32.c typedef.h basop32.h
	$(CC) $(CFLAGS) -c basop32.c

bits.o : bits.c typedef.h ld8k.h tab_ld8k.h\
    dtx.h tab_dtx.h octet.h
	$(CC) $(CFLAGS) -c bits.c

cod_ld8k.o : cod_ld8k.c typedef.h basop32.h ld8k.h\
    tab_ld8k.h oper_32b.h dtx.h sid.h
	$(CC) $(CFLAGS) -c cod_ld8k.c

coder.o : coder.c typedef.h basop32.h ld8k.h\
    dtx.h octet.h
	$(CC) $(CFLAGS) -c coder.c

dspfunc.o : dspfunc.c typedef.h basop32.h ld8k.h\
    tab_ld8k.h
	$(CC) $(CFLAGS) -c dspfunc.c

filter.o : filter.c typedef.h basop32.h ld8k.h
	$(CC) $(CFLAGS) -c filter.c

gainpred.o : gainpred.c typedef.h basop32.h ld8k.h\
    tab_ld8k.h oper_32b.h
	$(CC) $(CFLAGS) -c gainpred.c

lpc.o : lpc.c typedef.h basop32.h oper_32b.h\
    ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c lpc.c

lpcfunc.o : lpcfunc.c typedef.h basop32.h oper_32b.h\
    ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c lpcfunc.c

lspgetq.o : lspgetq.c typedef.h basop32.h ld8k.h
	$(CC) $(CFLAGS) -c lspgetq.c

oper_32b.o : oper_32b.c typedef.h basop32.h oper_32b.h
	$(CC) $(CFLAGS) -c oper_32b.c

p_parity.o : p_parity.c typedef.h basop32.h ld8k.h
	$(CC) $(CFLAGS) -c p_parity.c

pitch.o : pitch.c typedef.h basop32.h oper_32b.h\
    ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c pitch.c

pre_proc.o : pre_proc.c typedef.h basop32.h oper_32b.h\
    ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c pre_proc.c

pred_lt3.o : pred_lt3.c typedef.h basop32.h ld8k.h\
    tab_ld8k.h
	$(CC) $(CFLAGS) -c pred_lt3.c

pwf.o : pwf.c typedef.h basop32.h ld8k.h
	$(CC) $(CFLAGS) -c pwf.c

qua_gain.o : qua_gain.c typedef.h basop32.h oper_32b.h\
    ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c qua_gain.c

qua_lsp.o : qua_lsp.c typedef.h basop32.h ld8k.h\
    tab_ld8k.h
	$(CC) $(CFLAGS) -c qua_lsp.c

tab_ld8k.o : tab_ld8k.c typedef.h ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c tab_ld8k.c

util.o : util.c typedef.h basop32.h ld8k.h
	$(CC) $(CFLAGS) -c util.c

taming.o : taming.c typedef.h basop32.h oper_32b.h\
    ld8k.h tab_ld8k.h
	$(CC) $(CFLAGS) -c taming.c

dtx.o : dtx.c typedef.h basop32.h ld8k.h\
    oper_32b.h tab_ld8k.h dtx.h tab_dtx.h sid.h
	$(CC) $(CFLAGS) -c dtx.c

qsidgain.o : qsidgain.c typedef.h basop32.h oper_32b.h\
    ld8k.h dtx.h sid.h tab_dtx.h
	$(CC) $(CFLAGS) -c qsidgain.c

qsidlsf.o : qsidlsf.c typedef.h basop32.h ld8k.h\
    tab_ld8k.h sid.h dtx.h tab_dtx.h
	$(CC) $(CFLAGS) -c qsidlsf.c

calcexc.o : calcexc.c typedef.h ld8k.h dtx.h\
    basop32.h oper_32b.h
	$(CC) $(CFLAGS) -c calcexc.c

tab_dtx.o : tab_dtx.c typedef.h ld8k.h dtx.h\
    tab_dtx.h
	$(CC) $(CFLAGS) -c tab_dtx.c

dec_sid.o : dec_sid.c typedef.h ld8k.h tab_ld8k.h\
    basop32.h dtx.h sid.h tab_dtx.h
	$(CC) $(CFLAGS) -c dec_sid.c

ereal_fft_fx.o: ereal_fft_fx.c rom_fx.h stl.h constdef_fx.h ereal_fft_fx.h
	$(CC) $(CFLAGS) -c ereal_fft_fx.c

gsad_math_adv_fx.o: gsad_math_adv_fx.c stl.h rom_fx.h gsad_math_adv_fx.h
	$(CC) $(CFLAGS) -c gsad_math_adv_fx.c

parameters_fx.o: parameters_fx.c stl.h vad_fx.h parameters_fx.h gsad_math_adv_fx.h\
    rom_fx.h constdef_fx.h
	$(CC) $(CFLAGS) -c parameters_fx.c

preproc_fx.o: preproc_fx.c stl.h parameters_fx.h gsad_math_adv_fx.h preproc_fx.h\
    rom_fx.h ereal_fft_fx.h
	$(CC) $(CFLAGS) -c preproc_fx.c 

vad_fx.o: vad_fx.c stl.h parameters_fx.h gsad_math_adv_fx.h vad_fx.h rom_fx.h	
	$(CC) $(CFLAGS) -c vad_fx.c

rom_fx.o: rom_fx.c typedef.h constdef_fx.h
	$(CC) $(CFLAGS) -c rom_fx.c