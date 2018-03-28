#include "PluginChecker.h"
#pragma comment(lib, "ArkApi.lib")

void PluginUpdateCheck()
{
	if (PluginsArray.Num() == 0) return;
	SecCounter++;
	if(SecCounter > (IsCheckingArray ? 0 : 21600))
	{
		if (!VerifyPost)
		{
			FHttpModule::Get()->CreateRequest(&request);
			request->SetHeader(&FString("Content-Type"), &FString("text/html"));
			request->SetURL(&FString::Format("https://arkserverapi.com/resources/{}/", PluginsArray[PluginIndex].ResourceID));
			request->SetVerb(&FString("GET"));
			request->ProcessRequest();
			IsCheckingArray = VerifyPost = true;
		}
		else
		{
			switch (request->GetStatus())
			{
				case EHttpRequestStatus::Succeeded:
				{
					FString Response;
					request->ResponseField()()->GetContentAsString(&Response);
					request->FinishedRequest();

					float Version = GetPluginVersion(Response);

					if (Version != 0.0f && PluginsArray[PluginIndex].Version != Version)
					{
						const long long& nNowTime = timeGetTime();
						if (DiscordUseWebHooks && nNowTime > PluginsArray[PluginIndex].LastWarningMS)
						{
							PostDiscord(L"{{\"content\":\"{} {} is now available for download from https://arkserverapi.com/resources/{}/\",\"username\":\"Plugin Update Checker\",\"avatar_url\":null}}", PluginsArray[PluginIndex].Name.ToString().c_str(), Version, PluginsArray[PluginIndex].ResourceID);
							Log::GetLog()->info("{} {} is now available for download from www.arkserverapi.com", PluginsArray[PluginIndex].Name.ToString().c_str(), Version);
							PluginsArray[PluginIndex].LastWarningMS = nNowTime + 1800000;
						}
					}
					PluginIndex++;
					if (PluginIndex == PluginsArray.Num())
					{
						PluginIndex = 0; 
						IsCheckingArray = false;
					}
					VerifyPost = false;
				}
				break;

				case EHttpRequestStatus::NotStarted:
				case EHttpRequestStatus::Failed:
				{
					IsCheckingArray = VerifyPost = false;
				}
				break;
			}
		}
	}
}

void Init()
{
	Log::Get().Init("PluginChecker");
	LoadAllPlugins();
	ArkApi::GetCommands().AddOnTimerCallback("PluginUpdateCheck", &PluginUpdateCheck);
}

void InitConfig()
{
	nlohmann::json PluginCheckerConfig;
	std::ifstream file = std::ifstream(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/PluginUpdateChecker/config.json");
	if (!file.is_open()) return;
	file >> PluginCheckerConfig;
	file.close();

	DiscordUseWebHooks = PluginCheckerConfig["PluginChecker"]["DiscordUseWebHooks"];
	std::string Data;
	Data = PluginCheckerConfig["PluginChecker"]["DiscordWebHookURL"];
	DiscordWebHookURL = Data.c_str();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitConfig();
		Init();
		break;
	case DLL_PROCESS_DETACH:
		ArkApi::GetCommands().RemoveOnTimerCallback("PluginUpdateCheck");
		break;
	}
	return TRUE;
}