
s8 FocusStaffUsability(struct Unit* unit, int item);
s8 IsGeneratedTargetListEmpty(struct Unit* unit, void(*func)(struct Unit*));
void MakeTargetListForFocus(struct Unit* unit);
void TryAddUnitToFocusTargetList(struct Unit* unit);
s8 AreAllegiancesAllied(int left, int right);

enum
{
	// character ID, used for personals 

	ESME_CHAR_ID = 1,

};
