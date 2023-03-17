#include "gbafe.h"

#include "NewCalculations.h"

bool CanBattleUnitGainExp(BattleUnit* actor, BattleUnit* target);
bool CanBattleUnitGainLevels(BattleUnit* battleUnit);
int GetLevelDifference(BattleUnit* actor, BattleUnit* target);
int GetNPCStatIncrease(int growth);


int GetCurrentPromotedLevelBonus(){
    return 0; //promo'd units don't get SHIT
}

void ComputeBattleUnitHitRate(BattleUnit* bu) {
    bu->battleHitRate = (bu->unit.skl * 3) + GetItemHit(bu->weapon);
}

void ComputeBattleUnitAvoidRate(BattleUnit* bu) {
    bu->battleAvoidRate = bu->terrainAvoid + (bu->unit.lck * 3);

    if (bu->battleAvoidRate < 0){
        bu->battleAvoidRate = 0;
    }
}

void ComputeBattleUnitCritRate(struct BattleUnit* bu) {
	if (GetItemCrit(bu->weapon) != 255){
		bu->battleCritRate = GetItemCrit(bu->weapon);
	}
    else{
		bu->battleCritRate = 0;
	}
}

void ComputeBattleUnitEffectiveCritRate(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int i;
	Item item;

    attacker->battleEffectiveCritRate = attacker->battleCritRate;

    if (attacker->battleEffectiveCritRate < 0)
        attacker->battleEffectiveCritRate = 0;
	
    for (i = 0; (i < UNIT_ITEM_COUNT); ++i) {
		item = defender->unit.items[i];
        if (GetItemAttributes(item) & IA_NEGATE_CRIT) {
            attacker->battleEffectiveCritRate = 0;
            break;
        }
    }
}


void ComputeBattleUnitSpeed(BattleUnit* bu) {
    int effWt = GetItemWeight(bu->weaponBefore);

    bu->battleSpeed = bu->unit.spd - effWt;

    if (bu->battleSpeed < 0)
        bu->battleSpeed = 0;
}


int GetBattleUnitExpGain(BattleUnit* actor, BattleUnit* target){
	
	if (CanBattleUnitGainExp(actor, target)){

		// tinked or missed
		if (!(actor->nonZeroDamage)){
			return 0;
		}

		// killed
		if (target->unit.curHP == 0){
			int bossFactor = 1;
			if (target->unit.pCharacterData->attributes & CA_BOSS){
				bossFactor = 2;
			}
			int initialKillExp = (30 + (6 * GetLevelDifference(actor, target)) )*bossFactor;
			if(initialKillExp <= 2){
				return 2;
			}
			else if(initialKillExp >= 100){
				return 100;
			}
			else{
				return initialKillExp;
			}
		}

		// hit
		int initialHitExp = 15 + (3 * GetLevelDifference(actor, target));
			if(initialHitExp <= 1){
				return 1;
			}
			else if(initialHitExp >= 50){
				return 50;
			}
			else{
				return initialHitExp;
			}

	}

	return 0;
}

bool CanBattleUnitGainExp(BattleUnit* actor, BattleUnit* target){

	// is the unit exp maxed
	if (!CanBattleUnitGainLevels(actor)){
		return false;
	}

	// is the unit alive
	if (actor->unit.curHP == 0){
		return false;
	}

	// does the opponent prevent exp gain
	if (((target->unit.pCharacterData->attributes) | (target->unit.pClassData->attributes)) & CA_NO_EXP){
		return false;
	}

	return true;

}



int GetUnitEffectiveLevel(Unit* unit){

	int effectiveLevel = unit->level; // simple for now; no effective level shenanigans, what you see is what you get
	return effectiveLevel;

}

int GetLevelDifference(BattleUnit* actor, BattleUnit* target){

	int diff = GetUnitEffectiveLevel(&target->unit) - GetUnitEffectiveLevel(&actor->unit);
	return diff;

}

bool CanBattleUnitGainLevels(BattleUnit* battleUnit) {
    if (battleUnit->unit.exp == 0xFF){
    	return false;
    }

    if (UNIT_FACTION(&battleUnit->unit) != FACTION_BLUE){
    	return false;
    }

    return true;
}

