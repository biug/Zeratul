#pragma once

#include "Common.h"
#include "MicroManager.h"

class Zeratul;

class MeleeManager: public MicroManager
{

public:

    MeleeManager(Zeratul & bot);
    void    executeMicro(const std::vector<const sc2::Unit *> & targets);
    void    assignTargets(const std::vector<const sc2::Unit *> & targets);
    int     getAttackPriority(const sc2::Unit * attacker, const sc2::Unit * unit);
    const sc2::Unit * getTarget(const sc2::Unit * meleeUnit, const std::vector<const sc2::Unit *> & targets);
    bool    meleeUnitShouldRetreat(const sc2::Unit * meleeUnit, const std::vector<const sc2::Unit *> & targets);
};
