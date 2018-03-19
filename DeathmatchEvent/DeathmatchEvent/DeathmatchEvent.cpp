#pragma once
#include "DeathmatchEvent.h"
#include "../../EventManager/EventManager/Public/Event.h"
#include "../../EventManager/EventManager/Public/IEventManager.h"
#pragma comment(lib, "AAEventManager.lib")
#pragma comment(lib, "ArkApi.lib")
#include <fstream>
#include "json.hpp"

class DeathMatch : Event
{
private:
	int ArkShopPointsRewardMin, ArkShopPointsRewardMax, JoinMessages, JoinMessageDelaySeconds, PlayersNeededToStart, WaitForDelay, WaitCounter;
	FString JoinEventCommand, ServerName;

	struct Reward
	{
		const FString BP;
		const int QuantityMin, QuantityMax, QualityMin, QualityMax, MinIsBP, MaxIsBP;
		Reward(const FString BP, const int QuantityMin, const int QuantityMax, const int QualityMin, const int QualityMax, const int MinIsBP, const int MaxIsBP) : BP(BP), QuantityMin(QuantityMin)
			, QuantityMax(QuantityMax), QualityMin(QualityMin), QualityMax(QualityMax), MinIsBP(MinIsBP), MaxIsBP(MaxIsBP) {}
	};

	TArray<Reward> Rewards;

public:
	virtual void InitConfig(const FString& JoinEventCommand, const FString& ServerName, const FString& Map)
	{
		this->JoinEventCommand = JoinEventCommand;
		this->ServerName = ServerName;
		if (!HasConfigLoaded())
		{
			const std::string config_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/DeathMatchEvent/" + Map.ToString() + ".json";

			std::ifstream file { config_path };

			if (!file.is_open()) throw std::runtime_error(fmt::format("Can't open {}.json", Map.ToString().c_str()).c_str());

			nlohmann::json config;

			file >> config;

			const std::string eName = config["Deathmatch"]["EventName"];
			const FString& EventName = ArkApi::Tools::Utf8Decode(eName).c_str();

			PlayersNeededToStart = config["Deathmatch"]["PlayersNeededToStart"];

			JoinMessages = config["Deathmatch"]["JoinMessages"];
			JoinMessageDelaySeconds = config["Deathmatch"]["JoinMessageDelaySeconds"];

			const bool StructureProtection = config["Deathmatch"]["StructureProtection"];
			const auto StructureProtectionPosition = config["Deathmatch"]["StructureProtectionPosition"];
			const int StructureProtectionDistacne = config["Deathmatch"]["StructureProtectionDistance"];

			ArkShopPointsRewardMin = config["Deathmatch"]["ArkShopPointsRewardMin"];
			ArkShopPointsRewardMax = config["Deathmatch"]["ArkShopPointsRewardMax"];
			if (ArkShopPointsRewardMin > ArkShopPointsRewardMax) ArkShopPointsRewardMax = ArkShopPointsRewardMin;
			if (ArkShopPointsRewardMin < 0) ArkShopPointsRewardMin = 0;

			const bool KillOnLogg = config["Deathmatch"]["KillOnLoggout"];

			InitDefaults(EventName, false, KillOnLogg, StructureProtection
				, FVector(StructureProtectionPosition[0], StructureProtectionPosition[1], StructureProtectionPosition[2]), StructureProtectionDistacne);

			const auto& Spawns = config["Deathmatch"]["Spawns"];
			for (const auto& Spawn : Spawns)
			{
				const auto& SpawnPos = Spawn["Position"];
				AddSpawn(FVector(SpawnPos[0], SpawnPos[1], SpawnPos[2]));
			}
			
			const auto& RewardsConfig = config["Deathmatch"]["Rewards"];
			for (const auto& szitem : RewardsConfig)
			{
				const std::string& Data = szitem["Blueprint"];
				Rewards.Add(Reward(FString(Data.c_str()), (int)szitem["QuantityMin"], (int)szitem["QuantityMax"], (int)szitem["QualityMin"], (int)szitem["QualityMax"], (int)szitem["MinIsBP"], (int)szitem["MaxIsBP"]));
			}
			file.close();
		}
		Init(JoinMessageDelaySeconds + 1);
		WaitForDelay = JoinMessageDelaySeconds;
		WaitCounter = JoinMessages + 1;
	}

