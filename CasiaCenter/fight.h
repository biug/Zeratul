#pragma once
#include <string>
#include "sc2api/sc2_api.h"

sc2::ConfigAgent* loadBotFromDLL(const std::string & botpath, const std::string & botconfig);

sc2::Race GetRaceFromString(const std::string & raceIn);

int fight(char *sc2_path,
	const std::string & bot1_path, const std::string & bot1_config, const std::string & bot1_race,
	const std::string & bot2_path, const std::string & bot2_config, const std::string & bot2_race,
	const std::string & map_string, const std::string & replay_path);