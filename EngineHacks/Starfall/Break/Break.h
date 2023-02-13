//stuff i need

void ClearBattleHits();
void BattleGetBattleUnitOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender);
s8 BattleGetFollowUpOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
int GetBattleUnitHitCount(struct BattleUnit* attacker);
s8 BattleGenerateHit(struct BattleUnit* attacker, struct BattleUnit* defender);
s8 BattleRoll1RN(u16 threshold, s8 simResult);

#define SKILLSTATE_BREAK (1 << 2)
#define SKILLSTATE_BROKEN_IN_BATTLE (1 << 3)

typedef struct BreakEntry BreakEntry;

struct BreakEntry
{
	u8 breakerWType;
	u8 brokenWType;
};

extern struct BreakEntry BreakTargetTable[];

//break stuff