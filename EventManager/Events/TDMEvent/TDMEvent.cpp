#pragma once
#include "TDMEvent.h"
#include "../../EventManager/Public/Event.h"
#include "../../EventManager/Public/IEventManager.h"
#pragma comment(lib, "AZEventManager.lib")
#pragma comment(lib, "ArkApi.lib")
#include <fstream>
#include "json.hpp"

class TDMEvent : public Event
{
private:
	bool Notifications;
	int ArkShopPointsRewardMin, ArkShopPointsRewardMax, JoinMessages, JoinMessageDelaySeconds, PlayersNeededToStart, WaitForDelay, WaitCounter, LastEquipmentIndex = -1;
	FString JoinEventCommand, ServerName, Messages[13];

	struct Reward
	{
		const FString BP;
		const int QuantityMin, QuantityMax, QualityMin, QualityMax, MinIsBP, MaxIsBP;
		Reward(const FString BP, const int QuantityMin, const int QuantityMax, const int QualityMin, const int QualityMax, const int MinIsBP, const int MaxIsBP) : BP(BP), QuantityMin(QuantityMin)
			, QuantityMax(QuantityMax), QualityMin(QualityMin), QualityMax(QualityMax), MinIsBP(MinIsBP), MaxIsBP(MaxIsBP) {}
	};

