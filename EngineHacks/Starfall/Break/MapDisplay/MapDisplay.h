void Decompress(const void* src, void* dst);
#define OBJ_VRAM0      (void *)(VRAM + 0x10000)
extern u16 gAPMISSAnim[];
extern u8 gGfxBreakAnim[];


enum {
    BM_BGCHR_BANIM_IFBACK = 1,
    BM_BGCHR_BANIM_UNK160 = 0x160,
    BM_BGCHR_BANIM_UNK200 = 0x200,

    BM_OBJCHR_BANIM_EFFECT = 384,
    BM_OBJCHR_BANIM_EFFECT2 = 448,
};
