#include "Mutes.h"
#include "MuteManager.h"
#include "MuteConfig.h"
#include "MuteHooks.h"
#include "MuteCommands.h"
#pragma comment(lib, "ArkApi.lib")

void Init()
{
	Log::Get().Init("Mutes");
	InitConfig(false);
	InitMutes();
	InitHooks();
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
		RemoveHooks();
		RemoveCommands();
		break;
	}
	return TRUE;
}