	TArray<Reward> Rewards;
	TArray<EventManager::EventEquipment> Equipments;
public:
	virtual void InitConfig(const FString& JoinEventCommand, const FString& ServerName, const FString& Map)
	{
		this->JoinEventCommand = JoinEventCommand;
		this->ServerName = ServerName;
		if (!HasConfigLoaded())
		{
			Equipments.Empty();
			Rewards.Empty();
			ClearSpawns();

			const std::string config_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/TeamDeathMatchEvent/" + Map.ToString() + ".json";
			std::ifstream file { config_path };
			if (!file.is_open()) throw std::runtime_error(fmt::format("Can't open {}.json", Map.ToString().c_str()).c_str());
			nlohmann::json config;
			file >> config;
			const std::string eName = config["TDM"]["EventName"];
			const FString& EventName = ArkApi::Tools::Utf8Decode(eName).c_str();

			PlayersNeededToStart = config["TDM"]["PlayersNeededToStart"];

			JoinMessages = config["TDM"]["JoinMessages"];
			JoinMessageDelaySeconds = config["TDM"]["JoinMessageDelaySeconds"];

			const bool StructureProtection = config["TDM"]["StructureProtection"];
			const auto StructureProtectionPosition = config["TDM"]["StructureProtectionPosition"];
			const int StructureProtectionDistacne = config["TDM"]["StructureProtectionDistance"];

			Notifications = config["TDM"]["TopNotifications"];
			const float MovementSpeedAddon = config["TDM"]["MovementSpeedAddon"];
			const int ArkShopPointsEntryFee = config["TDM"]["ArkShopPointsEntryFee"];

			ArkShopPointsRewardMin = config["TDM"]["ArkShopPointsRewardMin"];
			ArkShopPointsRewardMax = config["TDM"]["ArkShopPointsRewardMax"];
			if (ArkShopPointsRewardMin > ArkShopPointsRewardMax) ArkShopPointsRewardMax = ArkShopPointsRewardMin;
			if (ArkShopPointsRewardMin < 0) ArkShopPointsRewardMin = 0;

			const bool KillOnLogg = config["TDM"]["KillOnLoggout"];
			std::string Data;

			InitDefaults(EventName, false, true, KillOnLogg, StructureProtection
				, FVector(StructureProtectionPosition[0], StructureProtectionPosition[1], StructureProtectionPosition[2]), StructureProtectionDistacne, MovementSpeedAddon, ArkShopPointsEntryFee);

			const auto& SpawnsA = config["TDM"]["TeamASpawns"];
			for (const auto& Spawn : SpawnsA)
			{
				const auto& SpawnPos = Spawn["Position"];
				AddSpawn(FVector(SpawnPos[0], SpawnPos[1], SpawnPos[2]));
			}

			const auto& SpawnsB = config["TDM"]["TeamBSpawns"];
			for (const auto& Spawn : SpawnsB)
			{
				const auto& SpawnPos = Spawn["Position"];
				AddSpawn(FVector(SpawnPos[0], SpawnPos[1], SpawnPos[2]), 1);
			}
			
			const auto& RewardsConfig = config["TDM"]["Rewards"];
			for (const auto& szitem : RewardsConfig)
			{
				Data = szitem["Blueprint"];
				Rewards.Add(Reward(FString(Data.c_str()), (int)szitem["QuantityMin"], (int)szitem["QuantityMax"], (int)szitem["QualityMin"], (int)szitem["QualityMax"], (int)szitem["MinIsBP"], (int)szitem["MaxIsBP"]));
			}
			
			const auto& EquipmentConfig = config["TDM"]["Equipment"];
			for (const auto& Equipment : EquipmentConfig)
			{
				TArray<EventManager::EventItem> Items;
				EventManager::EventItem Armour[EventManager::EventArmourType::Max];
				Data = Equipment["HeadBP"];
				Armour[EventManager::EventArmourType::Head] = EventManager::EventItem(Data.c_str(), 1, 0);

				Data = Equipment["TorsoBP"];
				Armour[EventManager::EventArmourType::Torso] = EventManager::EventItem(Data.c_str(), 1, 0);

				Data = Equipment["GlovesBP"];
				Armour[EventManager::EventArmourType::Gloves] = EventManager::EventItem(Data.c_str(), 1, 0);

				Data = Equipment["OffhandBP"];
				Armour[EventManager::EventArmourType::Offhand] = EventManager::EventItem(Data.c_str(), 1, 0);

				Data = Equipment["LegsBP"];
				Armour[EventManager::EventArmourType::Legs] = EventManager::EventItem(Data.c_str(), 1, 0);

				Data = Equipment["FeetBP"];
				Armour[EventManager::EventArmourType::Feet] = EventManager::EventItem(Data.c_str(), 1, 0);

				const auto& ItemConfig = Equipment["Items"];
				for (const auto& Item : ItemConfig)
				{
					Data = Item["BP"];
					Items.Add(EventManager::EventItem(Data.c_str(), Item["Quantity"], Item["Quality"]));
				}				
				Equipments.Add(EventManager::EventEquipment(Items, Armour));
			}


			int j = 0;
			const auto& Msgs = config["TDM"]["Messages"];
			for (const auto& Msg : Msgs)
			{
				Data = Msg;
				Messages[j++] = ArkApi::Tools::Utf8Decode(Data).c_str();
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
						ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, *Messages[4], *GetName(), PlayersNeededToStart);
						SetState(EventState::Finished);
					}
					else SetState(EventState::TeleportingPlayers);
				}
				else
				{
					const int Seconds = GetFinalWarning() ? WaitForDelay : ((WaitCounter - GetCounter()) * JoinMessageDelaySeconds);
					ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, *Messages[0], *GetName()
						, (Seconds >= 60 ? (Seconds / 60) : Seconds), (Seconds >= 60 ? (Seconds >= 120 ? *Messages[1] : *Messages[2]) : *Messages[3]), *JoinEventCommand);

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
			EventManager::Get().TeleportEventPlayers(true, true, true, true, false, true, GetSpawns());
			EventManager::Get().SendChatMessageToAllEventPlayers(ServerName, *Messages[5], *GetName());
			if (Equipments.Num() > 0)
			{
				EventManager::Get().GiveEventPlayersEquipment(Equipments[EventManager::Get().GetRandomIndexNonRecurr(Equipments.Num())]);
			}
			SetState(EventState::WaitForFight);
			break;
		case EventState::WaitForFight:
			if (Notifications) EventManager::Get().SendNotificationToAllEventPlayers(FLinearColor(0, 1, 1), 1.f, 1, nullptr, *Messages[11]);
			if (WaitForTimer(30))
			{
				EventManager::Get().EnableEventPlayersInputs();
				const auto& Players = EventManager::Get().GetEventPlayers();
				for (const auto& player : Players)
				{
					if(player.ASPC) ArkApi::GetApiUtils().SendChatMessage(player.ASPC, ServerName, *Messages[6], *GetName(), (player.Team == 1 ? *Messages[9] : *Messages[10]));
				}
				SetState(EventState::Fighting);
			}
			break;
		case EventState::Fighting:
			{
				const int RedPlayers = EventManager::Get().GetTeamAliveCount(0), BluePlayers = EventManager::Get().GetTeamAliveCount(1);
				if (Notifications) EventManager::Get().SendNotificationToAllEventPlayers(FLinearColor(0, 1, 0), 1.f, 1.f, nullptr, *Messages[12], *GetName(), *Messages[9], RedPlayers, *Messages[10], BluePlayers);
				if (RedPlayers == 0 || BluePlayers == 0) SetState(EventState::Rewarding);
			}
			break;
		case EventState::Rewarding:
			if (EventManager::Get().GetEventPlayersCount() > 0)
			{
				EventManager::Get().TeleportWinningEventPlayersToStart(true);

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
						//RewardPlayer->GiveItem(&BP, RandomQuantity, (float)RandomQuality, IsBP);
						UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(RewardPlayer->CheatManagerField());
						if (cheatManager) cheatManager->GiveItemToPlayer((int)RewardPlayer->LinkedPlayerIDField(), &BP, RandomQuantity, (float)RandomQuality, IsBP);
					}

					if (ArkShopPointsRewardMax > 0) EventManager::Get().ArkShopAddPoints(FMath::RandRange(ArkShopPointsRewardMin, ArkShopPointsRewardMax), (int)RewardPlayer->LinkedPlayerIDField());

					ArkApi::GetApiUtils().SendChatMessageToAll(ServerName, (EventManager::Get().GetTeamAliveCount(0) ? *Messages[7] : *Messages[8]), *GetName());
				}
			}
			SetState(EventState::Finished);
			break;
		}
	}
}; 

TDMEvent* TDmEvent;
void DMReload(APlayerController* player_controller, FString* message, bool LogToFile)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	TDmEvent->ResetConfigLoaded();
	ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), "Config Reloaded!");
}

void InitEvent()
{
	Log::Get().Init("Team Deathmatch Event");
	TDmEvent = new TDMEvent();
	EventManager::Get().AddEvent((Event*)TDmEvent);
	ArkApi::GetCommands().AddConsoleCommand("tdmreload", &DMReload);
}

void RemoveEvent()
{
	EventManager::Get().RemoveEvent((Event*)TDmEvent);
	ArkApi::GetCommands().RemoveConsoleCommand("tdmreload");
	if (TDmEvent != nullptr) delete[] TDmEvent;
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