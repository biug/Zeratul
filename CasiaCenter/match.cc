#include "match.h"
#include <stdio.h>
#include "fight.h"
#include <string.h>

JNIEXPORT jint JNICALL Java_match_service_MatchService_match
(JNIEnv *env, jobject object, jstring sc2_path,
	jstring bot1_path, jstring bot1_config, jstring bot1_race,
	jstring bot2_path, jstring bot2_config, jstring bot2_race,
	jstring map_path, jstring replay_path)
{
	const char *sc2_path_str = env->GetStringUTFChars(sc2_path, 0);
	const char *bot1_path_str = env->GetStringUTFChars(bot1_path, 0);
	const char *bot1_config_str = env->GetStringUTFChars(bot1_config, 0);
	const char *bot1_race_str = env->GetStringUTFChars(bot1_race, 0);
	const char *bot2_path_str = env->GetStringUTFChars(bot2_path, 0);
	const char *bot2_config_str = env->GetStringUTFChars(bot2_config, 0);
	const char *bot2_race_str = env->GetStringUTFChars(bot2_race, 0);
	const char *map_path_str = env->GetStringUTFChars(map_path, 0);
	const char *replay_path_str = env->GetStringUTFChars(replay_path, 0);
	//need to release this string when done with it in order to
	char *buf = (char*)malloc(strlen(sc2_path_str) + 1);
	strcpy(buf, sc2_path_str);
	int res = fight(buf,
		bot1_path_str, bot1_config_str, bot1_race_str,
		bot2_path_str, bot2_config_str, bot2_race_str,
		map_path_str,replay_path_str);

	//avoid memory leak
	env->ReleaseStringUTFChars(sc2_path, sc2_path_str);
	env->ReleaseStringUTFChars(bot1_path, bot1_path_str);
	env->ReleaseStringUTFChars(bot1_config, bot1_config_str);
	env->ReleaseStringUTFChars(bot1_race, bot1_race_str);
	env->ReleaseStringUTFChars(bot2_path, bot2_path_str);
	env->ReleaseStringUTFChars(bot2_config, bot2_config_str);
	env->ReleaseStringUTFChars(bot2_race, bot2_race_str);
	env->ReleaseStringUTFChars(map_path, map_path_str);
	env->ReleaseStringUTFChars(replay_path, replay_path_str);
	free(buf);
	return res;
}