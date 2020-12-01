
#include "OrionBot.h"

// some random

void OrionBot::CombinedBuild() {
	switch (FINALSTRATEGY_STATE.currentBuild) {
		case STAGE1_FINALSTRATEGY: {
			OrionBot::setChokePoint();
			if (Observation()->GetMinerals() >= 100) {
				OrionBot::TryBuildStructureAtCP(ABILITY_ID::BUILD_SUPPLYDEPOT, UNIT_TYPEID::TERRAN_SCV, FINALSTRATEGY_STATE.tobuildSD);
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_SUPPLYDEPOT) > 0) {
				if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_REFINERY) < 1) {
					if (Observation()->GetMinerals() >= 75) {
						const ObservationInterface* observation = Observation();
						Units workers = observation->GetUnits(Unit::Alliance::Self, IsUnit(UNIT_TYPEID::TERRAN_SCV));
						if (!workers.empty()) {
							bool vespene_target = false;
							OrionBot::FindNearestVespeneGeyser(workers.front()->pos);
						}
					}
				}
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS) < 1) {
				OrionBot::TryBuildStructureAtCP(ABILITY_ID::BUILD_BARRACKS, UNIT_TYPEID::TERRAN_SCV, FINALSTRATEGY_STATE.tobuildRaxs);
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_BARRACKS) > 0) {
				FINALSTRATEGY_STATE.upgradeOrbital = true;
			}
			if (OrionBot::CountUnitType(UNIT_TYPEID::TERRAN_ORBITALCOMMAND) > 0) {
				FINALSTRATEGY_STATE.currentBuild++;
			}
			break;
		}
		case STAGE2_FINALSTRATEGY: {
			OrionBot::TryBuildCommandCentreExpansion(ABILITY_ID::BUILD_COMMANDCENTER, UNIT_TYPEID::TERRAN_SCV);
			FINALSTRATEGY_STATE.expand = true;
			if ((FINALSTRATEGY_STATE.newCommandCentre == true)) {
				FINALSTRATEGY_STATE.currentBuild++;
				break;
			}
			break;
		}
		//JUST BUILD MORE SUPPLY DEPOTS AND BARRACKS FOR NOW
		case STAGE3_FINALSTRATEGY: {
			OrionBot::TryBuildSupplyDepot();
			OrionBot::TryBuildBarracks();
			break;
		}
		default: {
			break;
		}
	}
}


/*
 * Set the positions of chike points for SD and rax
 * for each of the four corners.
*/
void OrionBot::setChokePoint() {
	const ObservationInterface* observation = Observation();

	if (observation->GetStartLocation().x == FINALSTRATEGY_STATE.BOTTOM_LEFT.x && observation->GetStartLocation().y == FINALSTRATEGY_STATE.BOTTOM_LEFT.y) {
		FINALSTRATEGY_STATE.tobuildSD = Point2D(29, 54);
		FINALSTRATEGY_STATE.tobuildRaxs = Point2D(29, 51);	//Point2D(30, 51);
		FINALSTRATEGY_STATE.barracks = Point2D(29, 43.5);
	}
	else if (observation->GetStartLocation().x == RAX6_STATE.BOTTOM_RIGHT.x && observation->GetStartLocation().y == RAX6_STATE.BOTTOM_RIGHT.y) {
		FINALSTRATEGY_STATE.tobuildSD = Point2D(138, 29);
		FINALSTRATEGY_STATE.tobuildRaxs = Point2D(140, 29);		//Point2D(140, 30);
		FINALSTRATEGY_STATE.barracks = Point2D(148, 29);
	}
	else if (observation->GetStartLocation().x == FINALSTRATEGY_STATE.TOP_LEFT.x && observation->GetStartLocation().y == FINALSTRATEGY_STATE.TOP_LEFT.y) {
		FINALSTRATEGY_STATE.tobuildSD = Point2D(51, 160);
		FINALSTRATEGY_STATE.tobuildRaxs = Point2D(51, 162);		//Point2D(52, 162);
		FINALSTRATEGY_STATE.barracks = Point2D(148, 160);
	}
	else if (observation->GetStartLocation().x == FINALSTRATEGY_STATE.TOP_RIGHT.x && observation->GetStartLocation().y == FINALSTRATEGY_STATE.TOP_RIGHT.y) {
		FINALSTRATEGY_STATE.tobuildSD = Point2D(160, 141);
		FINALSTRATEGY_STATE.tobuildRaxs = Point2D(162, 140);
		FINALSTRATEGY_STATE.barracks = Point2D(160, 148);
	}
}
