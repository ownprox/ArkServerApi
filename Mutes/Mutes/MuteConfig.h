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
	const auto& BLItemMap = MuteConfigData["Mutes"]["BlackList"];
	for (const auto& szitem : BLItemMap)
	{
		Data = szitem["Word"];
		BadWord.push_back(BadWords(ArkApi::Tools::Utf8Decode(Data), szitem["Minutes"]));
	}

	int j = 0;
	const auto& Msgs = MuteConfigData["Mutes"]["Messages"];
	for (const auto& Msg : Msgs)
	{
		Data = Msg;
		Messages[j++] = ArkApi::Tools::Utf8Decode(Data).c_str();
	}
}