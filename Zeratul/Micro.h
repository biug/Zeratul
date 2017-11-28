#pragma once

#include "Common.h"
#include "sc2api/sc2_api.h"

class Zeratul;

namespace Micro
{   
    void SmartStop          (const sc2::Unit * attacker,  Zeratul & bot);
    void SmartAttackUnit    (const sc2::Unit * attacker,  const sc2::Unit * target, Zeratul & bot);
    void SmartAttackMove    (const sc2::Unit * attacker,  const sc2::Point2D & targetPosition, Zeratul & bot);
    void SmartMove          (const sc2::Unit * attacker,  const sc2::Point2D & targetPosition, Zeratul & bot);
    void SmartRightClick    (const sc2::Unit * unit,      const sc2::Unit * target, Zeratul & bot);
    void SmartRepair        (const sc2::Unit * unit,      const sc2::Unit * target, Zeratul & bot);
    void SmartKiteTarget    (const sc2::Unit * rangedUnit,const sc2::Unit * target, Zeratul & bot);
    void SmartBuild         (const sc2::Unit * builder,   const sc2::UnitTypeID & buildingType, sc2::Point2D pos, Zeratul & bot);
    void SmartBuildTarget   (const sc2::Unit * builder,   const sc2::UnitTypeID & buildingType, const sc2::Unit * target, Zeratul & bot);
    void SmartTrain         (const sc2::Unit * builder,   const sc2::UnitTypeID & buildingType, Zeratul & bot);
    void SmartAbility       (const sc2::Unit * builder,   const sc2::AbilityID & abilityID, Zeratul & bot);
	void SmartAbility       (const sc2::Unit * builder,   const sc2::AbilityID & abilityID, const sc2::Unit * target,Zeratul &bot);
	void SmartAbility       (const sc2::Unit * builder,   const sc2::AbilityID & abilityID, const sc2::Point2D & point, Zeratul & bot);
	void SmartWarp          (const sc2::Unit * builder,   const sc2::UnitTypeID & buildingType, const sc2::Point2D &point, Zeratul & bot);
};  