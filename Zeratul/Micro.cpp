#include "Micro.h"
#include "Util.h"
#include "Zeratul.h"

const float dotRadius = 0.1f;

void Micro::SmartStop(const sc2::Unit * attacker, Zeratul & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::STOP);
}

void Micro::SmartAttackUnit(const sc2::Unit * attacker, const sc2::Unit * target, Zeratul & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    BOT_ASSERT(target != nullptr, "Target is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::ATTACK_ATTACK, target);
}

void Micro::SmartAttackMove(const sc2::Unit * attacker, const sc2::Point2D & targetPosition, Zeratul & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::ATTACK_ATTACK, targetPosition);
}

void Micro::SmartMove(const sc2::Unit * attacker, const sc2::Point2D & targetPosition, Zeratul & bot)
{
    BOT_ASSERT(attacker != nullptr, "Attacker is null");
    bot.Actions()->UnitCommand(attacker, sc2::ABILITY_ID::MOVE, targetPosition);
}

void Micro::SmartRightClick(const sc2::Unit * unit, const sc2::Unit * target, Zeratul & bot)
{
    BOT_ASSERT(unit != nullptr, "Unit is null");
    bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::SMART, target);
}

void Micro::SmartRepair(const sc2::Unit * unit, const sc2::Unit * target, Zeratul & bot)
{
    BOT_ASSERT(unit != nullptr, "Unit is null");
    bot.Actions()->UnitCommand(unit, sc2::ABILITY_ID::SMART, target);
}

void Micro::SmartKiteTarget(const sc2::Unit * rangedUnit, const sc2::Unit * target, Zeratul & bot)
{
    BOT_ASSERT(rangedUnit != nullptr, "RangedUnit is null");
    BOT_ASSERT(target != nullptr, "Target is null");
    bot.Actions()->UnitCommand(rangedUnit, sc2::ABILITY_ID::ATTACK_ATTACK, target);
}

void Micro::SmartBuild(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, sc2::Point2D pos, Zeratul & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    bot.Actions()->UnitCommand(builder, bot.Data(buildingType).buildAbility, pos);
}

void Micro::SmartBuildTarget(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, const sc2::Unit * target, Zeratul & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    BOT_ASSERT(target != nullptr, "Target is null");
    bot.Actions()->UnitCommand(builder, bot.Data(buildingType).buildAbility, target);
}

void Micro::SmartTrain(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, Zeratul & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    bot.Actions()->UnitCommand(builder, bot.Data(buildingType).buildAbility);
}

void Micro::SmartWarp(const sc2::Unit * builder, const sc2::UnitTypeID & buildingType, const sc2::Point2D& point, Zeratul & bot)
{
	BOT_ASSERT(builder != nullptr, "Builder is null");
	bot.Actions()->UnitCommand(builder, bot.Data(buildingType).warpAbility, point);
}
void Micro::SmartAbility(const sc2::Unit * builder, const sc2::AbilityID & abilityID, Zeratul & bot)
{
    BOT_ASSERT(builder != nullptr, "Builder is null");
    bot.Actions()->UnitCommand(builder, abilityID);
}

void Micro::SmartAbility(const sc2::Unit * builder, const sc2::AbilityID & abilityID,  const sc2::Unit * target, Zeratul & bot)
{
	BOT_ASSERT(builder != nullptr, "Builder is null");
	bot.Actions()->UnitCommand(builder, abilityID, target);
}

void Micro::SmartAbility(const sc2::Unit * builder, const sc2::AbilityID & abilityID, const sc2::Point2D& point, Zeratul & bot)
{
	BOT_ASSERT(builder != nullptr, "Builder is null");
	bot.Actions()->UnitCommand(builder, abilityID, point);
}