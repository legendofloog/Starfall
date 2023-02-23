	.include "MPlayDef.s"

	.equ	song026F_grp, voicegroup000
	.equ	song026F_pri, 20
	.equ	song026F_rev, 0
	.equ	song026F_mvl, 127
	.equ	song026F_key, 0
	.equ	song026F_tbs, 1
	.equ	song026F_exg, 0
	.equ	song026F_cmp, 1

	.section .rodata
	.global	song026F
	.align	2


@**************** Track 1 (Midi-Chn.0) ****************@

song026F_001:
@  #01 @000   ----------------------------------------
 .byte   KEYSH , song026F_key+0
 .byte   TEMPO , 150*song026F_tbs/2
 .byte   VOICE , 113
 .byte   VOL , 102*song026F_mvl/mxv
 .byte   N24 ,Cn3 ,v127
 .byte   W24
 .byte   FINE

@**************** Track 2 (Midi-Chn.1) ****************@

song026F_002:
@  #02 @000   ----------------------------------------
 .byte   KEYSH , song026F_key+0
 .byte   TEMPO , 150*song026F_tbs/2
 .byte   VOICE , 50
 .byte   VOL , 80*song026F_mvl/mxv
 .byte   N12 ,Cn3 ,v127
 .byte   W12
 .byte   N11 ,Cn3 ,v096
 .byte   W12
 .byte   FINE

@******************************************************@
	.align	2

song026F:
	.byte	2	@ NumTrks
	.byte	0	@ NumBlks
	.byte	song026F_pri	@ Priority
	.byte	song026F_rev	@ Reverb.
    
	.word	song026F_grp
    
	.word	song026F_001
	.word	song026F_002

	.end
