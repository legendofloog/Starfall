#include "gbafe.h"
#include "Debuff.h"
#include "Break.h"

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

void BattleUnwind(void) {
    ClearBattleHits();

    // this do { ... } while (0); is required for match
    // which is kind of neat because it implies scrapped plans for supporting some accost kind of thing
    do {
        struct BattleUnit* attacker;
        struct BattleUnit* defender;
        BattleGetBattleUnitOrder(&attacker, &defender);

        gBattleHitIterator->info |= BATTLE_HIT_INFO_BEGIN;

        if (!BattleGenerateRoundHits(attacker, defender)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_RETALIATE;

            if (!BattleGenerateRoundHits(defender, attacker) && BattleGetFollowUpOrder(&attacker, &defender)) {
                gBattleHitIterator->attributes = BATTLE_HIT_ATTR_FOLLOWUP;

                BattleGenerateRoundHits(attacker, defender);
            }
        }
    } while (FALSE);
	gDebuffTable[gBattleTarget.unit.index].skillState &= ~SKILLSTATE_BROKEN_IN_BATTLE; //gets rid of mid battle break
    gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
}

s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int i, count;
    u16 attrs; // NOTE: this is a bug! attrs are 19 bits in FE8 (they're 16 bits in previous games)

    if (!(attacker->weapon.number) && !(attacker->weapon.durability)){
		return FALSE;
	}
        

	if (gDebuffTable[attacker->unit.index].skillState & SKILLSTATE_BROKEN_IN_BATTLE){
		return FALSE;
	}

    attrs = gBattleHitIterator->attributes;
    count = GetBattleUnitHitCount(attacker);

    for (i = 0; i < count; ++i) {
        gBattleHitIterator->attributes |= attrs;

        if (BattleGenerateHit(attacker, defender))
            return TRUE;
    }

    return FALSE;
}

void BattleGenerateHitEffects(struct BattleUnit* attacker, struct BattleUnit* defender) {
    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)) {
        if (DidUnitBreak()){
			if (gDebuffTable[defender->unit.index].skillState & SKILLSTATE_BROKEN_IN_BATTLE){

			}
			else{
				gDebuffTable[gBattleTarget.unit.index].skillState |= SKILLSTATE_BROKEN_IN_BATTLE;
				gBattleHitIterator->attributes |= BATTLE_HIT_BREAK;
			}
		}
		
		switch (GetItemWeaponEffect(attacker->weapon)) {

        case WPN_EFFECT_POISON:
            // Poison defender

            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;

            break;

        case WPN_EFFECT_HPHALVE:
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            break;

        } // switch (GetItemWeaponEffect(attacker->weapon))

    	if ((GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL) && (BattleRoll1RN(31 - attacker->unit.lck, FALSE))) {
        	gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;

        	attacker->unit.curHP -= gBattleStats.damage;

        	if (attacker->unit.curHP < 0)
            	attacker->unit.curHP = 0;
    		}
		else {
        	if (gBattleStats.damage > defender->unit.curHP)
            	gBattleStats.damage = defender->unit.curHP;

        	defender->unit.curHP -= gBattleStats.damage;

        	if (defender->unit.curHP < 0)
            	defender->unit.curHP = 0;
    	}

        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
        }

            if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY) {
                switch (gChapterData.currentPhase) {

                case FACTION_BLUE:
                    if (UNIT_FACTION(&defender->unit) == FACTION_BLUE)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_RED:
                    if (UNIT_FACTION(&defender->unit) == FACTION_RED)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_GREEN:
                    if (UNIT_FACTION(&defender->unit) == FACTION_GREEN)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                } // switch (gRAMChapterData.faction)

                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
            }
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) || attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC)) {
        attacker->weapon = GetItemAfterUse(attacker->weapon);

        if (!(attacker->weapon.number) && !(attacker->weapon.durability))
            attacker->weaponBroke = TRUE;
    }
}


void New_BattleInitTargetCanCounter(){;

	// eggs
	if (gBattleTarget.unit.pClassData->number == 0x34 || gBattleTarget.unit.pClassData->number == 0x62){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// attacker weapon is uncounterable
	if (gBattleActor.weaponAttributes & IA_UNCOUNTERABLE){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// target weapon is uncounterable
	if (gBattleTarget.weaponAttributes & IA_UNCOUNTERABLE){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// attacker is berserked and both units are blue
	if ((gBattleActor.unit.statusIndex == UNIT_STATUS_BERSERK) && (gBattleActor.unit.index & FACTION_BLUE) && (gBattleTarget.unit.index & FACTION_BLUE)){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// defender is broken
	if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BREAK){
		gBattleTarget.weapon.number = 0;
		gBattleTarget.weapon.durability = 0;
		gBattleTarget.canCounter = false;
		return;
	}

}

void MapAnim_BeginBREAKAnim(struct Unit* unit)
{
    Decompress(
        gGfxBreakAnim,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

    APProc_Create(
        gAPMISSAnim,
        16*(unit->xPos - (gGameState.cameraRealPos.x>>4)) + 8,
        16*(unit->yPos - (gGameState.cameraRealPos.y>>4)) + 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);
}

s8 CanUnitUseWeapon(Unit* unit, Item item) {
    if (item.number == 0 && item.durability == 0){
		return FALSE;
	}

	if (gDebuffTable[unit->index].skillState & SKILLSTATE_BREAK){
		return FALSE;
	}

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return FALSE;

    if (GetItemAttributes(item) & IA_LOCK_ANY) {
        // Check for item locks

        if ((GetItemAttributes(item) & IA_LOCK_1) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_1))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_4) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_4))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_5) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_5))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_6) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_6))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_7) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_7))
            return FALSE;

        if ((GetItemAttributes(item) & IA_LOCK_2) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_2))
            return FALSE;

        // Monster lock is special
        if (GetItemAttributes(item) & IA_LOCK_3) {
            if (!(UNIT_CATTRIBUTES(unit) & CA_LOCK_3))
                return FALSE;

            return TRUE;
        }

        if (GetItemAttributes(item) & IA_UNUSABLE)
            if (!(IsItemUnsealedForUnit(unit, item)))
                return FALSE;
    }

    if ((unit->statusIndex == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC)){
		return FALSE;
	}
        
    int wRank = GetItemRequiredExp(item);
    int uRank = (unit->ranks[GetItemType(item)]);

    return (uRank >= wRank) ? TRUE : FALSE;
}

