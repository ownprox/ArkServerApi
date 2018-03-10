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

void InitConfig(bool Reload)
{
	std::ifstream file(GetCurrentDir() + "/ArkApi/Plugins/Mutes/MuteData.json");
	nlohmann::json MuteConfigData;
	std::string Data;
	if (!Reload)
	{
		if (file.is_open())
		{
			file >> MuteConfigData;
			file.close();
			auto MutedPlayers = MuteConfigData["Mutes"]["MutedPlayers"];
			for (const auto& MutedPlayer : MutedPlayers)
			{
				Data = MutedPlayer["IP"];
				LoadMute(MutedPlayer["SteamID"], MutedPlayer["MutedTill"], MutedPlayer["IPMute"], Data.c_str());
			}
		}
	}

	file = std::ifstream(GetCurrentDir() + "/ArkApi/Plugins/Mutes/config.json");
	if (!file.is_open()) return;
	file >> MuteConfigData;
	file.close();
	auto BLItemMap = MuteConfigData["Mutes"]["BlackList"];
	for (const auto& szitem : BLItemMap)
	{
		Data = szitem["Word"];
		BadWord.push_back(BadWords(ArkApi::Tools::Utf8Decode(Data), szitem["Minutes"]));
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
	std::ofstream file(GetCurrentDir() + "/ArkApi/Plugins/Mutes/MuteData.json");
	if (!file.is_open()) return;
	file << nlohmann::json::parse(MutedPlayerStream.str());
	file.close();
}