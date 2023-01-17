#define STARFALL_A_RANK 241
#define CA_NO_EXP CA_NEGATE_LETHALITY //incorrectly labeled

extern int (*gGetHPGrowth)(Unit* unit);
extern int (*gGetStrGrowth)(Unit* unit);
extern int (*gGetMagGrowth)(Unit* unit);
extern int (*gGetSklGrowth)(Unit* unit);
extern int (*gGetSpdGrowth)(Unit* unit);
extern int (*gGetDefGrowth)(Unit* unit);
extern int (*gGetResGrowth)(Unit* unit);
extern int (*gGetLukGrowth)(Unit* unit);
extern int (*gExtra_Growth_Boosts)(Unit* unit, int growth, int index);

void CheckBattleUnitStatCaps(struct Unit* unit, struct BattleUnit* bu);
bool HasBattleUnitGainedStats(BattleUnit* battleUnit);
extern u8 Class_Level_Cap_Table[];