void ComputeAiAttackWeight(struct AiCombatSimulationSt* st) {
    int score;
    int backup;

    score = AiBattleGetDamageDealtWeight(); //max score: 110
    backup = score;

    score += AiBattleGetTargetHpWeight(); //max score: 60
    //score += AiBattleGetAlliesAroundWeight(); //max score: 10
    //score += AiGetTargetClassCombatScoreComponent(); does nothing in fe8
    //score += AiGetTurnCombatScoreComponent(); stupid and bad (adds turns to combat weight)
    score -= AiBattleGetDamageTakenWeight(); //minimum score: -110
    //score -= AiBattleGetTileDangerWeight(); //reduces priority if they attack into more enemies? minimum -20
    score -= AiBattleGetSubjectHpWeight(); //minimum score: -60

    if (score < 0) {
        score = 0;
    }

    if (DidUnitBreak() || gBattleActor.battleEffectiveHitRate > 0){
        score = (145 - ( 100 - gBattleActor.battleEffectiveHitRate)); //make enemies always want to break, unless they can kill (110 + 60) or hitrates are bad
    }
    if (gBattleTarget.weapon.number == 0 && gBattleTarget.weapon.durability == 0) {
        score = (160 - ( 100 - gBattleActor.battleEffectiveHitRate)); //guaranteed no counter slightly higher priority than break
    }

    if (score != 0) {
        score = score * 10;
    } else {
        score = backup;
    }

    st->score = score;

    return;
}

int AiBattleGetDamageDealtWeight(void) {
    int score;
    
    if (gBattleTarget.unit.curHP == 0) {
        return 110; //if enemy dies and actor doesn't, maximum value considered
        //idk how the simulation works and if units can both be dead, but just in case!
    }

    if ((gBattleActor.battleAttack - gBattleTarget.battleDefense) <= 0){
        return 0; //if do no damage, consider nothing
    }

    if (gBattleActor.battleEffectiveHitRate == 0){
        return 0; //if guaranteed miss, consider nothing
    }

    score = (gBattleActor.battleAttack - gBattleTarget.battleDefense) + (gBattleActor.battleEffectiveHitRate / 5);
    //one point of damage = five points of hit for priority (ex: 20 damage and 70 hit adds up to score of 34)

    if (gBattleActor.unit.curHP > 0 || gBattleTarget.battleEffectiveHitRate == 0){ //applies if unit doesn't die on counter
        if (gBattleActor.battleSpeed - gBattleTarget.battleSpeed >= BATTLE_FOLLOWUP_SPEED_THRESHOLD){
            score *= 2; //if actor follows up, score doubled
        }
        if (gBattleActor.weaponAttributes & IA_BRAVE){
            score *= 2; //if actor braves, score doubled
        }
    }

    if (score < 0) {
        score = 0;
    }

    if (score > 100) { 
        score = 100; //cap for non-kills is 100
    }

    return score;
}

int AiBattleGetTargetHpWeight(void) {
    int score;

    score = 60 - gBattleTarget.unit.curHP; //hp cap - cur hp

    if (score < 0) {
        score = 0;
    }

    return score;
}

int AiBattleGetDamageTakenWeight(void) {
    int score;

    if (gBattleTarget.weapon.number == 0 && gBattleTarget.weapon.durability == 0) {
        return 0; //they have no weapon, so no counter
    }

    if ((gBattleTarget.battleAttack - gBattleActor.battleDefense) <= 0){
        return 0; //no damage, no fear
    }

    if (DidUnitBreak() && gBattleActor.battleEffectiveHitRate > 0 ){
        return 0; //if unit would break in this combat, they believe they face no counter
    }

    if (gBattleActor.unit.curHP == 0){
        return 110; //if the unit dies in the combat, lowered priority
    } 
    
    score = (gBattleTarget.battleAttack - gBattleActor.battleDefense) + (gBattleTarget.battleEffectiveHitRate / 5);
    if (gBattleTarget.unit.curHP > 0 || gBattleActor.battleEffectiveHitRate == 0){ //applies if target doesn't die on counter
        if (gBattleTarget.battleSpeed - gBattleActor.battleSpeed >= BATTLE_FOLLOWUP_SPEED_THRESHOLD){
            score *= 2; //if target follows up, score doubled
        }
        if (gBattleTarget.weaponAttributes & IA_BRAVE){
            score *= 2; //if target braves, score doubled
        }
    }

    if (score < 0) {
        score = 0;
    }

    if (score > 100) {
        score = 100;
    }

    return score;
}

int AiBattleGetSubjectHpWeight(void) {
    int score;

    score = 60 - gBattleActor.unit.curHP; //hp cap - cur hp

    if (score < 0) {
        score = 0;
    }

    return score;
}