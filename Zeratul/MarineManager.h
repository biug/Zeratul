#pragma once

#include "Common.h"
#include "MicroManager.h"

class Zeratul;

struct MarineInfo
{
	float m_hpLastSecond;

	MarineInfo();
	MarineInfo(float hp);
};

class MarineManager : public MicroManager
{
	std::unordered_map<const sc2::Unit*, MarineInfo> marineInfos;
public:

	MarineManager(Zeratul & bot);
	void    executeMicro(const std::vector<const sc2::Unit *> & targets);
	void    assignTargets(const std::vector<const sc2::Unit *> & targets);
	int     getAttackPriority(const sc2::Unit * rangedUnit, const sc2::Unit * target);
	const sc2::Unit * getTarget(const sc2::Unit * rangedUnit, const std::vector<const sc2::Unit *> & targets);
};
