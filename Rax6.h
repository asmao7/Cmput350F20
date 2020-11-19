#include "OrionBot.h"
//Banshee Rush Strategy
//Made by: Joe.

/*
	STAGE1
	10 - Supply Depot
	12 - Refinery
	13 - Barracks
	16 - Orbital Command + Factory

	STAGE2
	16 - Marine (1) + Supply Depot
	20 - Starport + Hellion (1 only)
	@100% Hellion (1) - Tech Lab (1) > Banshees (to 2)
	23 - Refinery (2) + Supply Depot

	STAGE3
	30 - Barracks > Reactor + Supply Depot
	32 - Factory > Tech Lab (2)

	STAGE4
	39 - Siege Tanks + Supply Depot
	52 - Siege Tech > @100% - Attack
*/

void OrionBot::Rax6Build() {
	switch (RAX6_STATE.currentBuild) {
	case STAGE1_RAX6:
		if (Observation()->GetMinerals() >= 100) {
			OrionBot::TryBuildSupplyDepot();
		}
		if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_SUPPLYDEPOT) > 0) {
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_REFINERY) < 1) {
				if (Observation()->GetMinerals() >= 75) {
					const ObservationInterface* observation = Observation();
					Units workers = observation->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_SCV));
					if (!workers.empty()) {
						bool vespene_target = false;
						FindNearestVespeneGeyser(workers.front()->pos);
					}
				}
			}
		}
		if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS) < 1) {
			OrionBot::TryBuildBarracks();
		}
		if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS) > 0) {
			RAX6_STATE.upgradeOrbital = true;
			RAX6_STATE.currentBuild++;
		}
		break;

	case STAGE2_RAX6:
		TryBuildSupplyDepot();
		TryBuildBarracks();
		TryBuildMarine();
	}
}

void OrionBot::Rax6OnUnitIdle(const Unit* unit) {
	switch (unit->unit_type.ToType()) {
	case UNIT_TYPEID::TERRAN_COMMANDCENTER: {
		if (RAX6_STATE.upgradeOrbital) {
			Actions()->UnitCommand(unit, ABILITY_ID::MORPH_ORBITALCOMMAND);
			// call build on choke point
		}
		else {
			Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_SCV);
		}
		break;
	}
	case UNIT_TYPEID::TERRAN_ORBITALCOMMAND: {
		if (unit->energy > 50) {
			const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
			if (!mineral_target) {
				break;
			}
			Actions()->UnitCommand(unit, ABILITY_ID::EFFECT_CALLDOWNMULE);
		}
		break;
	}
	case UNIT_TYPEID::TERRAN_SCV: {
		const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
		// const bool vespene_target = FindNearestVespeneGeyser(unit->pos);
		if (!mineral_target) {
			break;
		}
		if (AddWorkersToRefineries(unit)) {
			break;
		}
		Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
		break;
	}
	case UNIT_TYPEID::TERRAN_BARRACKS: {
		if (BANSHEE_STATE.morph_reactor) {
			Actions()->UnitCommand(unit, ABILITY_ID::BARRACKSREACTORMORPH_REACTOR);
		}
		else {
			Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MARINE);
		}
		
		break;
	}
	
	case UNIT_TYPEID::TERRAN_MARINE: {
		const GameInfo& game_info = Observation()->GetGameInfo();
		Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, game_info.enemy_start_locations.front());
		break;
	}

	case UNIT_TYPEID::TERRAN_REAPER: {
		const GameInfo& game_info = Observation()->GetGameInfo();
		Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, game_info.enemy_start_locations.front());
		break;
	}
	
	default: {
		break;
	}
	}
}




bool OrionBot::TryBuildStructureChokePoint() {
	return false;
}

bool OrionBot::TryBuildSupplyDepotChokePoint() {
	return false;
}

bool OrionBot::TryBuildBarracksChokePoint() {
	return false;
}
