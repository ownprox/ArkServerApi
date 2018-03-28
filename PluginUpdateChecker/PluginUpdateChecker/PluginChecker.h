#pragma once
#include <filesystem>
#include <fstream>
#include "json.hpp"
#include "API\ARK\Ark.h"

const FString FindStarter = "muted\">";
FString DiscordWebHookURL;
TSharedRef<IHttpRequest, 0> request;
bool VerifyPost = false, IsCheckingArray = true, DiscordUseWebHooks = false;
int PluginIndex = 0, SecCounter;

struct PluginS
{
	FString Name;
	float Version;
	int ResourceID;
	long long LastWarningMS;
	PluginS(FString Name, float Version, int ResourceID) : Name(Name), Version(Version), ResourceID(ResourceID), LastWarningMS(0) {}
};

TArray<PluginS> PluginsArray;

void LoadAllPlugins()
{
	namespace fs = std::experimental::filesystem;
	const std::string dir_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins";
	nlohmann::json Config;
	for (const auto& dir_name : fs::directory_iterator(dir_path))
	{
		const auto& path = dir_name.path();
		if (!is_directory(path)) continue;
		const auto& filename = path.filename().stem().generic_string();
		const auto& ConfigName = dir_path + "/" + filename + "/PluginInfo.json";
		if (!fs::exists(ConfigName)) continue;
		std::ifstream file(ConfigName);
		if (!file.is_open()) continue;
		try
		{
			file >> Config;
			const int ResourceId = Config.value("ResourceId", -1);
			if (ResourceId == -1) continue;
			PluginsArray.Add(PluginS(filename.c_str(), Config["Version"], ResourceId));
			
		}
		catch (const std::exception& error)
		{
			Log::GetLog()->error(error.what());
			throw;
		}
		file.close();
	}
}

template <typename T, typename... Args>
void PostDiscord(const T* msg, Args&&... args)
{
	static_cast<AShooterGameState*>(ArkApi::GetApiUtils().GetWorld()->GameStateField()())->HTTPPostRequest(DiscordWebHookURL, FString::Format(msg, std::forward<Args>(args)...));
}

float GetPluginVersion(const FString& Response)
{
	float Version = 0.0f;
	int32 FindStart = Response.Find(FindStarter);
	if (FindStart != INDEX_NONE)
	{
		FindStart = FindStart + 7;
		const int32 FindEnd = Response.Find("<", ESearchCase::IgnoreCase, ESearchDir::FromStart, FindStart);
		if (FindEnd != INDEX_NONE)
		{
			try
			{
				Version = std::stof(Response.Mid(FindStart, FindEnd - FindStart).ToString().c_str());
			}
			catch (...) {}
		}
	}
	return Version;
}