	virtual void Update()
	{
		switch (GetState())
		{
		case EventState::WaitingForPlayers:
			if (WaitForTimer(WaitForDelay))
			{
				if (WaitForCounter(WaitCounter))
				{
					if (EventManager::Get().GetEventPlayersCount() < PlayersNeededToStart)
					{
						ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, L"[Event] {} Failed to start needed {} Players", *GetName(), PlayersNeededToStart);
						SetState(EventState::Finished);
					}
					else SetState(EventState::TeleportingPlayers);
				}
				else
				{
					const int Seconds = GetFinalWarning() ? WaitForDelay : ((WaitCounter - GetCounter()) * JoinMessageDelaySeconds);
					ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, L"[Event] {} Starting in {} {}, To Join Type {}", *GetName()
						, (Seconds >= 60 ? (Seconds / 60) : Seconds), (Seconds >= 60 ? (Seconds >= 120 ? L"Minutes" : L"Minute") : L"Seconds"), *JoinEventCommand);

					if (!GetFinalWarning() && GetCounter() == (WaitCounter - 1))
					{
						WaitForDelay = (int)(JoinMessageDelaySeconds / 2);
						SetFinalWarning(true);
					}
				}
				ResetTimer();
			}
			break;
		case EventState::TeleportingPlayers:
			EventManager::Get().TeleportEventPlayers(true, true, true, true, true, true, GetSpawns(), 0);
			EventManager::Get().SendChatMessageToAllEventPlayers(ServerName, L"[Event] {} Starting in 30 Seconds", *GetName());
			SetState(EventState::WaitForFight);
			break;
		case EventState::WaitForFight:
			if (WaitForTimer(30))
			{
				EventManager::Get().EnableEventPlayersInputs();
				EventManager::Get().SendChatMessageToAllEventPlayers(ServerName, L"[Event] {} Started Kill or Be Killed!", *GetName());
				SetState(EventState::Fighting);
			}
			break;
		case EventState::Fighting:
			if (EventManager::Get().GetEventPlayersCount() <= 2) SetState(EventState::Rewarding);
			break;
		case EventState::Rewarding:

			EventManager::Get().TeleportWinningEventPlayersToStart(true);

			if (EventManager::Get().GetEventPlayersCount() > 0)
			{
				AShooterPlayerController* RewardPlayer = EventManager::Get().GetEventPlayers()[0].ASPC;
				if (RewardPlayer && RewardPlayer->GetPlayerCharacter() && !RewardPlayer->GetPlayerCharacter()->IsDead())
				{
					if (Rewards.Num() != 0)
					{
						const Reward& reward = Rewards[FMath::RandRange(0, Rewards.Num() - 1)];
						const int RandomQuantity = (reward.QuantityMin == reward.QuantityMax ? reward.QuantityMin : FMath::RandRange(reward.QuantityMin, reward.QuantityMax))
							, RandomQuality = (reward.QualityMin == reward.QualityMax ? reward.QualityMin : FMath::RandRange(reward.QualityMin, reward.QualityMax));
						const bool IsBP = (reward.MinIsBP == reward.MaxIsBP && reward.MinIsBP == 0 ? false : (reward.MinIsBP == reward.MaxIsBP ? true
							: (FMath::RandRange(reward.MinIsBP, reward.MaxIsBP) == reward.MinIsBP)));
						FString BP = reward.BP;
						RewardPlayer->GiveItem(&BP, RandomQuantity, (float)RandomQuality, IsBP);
					}

					EventManager::Get().SendChatMessageToAllEventPlayers(ServerName, L"[Event] {} has won the {} event!", *ArkApi::GetApiUtils().GetCharacterName(RewardPlayer), *GetName());
				}
			}
			SetState(EventState::Finished);
			break;
		}
	}
};

DeathMatch* DmEvent;

void InitEvent()
{
	Log::Get().Init("Deathmatch Event");
	DmEvent = new DeathMatch();
	EventManager::Get().AddEvent((Event*)DmEvent);
}

void RemoveEvent()
{
	EventManager::Get().RemoveEvent((Event*)DmEvent);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitEvent();
		break;
	case DLL_PROCESS_DETACH:
		RemoveEvent();
		break;
	}
	return TRUE;
}