int GetBattleUnitStaffExp(BattleUnit* actor){
	if (!CanBattleUnitGainLevels(actor)){
		return 0;
	}

	// is the unit alive
	if (actor->unit.curHP == 0){
		return 0;
	}

	const ItemData* staffData = GetItemData(actor->weapon.number);
	int staffRank = staffData->weaponRank;
	if( staffRank == E_RANK ){ // e rank
		return 20;
	}
	else if( staffRank == D_RANK ){ // d rank
		return 25;
	}
	else if( staffRank == C_RANK){ // c rank
		return 30;
	} 
	else if( staffRank == B_RANK){ // b rank
		return 35;
	}
	else if( staffRank == A_RANK ){ // a rank
		return 40;
	}
	else{ // s rank
		return 45; // no s rank so this doesn't do anything lol
	}
}

void ComputeBattleUnitWeaponRankBonuses(struct BattleUnit* bu) {
    return;
}

int GetUnitItemHealAmount(struct Unit* unit, Item item) {
    int result = 0;

    switch (GetItemIndex(item)) {

    case ITEM_STAFF_SOOTHE:
		result = 5;
		break;

	case ITEM_STAFF_HEAL:
    case ITEM_STAFF_PHYSIC:
    case ITEM_STAFF_FORTIFY:
        result = 10;
        break;

	case ITEM_VULNERARY:
		result = 15;
		break;

    case ITEM_STAFF_MEND:
        result = 20;
        break;

    case ITEM_ELIXIR:
        result = 80;
        break;

    } // switch (GetItemIndex(item))

    if (GetItemAttributes(item) & IA_STAFF) {
        result += GetUnitPower(unit);

        if (result > 80)
            result = 80;
    }

    return result;
}

void ApplyUnitPromotion(Unit* unit, u8 classId) {
    int i;
    const ClassData* currentClass = unit->pClassData;
    const ClassData* promotedClass = GetClassData(classId);
    // Apply stat ups

    unit->maxHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->maxHP += (promotedClass->basePow - currentClass->basePow);

    if (unit->pow > promotedClass->maxPow){
		unit->pow = promotedClass->maxPow;
	}

	unit->mag += (MagClassTable[promotedClass->number].promotionMag - MagClassTable[currentClass->number].promotionMag);

	if (unit->mag > MagClassTable[promotedClass->number].maxMag){
		unit->mag = MagClassTable[promotedClass->number].maxMag;
	}
	
    unit->skl += (promotedClass->baseSkl - currentClass->baseSkl);

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += (promotedClass->baseSpd - currentClass->baseSpd);

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += (promotedClass->baseDef - currentClass->baseDef);

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += (promotedClass->baseRes - currentClass->baseRes);

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    
    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > STARFALL_A_RANK)
            wexp = STARFALL_A_RANK;

        unit->ranks[i] = wexp;
    }

    //unit->level = 1;
    //unit->exp   = 0; //do not reset level

    unit->curHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}

int GetBattleUnitUpdatedWeaponExp(BattleUnit* battleUnit) {
	return -1; //this makes wexp not update from battles; static ranks
}

void CheckBattleUnitLevelUp( BattleUnit* battleUnit) {
    if (CanBattleUnitGainLevels(battleUnit) && battleUnit->unit.exp >= 100) { //want to make a random, fixed, 0% mode

        battleUnit->unit.level++;
		
		if (battleUnit->unit.level == Class_Level_Cap_Table[battleUnit->unit.pClassData->number]){
			battleUnit->expGain -= (battleUnit->unit.exp - 100);
			battleUnit->unit.exp = 0xFF;
		}
		else {
			battleUnit->unit.exp -= 100;
		}

		Unit* levelingUnit = GetUnitByCharId(battleUnit->unit.pCharacterData->number);

		if (levelingUnit->maxHP < battleUnit->unit.pClassData->maxHP){
			battleUnit->changeHP  = GetStatIncrease(gGetHPGrowth(&battleUnit->unit));
		}

		if (levelingUnit->pow < battleUnit->unit.pClassData->maxPow){
			battleUnit->changePow = GetStatIncrease(gGetStrGrowth(&battleUnit->unit));
		}

		if (levelingUnit->mag < MagClassTable[battleUnit->unit.pClassData->number].maxMag){
			battleUnit->changeMag = GetStatIncrease(gGetMagGrowth(&battleUnit->unit));
		}

		if (levelingUnit->skl < battleUnit->unit.pClassData->maxSkl){
			battleUnit->changeSkl = GetStatIncrease(gGetSklGrowth(&battleUnit->unit));
		}

		if (levelingUnit->spd < battleUnit->unit.pClassData->maxSpd){
			battleUnit->changeSpd = GetStatIncrease(gGetSpdGrowth(&battleUnit->unit));
		}

		if (levelingUnit->def < battleUnit->unit.pClassData->maxDef){
			battleUnit->changeDef = GetStatIncrease(gGetDefGrowth(&battleUnit->unit));
		}

		if (levelingUnit->res < battleUnit->unit.pClassData->maxRes){
			battleUnit->changeRes = GetStatIncrease(gGetResGrowth(&battleUnit->unit));
		}

		if (levelingUnit->lck < battleUnit->unit.pClassData->maxLck){ //universal cap? kinda want to make it 20 for unpromo'd, 30 for promo'd like the rest
			battleUnit->changeLck = GetStatIncrease(gGetLukGrowth(&battleUnit->unit));
		}

        CheckBattleUnitStatCaps(GetUnit(battleUnit->unit.index), battleUnit);
    }
}

