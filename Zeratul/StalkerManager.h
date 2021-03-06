#pragma once

#include "Common.h"
#include "MicroManager.h"

class Zeratul;

struct StalkerInfo
{
	float m_hpLastSecond;

	StalkerInfo();
	StalkerInfo(float hp);
};

class StalkerManager : public MicroManager
{
	std::unordered_map<const sc2::Unit*, StalkerInfo> stalkerInfos;
public:

	StalkerManager(Zeratul & bot);
	void    executeMicro(const std::vector<const sc2::Unit *> & targets);
	void    assignTargets(const std::vector<const sc2::Unit *> & targets);
	int     getAttackPriority(const sc2::Unit * rangedUnit, const sc2::Unit * target);
	const sc2::Unit * getTarget(const sc2::Unit * rangedUnit, const std::vector<const sc2::Unit *> & targets);
};
