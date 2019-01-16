#include "VoteRewards.h"

void RewardPlayer(const std::string& VoteSite, AShooterPlayerController* player)
{
	int Quantity;
	float Quality;
	bool IsBP;
	std::string sBlueprint;
	FString fBlueprint;
#ifndef ATLAS
	TArray<UPrimalItem*> Out;
#endif
	auto Items = VoteConfig[VoteSite]["Items"];
	for (const auto& Item : Items)
	{
		if (Item["RewardChance"] <= 0 || RandomNumber(0, Item["RewardChance"]) == 0)
		{
			Quantity = Item["MaxQuantity"] == Item["MinQuantity"] ? Item["MinQuantity"] : RandomNumber(Item["MinQuantity"], Item["MaxQuantity"]);

			if (Quantity > 0)
			{
				sBlueprint = Item["Blueprint"];
				fBlueprint = sBlueprint.c_str();

				Quality = Item["MaxQuality"] == Item["MinQuality"] ? Item["MinQuality"] : RandomNumber(Item["MinQuality"], Item["MaxQuality"]);

				IsBP = Item["MaxIsBP"] == 0 ? false :
					(Item["MinIsBP"] == Item["MaxIsBP"] ? Item["MinIsBP"]
						: RandomNumber(Item["MinIsBP"], Item["MaxIsBP"]));
#ifdef ATLAS
				player->GiveItem(&fBlueprint, Quantity, Quality, IsBP);
#else
				player->GiveItem(&Out, &fBlueprint, Quantity, Quality, IsBP, false, 0);
#endif
			}
		}
	}

	int Level;
	auto Dinos = VoteConfig[VoteSite]["Dinos"];
	for (const auto& Dino : Dinos)
	{
		if (Dino["RewardChance"] <= 0 || RandomNumber(0, Dino["RewardChance"]) == 0)
		{
			sBlueprint = Dino["Blueprint"];
			fBlueprint = sBlueprint.c_str();
			Level = Dino["MaxLevel"] == Dino["MinLevel"] ? Dino["MinLevel"] : RandomNumber(Dino["MinLevel"], Dino["MaxLevel"]);

			ArkApi::GetApiUtils().SpawnDino(player, fBlueprint, nullptr, Level, Dino["Tamed"], Dino["Neutered"]);
		}
	}
	ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg(VoteSite, 1));
}

void HttpVoteCallBack(bool Success, std::string Response, const int VoteSiteIndex, const VoteSiteConfig& voteSiteConfig, AShooterPlayerController* player, const bool IsLast)
{
	if (!player) return;

	if (!Success)
	{
		ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg(voteSiteConfig.Site, 0));
		return;
	}

	if (voteSiteConfig.ExactMatch && Response == voteSiteConfig.Response || !voteSiteConfig.ExactMatch && Response.find(voteSiteConfig.Response) != std::string::npos)
	{
		PlayerData[player->LinkedPlayerIDField()].HasRewarded = true;
		const long double nNow = ArkApi::GetApiUtils().GetWorld()->GetTimeSeconds();
		PlayerData[player->LinkedPlayerIDField()].NextVoteTime[VoteSiteIndex] = nNow + ((static_cast<long double>(VoteConfig[voteSiteConfig.Site]["VoteDelayHours"]) * 3600) + 120);
		RewardPlayer(voteSiteConfig.Site, player);
	}
	else if (!PlayerData[player->LinkedPlayerIDField()].HasRewarded && IsLast)
	{
		ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg("VoteRewards", 1));
	}
}

