#include "TargetingAI.h"

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