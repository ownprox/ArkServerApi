#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig(bool Reload)
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/Mutes/config.json");
	nlohmann::json MuteConfigData;
	std::string Data;

	if (!file.is_open()) return;
	file >> MuteConfigData;
	file.close();
	
	if (Reload)
		BadWord.Empty();

	DBPath = MuteConfigData["Mutes"].value("DBOverride", "");

	SyncSeconds = MuteConfigData["Mutes"].value("DBSyncSeconds", 60);

	const auto& BLItemMap = MuteConfigData["Mutes"]["BlackList"];
	for (const auto& szitem : BLItemMap)
	{
		Data = szitem["Word"];
		BadWord.Add(BadWords(ArkApi::Tools::Utf8Decode(Data), szitem["Minutes"]));
	}

	int j = 0;
	const auto& Msgs = MuteConfigData["Mutes"]["Messages"];
	for (const auto& Msg : Msgs)
	{
		Data = Msg;
		Messages[j++] = ArkApi::Tools::Utf8Decode(Data).c_str();
	}
}