#ifdef ATLAS
void AtlasSubmitVoteRequest(const int VoteSiteIndex, const VoteSiteConfig& voteSiteConfig, const std::string& VoteKey, const int64 SteamID, AShooterPlayerController* player, const bool IsLast)
{
	switch (VoteSiteIndex)
	{
	case 0:	
		API::Requests::Get().CreateGetRequest(fmt::format(
			"https://atlasserverslist.com/api-{}_{}.json",
			VoteKey.c_str(), SteamID), std::bind(&HttpVoteCallBack,
				std::placeholders::_1, std::placeholders::_2, VoteSiteIndex, voteSiteConfig, player, IsLast));
		break;
	case 1:
		API::Requests::Get().CreateGetRequest(fmt::format(
			"http://www.api.trackyserver.com/vote/?action=claim&key={}&steamid={}",
			VoteKey.c_str(), SteamID), std::bind(&HttpVoteCallBack,
				std::placeholders::_1, std::placeholders::_2, VoteSiteIndex, voteSiteConfig, player, IsLast));
		break;
	case 2:
		API::Requests::Get().CreateGetRequest(fmt::format(
			"https://atlasserverlist.com/rewards?claim&serverkey={}&steamid={}",
			VoteKey.c_str(), SteamID), std::bind(&HttpVoteCallBack,
				std::placeholders::_1, std::placeholders::_2, VoteSiteIndex, voteSiteConfig, player, IsLast));
		break;
	}
}
#else
void ArkSubmitVoteRequest(const int VoteSiteIndex, const VoteSiteConfig& voteSiteConfig, const std::string& VoteKey, const int64 SteamID, AShooterPlayerController* player, const bool IsLast)
{
	switch (VoteSiteIndex)
	{
	case 0:
		API::Requests::Get().CreateGetRequest(fmt::format(
			"https://ark-servers.net/api/?action=post&object=votes&element=claim&key={}&steamid={}",
			VoteKey.c_str(), SteamID), std::bind(&HttpVoteCallBack,
				std::placeholders::_1, std::placeholders::_2, VoteSiteIndex, voteSiteConfig, player, IsLast));
		break;
	case 1:
		API::Requests::Get().CreateGetRequest(fmt::format(
			"http://www.api.trackyserver.com/vote/?action=claim&key={}&steamid={}",
			VoteKey.c_str(), SteamID), std::bind(&HttpVoteCallBack,
				std::placeholders::_1, std::placeholders::_2, VoteSiteIndex, voteSiteConfig, player, IsLast));
		break;
	case 2:
		API::Requests::Get().CreateGetRequest(fmt::format(
			"https://toparkservers.com/rewards?claim&serverkey={}&steamid={}",
			VoteKey.c_str(), SteamID), std::bind(&HttpVoteCallBack,
				std::placeholders::_1, std::placeholders::_2, VoteSiteIndex, voteSiteConfig, player, IsLast));
		break;
	}
}
#endif

void CheckVoteReward(AShooterPlayerController* player)
{
	const long double nNow = ArkApi::GetApiUtils().GetWorld()->GetTimeSeconds();
	const auto playerData = PlayerData.emplace(player->LinkedPlayerIDField(), PlayerDataS{ {0, }, false });
	const int64 SteamID = ArkApi::GetApiUtils().GetSteamIDForPlayerID(static_cast<int>(player->LinkedPlayerIDField()));
	PlayerData[player->LinkedPlayerIDField()].HasRewarded = false;
	bool VotesChecked = false;
	long double LowestNextVoteTime = -1;
	for (const auto& VoteSiteConfig : VoteSites)
	{
		const int VoteSiteIndex = GetVoteSiteIndex(VoteSiteConfig.Site);
		if (VoteSiteIndex != -1)
		{
			const long double NextVoteTime = playerData.first->second.NextVoteTime[VoteSiteIndex];
			if (nNow > NextVoteTime)
			{
				const bool IsLast = (VoteSiteIndex == (VoteSites.size() - 1));
				PlayerData[player->LinkedPlayerIDField()].NextVoteTime[VoteSiteIndex] = nNow + 30;
				auto VoteKeys = VoteConfig[VoteSiteConfig.Site].value("VoteKeys", nlohmann::json::array());
				for (const auto& VoteKey : VoteKeys)
				{
#ifdef ATLAS
					AtlasSubmitVoteRequest(VoteSiteIndex, VoteSiteConfig, VoteKey, SteamID, player, IsLast);
#else
					ArkSubmitVoteRequest(VoteSiteIndex, VoteSiteConfig, VoteKey, SteamID, player, IsLast);
#endif
					VotesChecked = true;
				}
			}
			else if (LowestNextVoteTime == -1 || LowestNextVoteTime > NextVoteTime) LowestNextVoteTime = NextVoteTime;
		}
		else if (player->bIsAdmin()()) ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), L"Vote Site does not exist: {}", VoteSiteConfig.Site);
	}

	if (VotesChecked)
	{
		ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg("VoteRewards", 0));
	}
	else
	{
		const long double SecondsRemaining = LowestNextVoteTime - nNow,
			hours = (SecondsRemaining > 3600 ? (SecondsRemaining / 3600) : 0),
			mins = (hours > 0 && (SecondsRemaining - (hours * 3600)) > 60) ?
			((SecondsRemaining - (hours * 3600)) / 60) : (SecondsRemaining > 60 ? (SecondsRemaining / 60) : 0);
		if (mins != 0 || hours != 0) ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg("VoteRewards", 4), static_cast<int>(hours), static_cast<int>(mins));
		else ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg("VoteRewards", 5), static_cast<int>(SecondsRemaining));
	}
}

