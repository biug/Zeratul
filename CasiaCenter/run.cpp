#include "sc2api/sc2_api.h"
#include "sc2utils/sc2_manage_process.h"
#include "rapidjson/document.h"
#include "JSONTools.h"
#include "gflags/gflags.h"

#include <iostream>
#include <string>
#include <random>
#include <cmath>

#include <Windows.h>
#include <WinBase.h>

DEFINE_string(mode, "PvC", "play mode");
DEFINE_string(map, "D:\\scbots\\maps\\Ladder2017Season1\\AbyssalReefLE.SC2Map", "map");
DEFINE_string(bot1, "Zeratul.dll", "bot1");
DEFINE_string(race1, "Protoss", "race1");
DEFINE_string(bot2, "Zeratul.dll", "bot2");
DEFINE_string(race2, "Protoss", "race2");
DEFINE_string(computer_race, "Terran", "computer race");
DEFINE_int32(computer_difficulty, 5, "Hard");
DEFINE_int32(step_size, 1, "step size");

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

typedef sc2::Agent*(*DLLFunc)();

sc2::Agent* loadBotFromDLL(const std::string & botpath)
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
	sc2::Agent *bot = dllFunc();
	if (bot == nullptr)
	{
		std::cerr << botname << "create failed\n";
		return nullptr;
	}
	//FreeLibrary(hInstLib);
	std::cout << botpath << std::endl;
	return bot;
}

int main(int argc, char* argv[])
{
	google::ParseCommandLineFlags(&argc, &argv, true);

	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(argc, argv))
	{
		std::cout << "Unable to find or parse settings." << std::endl;
		return 1;
	}

	// WARNING: Bot logic has not been thorougly tested on step sizes > 1
	//          Setting this = N means the bot's onFrame gets called once every N frames
	//          The bot may crash or do unexpected things if its logic is not called every frame
	coordinator.SetStepSize(FLAGS_step_size);
	coordinator.SetRealtime(false);

	sc2::Agent *bot1 = nullptr, *bot2 = nullptr;

	if (FLAGS_mode == "PvP")
	{
		// Add the custom bot, it will control the players.
		bot1 = loadBotFromDLL(FLAGS_bot1);
		bot2 = loadBotFromDLL(FLAGS_bot2);
		if (bot1 == nullptr || bot2 == nullptr)
		{
			exit(-1);
		}

		coordinator.SetParticipants({
			CreateParticipant(GetRaceFromString(FLAGS_race1), bot1),
			CreateParticipant(GetRaceFromString(FLAGS_race2), bot2)
		});
	}
	else if (FLAGS_mode == "PvC")
	{
		bot1 = loadBotFromDLL(FLAGS_bot1);
		if (bot1 == nullptr)
		{
			exit(-1);
		}
		coordinator.SetParticipants({
			CreateParticipant(GetRaceFromString(FLAGS_race1), bot1),
			CreateComputer(GetRaceFromString(FLAGS_computer_race), sc2::Difficulty(FLAGS_computer_difficulty))
		});
	}

	// Start the game.
	coordinator.LaunchStarcraft();
	std::cout << FLAGS_map << std::endl;
	coordinator.StartGame(FLAGS_map);

	// Step forward the game simulation.
	while (true)
	{
		coordinator.Update();
	}

	if (bot1)
	{
		delete bot1;
	}
	if (bot2)
	{
		delete bot2;
	}

	return 0;
}
