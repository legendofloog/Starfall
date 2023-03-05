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

enum{
    ITEM_STAFF_HEAL        = 0x4B,
    ITEM_STAFF_MEND        = 0x4C,
    ITEM_STAFF_RECOVER     = 0x4D,
    ITEM_STAFF_PHYSIC      = 0x4E,
    ITEM_STAFF_FORTIFY     = 0x4F,
    ITEM_STAFF_RESTORE     = 0x50,
    ITEM_STAFF_SILENCE     = 0x51,
    ITEM_STAFF_SLEEP       = 0x52,
    ITEM_STAFF_BERSERK     = 0x53,
    ITEM_STAFF_WARP        = 0x54,
    ITEM_STAFF_RESCUE      = 0x55,
    ITEM_STAFF_TORCH       = 0x56,
    ITEM_STAFF_REPAIR      = 0x57,
    ITEM_STAFF_UNLOCK      = 0x58,
    ITEM_STAFF_BARRIER     = 0x59,
    ITEM_STAFF_SOOTHE      = 0x5A,
    ITEM_VULNERARY         = 0x6C,
    ITEM_ELIXIR            = 0x6D,
};

enum
{
	// Weapon exp needed to have a given weapon level

	NO_RANK = 0,
	E_RANK = 1,
	D_RANK = 61,
	C_RANK = 121,
	B_RANK = 181,
	A_RANK = 241,
	S_RANK = 251,
};

int GetUnitItemHealAmount(struct Unit*, Item item);
