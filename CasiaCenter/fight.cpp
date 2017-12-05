#include "rapidjson/document.h"
#include "JSONTools.h"
#include "fight.h"
#include "sc2utils/sc2_manage_process.h"
#include <Windows.h>
#include <iostream>

typedef sc2::ConfigAgent*(*DLLFunc)(const std::string &);

sc2::ConfigAgent* loadBotFromDLL(const std::string & botpath, const std::string & botconfig)
{
	std::string botname = botpath.substr(0, botpath.find('.'));
	HINSTANCE hInstLib = LoadLibrary(botpath.c_str());
	if (hInstLib == nullptr)
	{
		std::cerr << botname << " path invalid\n";
		return nullptr;
	}
	DLLFunc dllFunc = (DLLFunc)GetProcAddress(hInstLib, "newAIModule");
	if (dllFunc == nullptr)
	{
		std::cerr << botname << " doesn't have newAIModule function\n";
		return nullptr;
	}
	sc2::ConfigAgent *bot = dllFunc(botconfig);
	if (bot == nullptr)
	{
		std::cerr << botname << "create failed\n";
		return nullptr;
	}
	//FreeLibrary(hInstLib);
	return bot;
}

sc2::Race GetRaceFromString(const std::string & raceIn)
{
	std::string race(raceIn);
	std::transform(race.begin(), race.end(), race.begin(), ::tolower);

	if (race == "terran")
	{
		return sc2::Race::Terran;
	}
	else if (race == "protoss")
	{
		return sc2::Race::Protoss;
	}
	else if (race == "zerg")
	{
		return sc2::Race::Zerg;
	}
	else if (race == "random")
	{
		return sc2::Race::Random;
	}

	BOT_ASSERT(false, "Unknown Race: ", race.c_str());
	return sc2::Race::Terran;
}

int fight(char *sc2_path,
	const std::string & bot1_path, const std::string & bot1_config, const std::string & bot1_race,
	const std::string & bot2_path, const std::string & bot2_config, const std::string & bot2_race,
	const std::string & map_string, const std::string & replay_path)
{
	int argc = 3;
	char* argv[3];
	argv[0] = "CasiaCenter";
	argv[1] = "-e";
	argv[2] = sc2_path;
	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(argc, argv))
	{
		std::cout << "Unable to find or parse settings." << std::endl;
		exit(-1);
	}

	sc2::Agent *bot1 = loadBotFromDLL(bot1_path, bot1_config), *bot2 = loadBotFromDLL(bot2_path, bot2_config);
	if (bot1 == nullptr || bot2 == nullptr)
	{
		exit(-1);
	}


	// WARNING: Bot logic has not been thorougly tested on step sizes > 1
	//          Setting this = N means the bot's onFrame gets called once every N frames
	//          The bot may crash or do unexpected things if its logic is not called every frame
	coordinator.SetStepSize(1);
	coordinator.SetRealtime(false);
	coordinator.SetReplayPath(replay_path);

	coordinator.SetParticipants({
		sc2::CreateParticipant(GetRaceFromString(bot1_race), bot1),
		sc2::CreateParticipant(GetRaceFromString(bot2_race), bot2)
	});

	// Start the game.
	coordinator.LaunchStarcraft();
	coordinator.StartGame(map_string);
	// 1: botA win, 0: draw, -1: botB win
	int match_result = 0;
	// Step forward the game simulation.
	while (true)
	{
		coordinator.Update();
		if (coordinator.AllGamesEnded())
		{
			uint32_t botA_id = bot1->Observation()->GetPlayerID();
			auto result = bot1->Observation()->GetResults();
			for (sc2::PlayerResult player_result : result)
			{
				if (player_result.player_id == botA_id)
				{
					if (player_result.result == sc2::GameResult::Win)
					{
						match_result = 1;
					}
					else if (player_result.result == sc2::GameResult::Loss)
					{
						match_result = -1;
					}
					else
					{
						match_result = 0;
					}
				}
			}
			break;
		}

	}
	coordinator.SaveReplayList(replay_path);

	return match_result;
}