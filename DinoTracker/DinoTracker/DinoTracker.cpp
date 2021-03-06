#include "DinoTracker.h"
#include "DinoTrackerCommands.h"
#include "DinoTrackerConfig.h"

#pragma comment(lib, "ArkApi.lib")

void Init()
{
	Log::Get().Init("DinoTracker");
	InitConfig();
	InitCommands();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
		break;
	case DLL_PROCESS_DETACH:
		RemoveCommands();
		break;
	}
	return TRUE;
}