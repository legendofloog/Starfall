#include "gbafe.h"

#include "NewItemEffects.h"

s8 FocusStaffUsability(struct Unit* unit, int item){
	if (unit->pCharacterData->number != ESME_CHAR_ID){
		return FALSE;
	}
	return true;
}

void MakeTargetListForFocus(struct Unit* unit){
    int x = unit->xPos;
    int y = unit->yPos;

    gActiveUnit = unit;

    InitTargets(x, y);

    BmMapFill(gMapRange, 0);

    MapAddInRange(x, y, 5, 1); //focus is 5 range

    ForEachUnitInRange(TryAddUnitToFocusTargetList);

    return;
}

void TryAddUnitToFocusTargetList(struct Unit* unit){

    if (!AreAllegiancesAllied(gActiveUnit->index, unit->index)) {
        return;
    }

    if (unit->state & US_RESCUED) {
        return;
    }

    AddTarget(unit->xPos, unit->yPos, unit->index, 0);

    return;
}
