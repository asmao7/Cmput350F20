#include "OrionBot.h"


// which wall was hit? (used as array index)
enum CollDir { STAGE1 = 0, STAGE2, STAGE3, STAGE4};

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

void OrionBot::BansheeBuild() {
	int current_build = 0;
	switch (current_build) {
		case STAGE1:
			OrionBot::TryBuildSupplyDepot();
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_REFINERY) < 1) {
				if (Observation()->GetMinerals() >= 75) {
					const ObservationInterface* observation = Observation();
					Units workers = observation->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_SCV));
					bool vespene_target = false;
					FindNearestVespeneGeyser(workers.front()->pos);
					while (!vespene_target) {
						vespene_target = FindNearestVespeneGeyser(workers.front()->pos);
					}
				}
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS) < 1) {
				OrionBot::TryBuildBarracks();
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS) > 1) {
			
				OrionBot::TryBuildOrbitalCommand();
				/*if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_SCV) < 25) {
					orbital_upgrade = false;
					OrionBot::TryBuildOrbitalCommand();
				}*/
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_ORBITALCOMMAND) > 0) {
				if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_FACTORY) < 1) {
					OrionBot::TryBuildFactory();
				}
			}
		case STAGE2:
			break;
		case STAGE3:
			break;
		case STAGE4:
			break;
	}
}