bool HasBattleUnitGainedStats(BattleUnit* battleUnit){
	if (battleUnit->changeHP || battleUnit->changePow || battleUnit->changeMag || battleUnit->changeSkl || battleUnit->changeSpd || battleUnit->changeDef || battleUnit->changeRes || battleUnit->changeLck || battleUnit->changeMov || battleUnit->changeCon){
		return true;
	}
	return false;
}

void CheckBattleUnitStatCaps(struct Unit* unit, struct BattleUnit* bu) {
    if ((unit->maxHP + bu->changeHP) > UNIT_MHP_MAX(unit)){
		bu->changeHP = UNIT_MHP_MAX(unit) - unit->maxHP;
	}
        
    if ((unit->pow + bu->changePow) > UNIT_POW_MAX(unit)){
		bu->changePow = UNIT_POW_MAX(unit) - unit->pow;
	}
        
	if ((unit->mag + bu->changeMag) > MagClassTable[bu->unit.pClassData->number].maxMag){
		bu->changeMag = MagClassTable[bu->unit.pClassData->number].maxMag - unit->mag;
	}
		
    if ((unit->skl + bu->changeSkl) > UNIT_SKL_MAX(unit)){
		bu->changeSkl = UNIT_SKL_MAX(unit) - unit->skl;
	}

    if ((unit->spd + bu->changeSpd) > UNIT_SPD_MAX(unit)){
		bu->changeSpd = UNIT_SPD_MAX(unit) - unit->spd;
	}
        

    if ((unit->def + bu->changeDef) > UNIT_DEF_MAX(unit)){
		bu->changeDef = UNIT_DEF_MAX(unit) - unit->def;
	} 

    if ((unit->res + bu->changeRes) > UNIT_RES_MAX(unit)){
		bu->changeRes = UNIT_RES_MAX(unit) - unit->res;
	}
        
    if ((unit->lck + bu->changeLck) > UNIT_LCK_MAX(unit)){
		bu->changeLck = UNIT_LCK_MAX(unit) - unit->lck;
	}
        
}

// makes autolevels fixed
int GetAutoleveledStatIncrease(int growth, int levelCount) {
    return GetNPCStatIncrease(growth * (levelCount + 1));
}

int GetNPCStatIncrease(int growth){
	int result = 0;
	
	while (growth >= 100) {
        result++;
        growth -= 100;
    }

	return result;
}

//FE4 weapon rank thing (built to save weapon rank data for other things, perhaps); also con bonus is meaningless
//Notes: remove UnitHasMagicRank from wherever it's used

void UnitLoadStatsFromChracter(struct Unit* unit, const struct CharacterData* character) {
    int i;

    unit->maxHP = character->baseHP + unit->pClassData->baseHP;
    unit->pow   = character->basePow + unit->pClassData->basePow;
	unit->mag   = MagCharTable[character->number].baseMag + MagClassTable[unit->pClassData->number].baseMag;
    unit->skl   = character->baseSkl + unit->pClassData->baseSkl;
    unit->spd   = character->baseSpd + unit->pClassData->baseSpd;
    unit->def   = character->baseDef + unit->pClassData->baseDef;
    unit->res   = character->baseRes + unit->pClassData->baseRes;
    unit->lck   = character->baseLck + unit->pClassData->baseLck;

    unit->conBonus = 0;

    for (i = 0; i < 8; ++i) {
        unit->ranks[i] = unit->pClassData->baseRanks[i];

        if (unit->pCharacterData->baseRanks[i])
            unit->ranks[i] = unit->pCharacterData->baseRanks[i];
    }

    if (UNIT_FACTION(unit) == FACTION_BLUE && (unit->level != UNIT_LEVEL_MAX))
        unit->exp = 0;
    else
        unit->exp = UNIT_EXP_DISABLED;
}