void CheckVoteCMD(AShooterPlayerController* player, FString* message, int mode)
{
	if (player->PlayerStateField() && player->GetPlayerCharacter())
		CheckVoteReward(player);
}

void VoteSitesCMD(AShooterPlayerController* player, FString* message, int mode)
{
	if (player->PlayerStateField() && player->GetPlayerCharacter())
	{
		ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg("VoteRewards", 2));
		for (const auto& VoteSiteConfig : VoteSites)
		{
			auto VoteUrls = VoteConfig[VoteSiteConfig.Site].value("VoteUrls", nlohmann::json::array());
			for (const auto& VoteUrl : VoteUrls)
				ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), L"{}", *FString(ArkApi::Tools::Utf8Decode(VoteUrl).c_str()));
		}
		ArkApi::GetApiUtils().SendChatMessage(player, *GetConfig("ServerName"), *GetMsg("VoteRewards", 3));
	}
}

void ReloadConfigCMD(AShooterPlayerController* player, FString* message, int mode)
{
	if (player->PlayerStateField() && player->bIsAdmin()())
		LoadConfig();
}

int Counter = 0;
void PlayerVoteDataCleanUp()
{
	if (Counter++ > 86400)
	{
		Counter = 0;
		const long double nNow = ArkApi::GetApiUtils().GetWorld()->GetTimeSeconds();
		bool CanRemove = true;
		for(auto& itr = PlayerData.begin(); itr != PlayerData.end();)
		{
			for (int i = 0; i < TotalVoteSites; i++) if (itr->second.NextVoteTime[i] > nNow) CanRemove = false;
			if (CanRemove)
			{
				PlayerData.erase(itr++);
				continue;
			}
			CanRemove = true;
			++itr;
		}
	}
}

void Load()
{
	Log::Get().Init("VoteRewards");
	LoadConfig();
	ArkApi::GetCommands().AddChatCommand(GetConfig("CheckCommand"), &CheckVoteCMD);
	ArkApi::GetCommands().AddChatCommand(GetConfig("VoteSitesCommand"), &VoteSitesCMD);
	ArkApi::GetCommands().AddChatCommand("/vrreload", &ReloadConfigCMD);
	ArkApi::GetCommands().AddOnTimerCallback("ClearOldVotes", &PlayerVoteDataCleanUp);
}

void Unload()
{
	PlayerData.clear();
	ArkApi::GetCommands().RemoveChatCommand(GetConfig("CheckCommand"));
	ArkApi::GetCommands().RemoveChatCommand(GetConfig("VoteSitesCommand"));
	ArkApi::GetCommands().RemoveChatCommand("/vrreload");
	ArkApi::GetCommands().RemoveOnTimerCallback("ClearOldVotes");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}