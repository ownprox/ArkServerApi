#pragma once
#include <fstream>
#include "json.hpp"

std::string GetCurrentDir()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

void InitConfig()
{
	std::ifstream file(GetCurrentDir() + "/ArkApi/Plugins/Mutes/config.json");
	if (!file.is_open()) return;
	nlohmann::json MuteConfigData;
	file >> MuteConfigData;
	file.close();
	std::string IPAddress;
	auto MutedPlayers = MuteConfigData["Mutes"]["MutedPlayers"];
	for (const auto& MutedPlayer : MutedPlayers)
	{
		IPAddress = MutedPlayer["IP"];
		LoadMute(MutedPlayer["SteamID"], MutedPlayer["MutedTill"], MutedPlayer["IPMute"], IPAddress.c_str());
	}
}

void SaveConfig()
{
	bool First = true;
	const DWORD64 NowTime = timeGetTime();
	std::stringstream MutedPlayerStream;
	MutedPlayerStream << "{\"Mutes\": {\"MutedPlayers\":[";
	for (const auto& MutedPlayer : muteData)
	{
		if (MutedPlayer.MutedTill > NowTime) MutedPlayerStream << (First ? "" : ", ") << "{ \"SteamID\":" << MutedPlayer.SteamID << ",\"MutedTill\":" << MutedPlayer.MutedTill << ",\"IPMute\":" << (MutedPlayer.IsIP ? "true" : "false") << ",\"IP\":\"" << MutedPlayer.IPAddress.ToString() << "\" }";
		First = false;
	}
	MutedPlayerStream << "]}}";
	std::ofstream file(GetCurrentDir() + "/ArkApi/Plugins/Mutes/config.json");
	if (!file.is_open()) return;
	file << nlohmann::json::parse(MutedPlayerStream.str());
	file.close();
}