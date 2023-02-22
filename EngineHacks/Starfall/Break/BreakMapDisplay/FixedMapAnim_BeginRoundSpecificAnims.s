.thumb

push {r4,r5,r6,r7,lr}   @ MapAnim_BeginRoundSpecificAnims
mov r7, r10
mov r6, r9
mov r5, r8
push {r5,r6,r7}
sub sp, #0x4
ldr r1, [PC, #0x1C] @ pointer:08081648 -> 0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
mov r0 ,r1
add r0, #0x58
ldrb r0, [r0, #0x0] @ pointer:0203E248 (MapAnimStruct@gMapAnimStruct.current round attacking actor id )
mov r10, r0
mov r0 ,r1
add r0, #0x5A
ldrh r2, [r0, #0x0] # pointer:0203E24A (MapAnimStruct@gMapAnimStruct.CurrentBattleRound 4byte data [24-31] damage /[23] other bitfield /[22] Not attack /[19-21] other bitfield /[00-15] big bitfield)
mov r0, #0x80
and r0 ,r2
mov r7 ,r1
cmp r0, #0x0
beq #0x808164C
    mov r5, r10
    b 0x8081652
    nop
08081648 E1F0 0203   @ ldrDATA
mov r0 ,r7
add r0, #0x59
ldrb r5, [r0, #0x0] # pointer:0203E249 (MapAnimStruct@gMapAnimStruct.current round defending actor id )
mov r6 ,r7
mov r0, r10
lsl r4 ,r0 ,#0x2
add r0 ,r4, R0
lsl r0 ,r0 ,#0x2
add r1 ,r6, #0x4
add r0 ,r0, R1
ldr r0, [r0, #0x0] # pointer:0203E1F4
add r0, #0x4A
ldrh r0, [r0, #0x0]
bl 0x08078278   @ GetSpellAssocReturnBool
lsl r0 ,r0 ,#0x18
str r4,[sp, #0x0]
cmp r0, #0x0
bne #0x8081692
    mov r0 ,r6
    add r0, #0x5A
    ldrh r1, [r0, #0x0] # pointer:0203E24A (MapAnimStruct@gMapAnimStruct.CurrentBattleRound 4byte data [24-31] damage /[23] other bitfield /[22] Not attack /[19-21] other bitfield /[00-15] big bitfield)
    mov r0, #0x2
    and r0 ,r1
    cmp r0, #0x0
    bne #0x8081682
        b 0x80818C4
    lsl r0 ,r5 ,#0x2
    add r0 ,r0, R5
    lsl r0 ,r0 ,#0x2
    add r0 ,r0, R6
    ldr r0, [r0, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    bl 0x0807CB24   @ MapAnim_BeginMISSAnim
    b 0x80818C4
08081692 1C31   mov r1 ,r6
08081694 315F   add r1, #0x5F
08081696 2001   mov r0, #0x1
08081698 7008   STRB r0, [r1, #0x0]   @ gMapAnimaionWait
0808169A 6830   ldr r0, [r6, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
0808169C 3030   add r0, #0x30
0808169E 7801   ldrb r1, [r0, #0x0]
080816A0 200F   mov r0, #0xF
080816A2 4008   and r0 ,r1
080816A4 280A   cmp r0, #0xA
080816A6 D108   bne #0x80816BA
    080816A8 1C30   mov r0 ,r6
    080816AA 305D   add r0, #0x5D
    080816AC 2100   mov r1, #0x0
    080816AE 5641   LDSB r1, [r0, r1] # pointer:0203E24D
    080816B0 4249   NEG r1 ,r1
    080816B2 4650   mov r0, r10
    080816B4 F000 F910   bl 0x080818D8
    080816B8 E006   b 0x80816C8
080816BA 1C38   mov r0 ,r7
080816BC 305D   add r0, #0x5D
080816BE 2100   mov r1, #0x0
080816C0 5641   LDSB r1, [r0, r1] # pointer:0203E24D
080816C2 1C28   mov r0 ,r5
080816C4 F000 F908   bl 0x080818D8
080816C8 4E17   ldr r6, [PC, #0x5C] # pointer:08081728 -> 0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
080816CA 1C34   mov r4 ,r6
080816CC 345A   add r4, #0x5A
080816CE 8821   ldrh r1, [r4, #0x0] # pointer:0203E24A (MapAnimStruct@gMapAnimStruct.CurrentBattleRound 4byte data [24-31] damage /[23] other bitfield /[22] Not attack /[19-21] other bitfield /[00-15] big bitfield)
080816D0 2080   mov r0, #0x80
080816D2 0040   lsl r0 ,r0 ,#0x1
080816D4 4008   and r0 ,r1
080816D6 2800   cmp r0, #0x0
080816D8 D007   beq #0x80816EA
    080816DA 1C30   mov r0 ,r6
    080816DC 305D   add r0, #0x5D
    080816DE 2100   mov r1, #0x0
    080816E0 5641   LDSB r1, [r0, r1] # pointer:0203E24D
    080816E2 4249   NEG r1 ,r1
    080816E4 4650   mov r0, r10
    080816E6 F000 F8F7   bl 0x080818D8
080816EA 1C30   mov r0 ,r6
080816EC 305D   add r0, #0x5D
080816EE 2200   mov r2, #0x0
080816F0 5682   LDSB r2, [r0, r2] # pointer:0203E24D
080816F2 2A00   cmp r2, #0x0
080816F4 DA00   BGE #0x80816F8
    080816F6 E0E5   b 0x80818C4
080816F8 8821   ldrh r1, [r4, #0x0] # pointer:0203E24A (MapAnimStruct@gMapAnimStruct.CurrentBattleRound 4byte data [24-31] damage /[23] other bitfield /[22] Not attack /[19-21] other bitfield /[00-15] big bitfield)
080816FA 2002   mov r0, #0x2
080816FC 4008   and r0 ,r1
080816FE 2800   cmp r0, #0x0
08081700 D016   beq #0x8081730
    08081702 00AC   lsl r4 ,r5 ,#0x2
    08081704 1964   add r4 ,r4, R5
    08081706 00A4   lsl r4 ,r4 ,#0x2
    08081708 19A4   add r4 ,r4, R6
    0808170A 6820   ldr r0, [r4, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    0808170C 2110   mov r1, #0x10
    0808170E 5641   LDSB r1, [r0, r1]
    08081710 0109   lsl r1 ,r1 ,#0x4
    08081712 4806   ldr r0, [PC, #0x18] # pointer:0808172C -> 0202BCB0 (BattleMapState@gGameState.boolMainLoopEnded )
    08081714 220C   mov r2, #0xC
    08081716 5E80   LDSH r0, [r0, r2] # pointer:0202BCBC (gCurrentRealCameraPos )
    08081718 1A09   sub r1 ,r1, R0
    0808171A 20C8   mov r0, #0xC8
    0808171C F793 FA04   bl 0x08014B28   @ PlaySpacialSoundMaybe
    08081720 6820   ldr r0, [r4, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    08081722 F7FB F9FF   bl 0x0807CB24   @ MapAnim_BeginMISSAnim
    08081726 E0CD   b 0x80818C4
08081728 E1F0 0203   @ ldrDATA
0808172C BCB0 0202   @ ldrDATA
08081730 2A00   cmp r2, #0x0
08081732 D11D   bne #0x8081770
    08081734 2080   mov r0, #0x80
    08081736 0180   lsl r0 ,r0 ,#0x6
    08081738 4008   and r0 ,r1
    0808173A 2800   cmp r0, #0x0
    0808173C D000   beq #0x8081740
        0808173E E0C1   b 0x80818C4
    08081740 4809   ldr r0, [PC, #0x24] # pointer:08081768 -> 000002CE
    08081742 00AC   lsl r4 ,r5 ,#0x2
    08081744 1964   add r4 ,r4, R5
    08081746 00A4   lsl r4 ,r4 ,#0x2
    08081748 19A4   add r4 ,r4, R6
    0808174A 6821   ldr r1, [r4, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    0808174C 7C09   ldrb r1, [r1, #0x10]
    0808174E 0609   lsl r1 ,r1 ,#0x18
    08081750 1609   ASR r1 ,r1 ,#0x18
    08081752 0109   lsl r1 ,r1 ,#0x4
    08081754 4A05   ldr r2, [PC, #0x14] # pointer:0808176C -> 0202BCB0 (BattleMapState@gGameState.boolMainLoopEnded )
    08081756 230C   mov r3, #0xC
    08081758 5ED2   LDSH r2, [r2, r3] # pointer:0202BCBC (gCurrentRealCameraPos )
    0808175A 1A89   sub r1 ,r1, R2
    0808175C F793 F9E4   bl 0x08014B28   @ PlaySpacialSoundMaybe
    08081760 6820   ldr r0, [r4, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    08081762 F7FB FA0D   bl 0x0807CB80   @ MapAnim_BeginNODAMAGEAnim
    08081766 E0AD   b 0x80818C4
08081768 02CE 0000   @ ldrDATA
0808176C BCB0 0202   @ ldrDATA
08081770 2300   mov r3, #0x0
08081772 00AA   lsl r2 ,r5 ,#0x2
08081774 1950   add r0 ,r2, R5
08081776 0080   lsl r0 ,r0 ,#0x2
08081778 1D31   add r1 ,r6, #0x4
0808177A 1840   add r0 ,r0, R1
0808177C 6800   ldr r0, [r0, #0x0] # pointer:0203E1F4
0808177E 3055   add r0, #0x55
08081780 7800   ldrb r0, [r0, #0x0]
08081782 1C14   mov r4 ,r2
08081784 281B   cmp r0, #0x1B
08081786 D001   beq #0x808178C
    08081788 2833   cmp r0, #0x33
    0808178A D100   bne #0x808178E
    0808178C 2301   mov r3, #0x1
0808178E 2B00   cmp r3, #0x0
08081790 D01B   beq #0x80817CA
    08081792 4A08   ldr r2, [PC, #0x20] # pointer:080817B4 -> 0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    08081794 1C10   mov r0 ,r2
    08081796 305C   add r0, #0x5C
    08081798 7801   ldrb r1, [r0, #0x0] # pointer:0203E24C
    0808179A 2002   mov r0, #0x2
    0808179C 4008   and r0 ,r1
    0808179E 2800   cmp r0, #0x0
    080817A0 D00A   beq #0x80817B8
        080817A2 26AF   mov r6, #0xAF
        080817A4 1960   add r0 ,r4, R5
        080817A6 0080   lsl r0 ,r0 ,#0x2
        080817A8 1880   add r0 ,r0, R2
        080817AA 6800   ldr r0, [r0, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
        080817AC 2101   mov r1, #0x1
        080817AE F7FB FA15   bl 0x0807CBDC   @ MapAnim_BeginWallBreakAnim
        080817B2 E022   b 0x80817FA
080817B4 E1F0 0203   @ ldrDATA
    080817B8 26B0   mov r6, #0xB0
    080817BA 1960   add r0 ,r4, R5
    080817BC 0080   lsl r0 ,r0 ,#0x2
    080817BE 1880   add r0 ,r0, R2
    080817C0 6800   ldr r0, [r0, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    080817C2 2100   mov r1, #0x0
    080817C4 F7FB FA0A   bl 0x0807CBDC   @ MapAnim_BeginWallBreakAnim
    080817C8 E017   b 0x80817FA
080817CA 4A05   ldr r2, [PC, #0x14] # pointer:080817E0 -> 0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
080817CC 6810   ldr r0, [r2, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
080817CE 3030   add r0, #0x30
080817D0 7801   ldrb r1, [r0, #0x0]
080817D2 200F   mov r0, #0xF
080817D4 4008   and r0 ,r1
080817D6 280A   cmp r0, #0xA
080817D8 D106   bne #0x80817E8
    080817DA 4E02   ldr r6, [PC, #0x8] # pointer:080817E4 -> 000003C9
    080817DC E00D   b 0x80817FA
    080817DE 0000   nop
080817E0 E1F0 0203   @ ldrDATA
080817E4 03C9 0000   @ ldrDATA
080817E8 1C10   mov r0 ,r2
080817EA 305C   add r0, #0x5C
080817EC 7801   ldrb r1, [r0, #0x0] # pointer:0203E24C
080817EE 2002   mov r0, #0x2
080817F0 4008   and r0 ,r1
080817F2 26D2   mov r6, #0xD2
080817F4 2800   cmp r0, #0x0
080817F6 D000   beq #0x80817FA
    080817F8 26D5   mov r6, #0xD5
080817FA 4F1F   ldr r7, [PC, #0x7C] # pointer:08081878 -> 0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
080817FC 1C38   mov r0 ,r7
080817FE 305A   add r0, #0x5A
08081800 8801   ldrh r1, [r0, #0x0] # pointer:0203E24A (MapAnimStruct@gMapAnimStruct.CurrentBattleRound 4byte data [24-31] damage /[23] other bitfield /[22] Not attack /[19-21] other bitfield /[00-15] big bitfield)
08081802 2001   mov r0, #0x1
08081804 4008   and r0 ,r1
08081806 2800   cmp r0, #0x0
08081808 D03A   beq #0x8081880
    0808180A 1964   add r4 ,r4, R5
    0808180C 00A4   lsl r4 ,r4 ,#0x2
    0808180E 19E0   add r0 ,r4, R7
    08081810 4681   mov r9, r0
    08081812 6800   ldr r0, [r0, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    08081814 2110   mov r1, #0x10
    08081816 5641   LDSB r1, [r0, r1]
    08081818 0109   lsl r1 ,r1 ,#0x4
    0808181A 4A18   ldr r2, [PC, #0x60] # pointer:0808187C -> 0202BCB0 (BattleMapState@gGameState.boolMainLoopEnded )
    0808181C 4690   mov r8, r2
    0808181E 230C   mov r3, #0xC
    08081820 5ED0   LDSH r0, [r2, r3] # pointer:0202BCBC (gCurrentRealCameraPos )
    08081822 1A09   sub r1 ,r1, R0
    08081824 1C30   mov r0 ,r6
    08081826 F793 F97F   bl 0x08014B28   @ PlaySpacialSoundMaybe
    0808182A 1C3E   mov r6 ,r7
    0808182C 3608   add r6, #0x8
    0808182E 19A4   add r4 ,r4, R6
    08081830 6825   ldr r5, [r4, #0x0] # pointer:0203E1F8
    08081832 9C00   ldr r4,[sp, #0x0]
    08081834 4454   add r4, r10
    08081836 00A4   lsl r4 ,r4 ,#0x2
    08081838 1D38   add r0 ,r7, #0x4
    0808183A 1820   add r0 ,r4, R0
    0808183C 6800   ldr r0, [r0, #0x0] # pointer:0A1369D4 -> 00000000
    0808183E 304A   add r0, #0x4A
    08081840 8800   ldrh r0, [r0, #0x0]
    08081842 F7F6 FD29   bl 0x08078298   @ GetSpellAssocFlashColor
    08081846 1C01   mov r1 ,r0
    08081848 0609   lsl r1 ,r1 ,#0x18
    0808184A 0E09   LSR r1 ,r1 ,#0x18
    0808184C 1C28   mov r0 ,r5
    0808184E F7F8 F88F   bl 0x08079970   @ MU_StartCritFlash
    08081852 F7FD FEDF   bl 0x0807F614   @ NewBG0Shaker
    08081856 4649   mov r1, r9
    08081858 6808   ldr r0, [r1, #0x0] # pointer:0203E1F0 (MapAnimStruct@gMapAnimStruct.Data1 )
    0808185A 2110   mov r1, #0x10
    0808185C 5641   LDSB r1, [r0, r1]
    0808185E 0109   lsl r1 ,r1 ,#0x4
    08081860 4642   mov r2, r8
    08081862 230C   mov r3, #0xC
    08081864 5ED0   LDSH r0, [r2, r3] # pointer:0202BCBC (gCurrentRealCameraPos )
    08081866 1A09   sub r1 ,r1, R0
    08081868 20D8   mov r0, #0xD8
    0808186A F793 F95D   bl 0x08014B28   @ PlaySpacialSoundMaybe
    0808186E 19A4   add r4 ,r4, R6
    08081870 6820   ldr r0, [r4, #0x0] # pointer:0A1369D8 -> 00000000
    08081872 F7F8 F867   bl 0x08079944   @ movEUNIT_Begin20FramesFastAnim
    08081876 E025   b 0x80818C4
08081878 E1F0 0203   @ ldrDATA
0808187C BCB0 0202   @ ldrDATA
08081880 1964   add r4 ,r4, R5
08081882 00A4   lsl r4 ,r4 ,#0x2
08081884 19E0   add r0 ,r4, R7
08081886 6800   ldr r0, [r0, #0x0]
08081888 2110   mov r1, #0x10
0808188A 5641   LDSB r1, [r0, r1]
0808188C 0109   lsl r1 ,r1 ,#0x4
0808188E 4811   ldr r0, [PC, #0x44] # pointer:080818D4 -> 0202BCB0 (BattleMapState@gGameState.boolMainLoopEnded )
08081890 220C   mov r2, #0xC
08081892 5E80   LDSH r0, [r0, r2] # pointer:0202BCBC (gCurrentRealCameraPos )
08081894 1A09   sub r1 ,r1, R0
08081896 1C30   mov r0 ,r6
08081898 F793 F946   bl 0x08014B28   @ PlaySpacialSoundMaybe
0808189C 1C38   mov r0 ,r7
0808189E 3008   add r0, #0x8
080818A0 1824   add r4 ,r4, R0
080818A2 6824   ldr r4, [r4, #0x0]
080818A4 9800   ldr r0,[sp, #0x0]
080818A6 4450   add r0, r10
080818A8 0080   lsl r0 ,r0 ,#0x2
080818AA 1D39   add r1 ,r7, #0x4
080818AC 1840   add r0 ,r0, R1
080818AE 6800   ldr r0, [r0, #0x0] # pointer:0A1369D4 -> 00000000
080818B0 304A   add r0, #0x4A
080818B2 8800   ldrh r0, [r0, #0x0]
080818B4 F7F6 FCF0   bl 0x08078298   @ GetSpellAssocFlashColor
080818B8 1C01   mov r1 ,r0
080818BA 0609   lsl r1 ,r1 ,#0x18
080818BC 0E09   LSR r1 ,r1 ,#0x18
080818BE 1C20   mov r0 ,r4
080818C0 F7F8 F8D8   bl 0x08079A74   @ MU_StartHitFlash
080818C4 B001   add sp, #0x4
080818C6 BC38   pop {r3,r4,r5}
080818C8 4698   mov r8, r3
080818CA 46A1   mov r9, r4
080818CC 46AA   mov r10, r5
080818CE BCF0   pop {r4,r5,r6,r7}
080818D0 BC01   pop {r0}
080818D2 4700   BX r0
080818D4 BCB0 0202   @ ldrDATA

