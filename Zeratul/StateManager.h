#pragma once

#include "Common.h"

class Zeratul;

struct StateManager
{
	Zeratul & m_bot;
	bool m_waitWarpGate;
	bool m_waitBlink;
	bool m_rallyAtPylon;
	bool m_startAttack;
	bool m_startBlink;
	bool m_keepTrainWorker;
	bool m_rschWarpGate;
	bool m_rschBlink;
	bool m_scout;
	bool m_patrol;
	bool m_stimpack;
	int m_numKeepTrainWorker;
	
	sc2::UnitTypeID m_chronoTarget;

public:
	StateManager(Zeratul & bot);
	void clear();
	void onFrame();
	void OnUpgradeCompleted(sc2::UpgradeID upgradeID);

	bool shouldAttack();
};