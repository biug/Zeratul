#pragma once

#include "sc2api/sc2_api.h"
#include "Common.h"

class Zeratul;

namespace Util
{
    struct IsUnit 
    {
        sc2::UNIT_TYPEID m_type;

        IsUnit(sc2::UNIT_TYPEID type);
        bool operator()(const sc2::Unit * unit, const sc2::ObservationInterface*);
    };

    int GetPlayer(const sc2::Unit * unit);
	bool IsPsionicUnit(const sc2::Unit * unit);
	bool hasReactor(const sc2::Unit * unit, Zeratul &bot);
    bool IsCombatUnit(const sc2::Unit * unit, Zeratul & bot);
    bool IsCombatUnitType(const sc2::UnitTypeID & type, Zeratul & bot);
    bool IsSupplyProvider(const sc2::Unit * unit);
    bool IsSupplyProviderType(const sc2::UnitTypeID & type);
    bool IsTownHall(const sc2::Unit * unit);
    bool IsTownHallType(const sc2::UnitTypeID & type);
    bool IsRefinery(const sc2::Unit * unit);
    bool IsRefineryType(const sc2::UnitTypeID & type);
    bool IsDetector(const sc2::Unit * type);
    bool IsDetectorType(const sc2::UnitTypeID & type);
    bool IsGeyser(const sc2::Unit * unit);
    bool IsMineral(const sc2::Unit * unit);
    bool IsWorker(const sc2::Unit * unit);
    bool IsWorkerType(const sc2::UnitTypeID & unit);
    bool IsIdle(const sc2::Unit * unit);
    bool IsCompleted(const sc2::Unit * unit);
    float GetAttackRange(const sc2::UnitTypeID & type, Zeratul & bot);
    
    bool UnitCanBuildTypeNow(const sc2::Unit * unit, const sc2::UnitTypeID & type, Zeratul & m_bot);
    int GetUnitTypeWidth(const sc2::UnitTypeID type, const Zeratul & bot);
    int GetUnitTypeHeight(const sc2::UnitTypeID type, const Zeratul & bot);
    int GetUnitTypeMineralPrice(const sc2::UnitTypeID type, const Zeratul & bot);
    int GetUnitTypeGasPrice(const sc2::UnitTypeID type, const Zeratul & bot);
    sc2::UnitTypeID GetTownHall(const sc2::Race & race);
    sc2::UnitTypeID GetSupplyProvider(const sc2::Race & race);
    std::string     GetStringFromRace(const sc2::Race & race);
    sc2::Race       GetRaceFromString(const std::string & race);
    sc2::Point2D    CalcCenter(const std::vector<const sc2::Unit *> & units);

    sc2::UnitTypeID GetUnitTypeIDFromName(const std::string & name, Zeratul & bot);
    sc2::UpgradeID  GetUpgradeIDFromName(const std::string & name, Zeratul & bot);
    sc2::BuffID     GetBuffIDFromName(const std::string & name, Zeratul & bot);
    sc2::AbilityID  GetAbilityIDFromName(const std::string & name, Zeratul & bot);

    float Dist(const sc2::Point2D & p1, const sc2::Point2D & p2);
    float DistSq(const sc2::Point2D & p1, const sc2::Point2D & p2);
    
    // Kevin-provided helper functions
    void    VisualizeGrids(const sc2::ObservationInterface* obs, sc2::DebugInterface* debug);
    float   TerainHeight(const sc2::GameInfo& info, const sc2::Point2D& point);
    bool    Placement(const sc2::GameInfo& info, const sc2::Point2D& point);
    bool    Pathable(const sc2::GameInfo& info, const sc2::Point2D& point);

	const sc2::Unit*	getClosestPylon(Zeratul & bot);

	template<typename P1, typename P2>
	float PlanerDist(const P1 & p1, const P2 & p2)
	{
		return fabs(p1.x - p2.x) + fabs(p1.y - p2.y);
	}
};
