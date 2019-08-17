#include <API/ARK/Ark.h>

#include "DinoColourCommand.h"
#include "DinoColourCommandCommands.h"
#include "DinoColourCommandConfig.h"

#pragma comment(lib, "ArkApi.lib")
#pragma comment(lib, "Permissions.lib")

void Init()
{
	Log::Get().Init("DinoColourCommand");
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
