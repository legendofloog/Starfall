	.include "MPlayDef.s"

	.equ	song026E_grp, voicegroup000
	.equ	song026E_pri, 20
	.equ	song026E_rev, 0
	.equ	song026E_mvl, 127
	.equ	song026E_key, 0
	.equ	song026E_tbs, 1
	.equ	song026E_exg, 0
	.equ	song026E_cmp, 1

	.section .rodata
	.global	song026E
	.align	2


@**************** Track 1 (Midi-Chn.0) ****************@

song026E_001:
@  #01 @000   ----------------------------------------
 .byte   KEYSH , song026E_key+0
 .byte   TEMPO , 150*song026E_tbs/2
 .byte   VOICE , 110
 .byte   VOL , 125*song026E_mvl/mxv
 .byte   N24 ,Cn3 ,v127
 .byte   W24
 .byte   FINE

@**************** Track 2 (Midi-Chn.1) ****************@

song026E_002:
@  #02 @000   ----------------------------------------
 .byte   KEYSH , song026E_key+0
 .byte   TEMPO , 150*song026E_tbs/2
 .byte   VOICE , 50
 .byte   VOL , 80*song026E_mvl/mxv
 .byte   N12 ,Cn3 ,v127
 .byte   W12
 .byte   N11 ,Cn3 ,v096
 .byte   W12
 .byte   FINE

@******************************************************@
	.align	2

song026E:
	.byte	2	@ NumTrks
	.byte	0	@ NumBlks
	.byte	song026E_pri	@ Priority
	.byte	song026E_rev	@ Reverb.
    
	.word	song026E_grp
    
	.word	song026E_001
	.word	song026E_002

	.end
