#include "sc2api/sc2_api.h"
#include "sc2utils/sc2_manage_process.h"
#include "rapidjson/document.h"
#include "JSONTools.h"
#include "gflags/gflags.h"
#include "fight.h"

#include <iostream>
#include <string>
#include <random>
#include <cmath>

#include <Windows.h>
#include <WinBase.h>

DEFINE_string(mode, "PvC", "play mode");
DEFINE_string(map, "D:\\scbots\\maps\\Ladder2017Season1\\AbyssalReefLE.SC2Map", "map");
DEFINE_string(bot1, "Zeratul.dll", "bot1");
DEFINE_string(config1, "BotConfig.txt", "config1");
DEFINE_string(race1, "Protoss", "race1");
DEFINE_string(bot2, "Zeratul.dll", "bot2");
DEFINE_string(config2, "BotConfig.txt", "config2");
DEFINE_string(race2, "Protoss", "race2");
DEFINE_string(computer_race, "Terran", "computer race");
DEFINE_int32(computer_difficulty, 5, "Hard");
DEFINE_int32(step_size, 1, "step size");
DEFINE_string(game_path, "E:\\StarCraft II\\Versions\\Base59587\\SC2_x64.exe", "game path");

int main(int argc, char* argv[])
{
	google::ParseCommandLineFlags(&argc, &argv, true);

	char *s_GamePath = new char(FLAGS_game_path.size() + 1);
	strcpy(s_GamePath, FLAGS_game_path.c_str());

	char *coorArgs[3];
	coorArgs[0] = argv[0];
	coorArgs[1] = "-e";
	coorArgs[2] = s_GamePath;

	sc2::Coordinator coordinator;
	if (!coordinator.LoadSettings(3, coorArgs))
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
		fight(s_GamePath,
			FLAGS_bot1, FLAGS_config1, FLAGS_race1,
			FLAGS_bot2, FLAGS_config2, FLAGS_race2,
			FLAGS_map, "E:\\StarCraft II\\Temp");
	}
	else if (FLAGS_mode == "PvC")
	{
		bot1 = loadBotFromDLL(FLAGS_bot1, FLAGS_config1);
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
	if (s_GamePath)
	{
		delete s_GamePath;
	}

	return 0;
}
