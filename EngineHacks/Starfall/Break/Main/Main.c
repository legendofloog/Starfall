#include  "Main.h"

bool DidUnitBreak(){
	if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BREAK){
		return false;
	}
	int j = 0;
	while (BreakExemptCharacterList[j] != 0){
		if (BreakExemptCharacterList[j] == gBattleTarget.unit.pCharacterData->number){
			return false; //if target's char ID is on exempt list, no break allowed
		}
		j++;
	}
	int k = 0;
	while (BreakExemptClassList[k] != 0){
		if (BreakExemptClassList[k] == gBattleTarget.unit.pClassData->number){
			return false; //if target's class ID is on exempt list, no break allowed
		}
		k++;
	}
	if ((gBattleActor.battleAttack > gBattleTarget.battleDefense)){ //did unit do damage
		int i = 0;
		while (BreakTargetTable[i].breakerWType != 0xFF){
			if ((BreakTargetTable[i].breakerWType == GetItemType(gBattleActor.weaponBefore)) && (BreakTargetTable[i].brokenWType == GetItemType(gBattleTarget.weaponBefore))){
				
				return true;
			}
			i++;
		}
	}
	return false;
}

void BreakPostBattle(){

	// ignore non combat actions
	if (gActionData.unitActionType != UNIT_ACTION_COMBAT){
		return;
	}

	Unit* target = &gBattleTarget.unit;

	// unset aftershock
	bool alreadyBroken = false;
	if (gDebuffTable[target->index].skillState & SKILLSTATE_BREAK){
		gDebuffTable[target->index].skillState &= ~SKILLSTATE_BREAK;
		alreadyBroken = true;
	}

	// ignore dead people
	if (target->curHP <= 0){
		return;
	}

	// make sure the enemy is real
	if (!target->pClassData){
		return;
	}

	// try to apply aftershock
	if(DidUnitBreak() & (!alreadyBroken)){
		gDebuffTable[target->index].skillState |= SKILLSTATE_BREAK;
	}
}

void ClearActiveFactionBreakStatus(){
	Unit* unit;
	int i;
	for (i = gChapterData.currentPhase + 1; i < gChapterData.currentPhase + 0x40; i++) {
        unit = GetUnit(i);

        if (UNIT_IS_VALID(unit)){
			gDebuffTable[unit->index].skillState &= ~SKILLSTATE_BREAK; //undo break status for current actors
		} 
    }
}