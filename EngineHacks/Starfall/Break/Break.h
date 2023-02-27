//stuff i need

void ClearBattleHits();
void BattleGetBattleUnitOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender);
s8 BattleGetFollowUpOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
int GetBattleUnitHitCount(struct BattleUnit* attacker);
s8 BattleGenerateHit(struct BattleUnit* attacker, struct BattleUnit* defender);
s8 BattleRoll1RN(u16 threshold, s8 simResult);
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

#define SKILLSTATE_BREAK (1 << 2)
#define SKILLSTATE_BROKEN_IN_BATTLE (1 << 3)
#define BATTLE_HIT_BREAK BATTLE_HIT_ATTR_12 // (1 << 17)

typedef struct BreakEntry BreakEntry;

struct BreakEntry
{
	u8 breakerWType;
	u8 brokenWType;
};

extern struct BreakEntry BreakTargetTable[];

extern u8 BreakExemptCharacterList[];
extern u8 BreakExemptClassList[];

//break stuff