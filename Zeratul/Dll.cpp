#include "Zeratul.h"

#ifdef ZERATUL_EXPORTS  
#define ZERATUL_API __declspec(dllexport)
#else  
#define ZERATUL_API __declspec(dllimport)
#endif

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

extern "C" ZERATUL_API sc2::Agent* newAIModule()
{
	return new Zeratul();
}