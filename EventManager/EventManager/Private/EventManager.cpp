#include "EventManager.h"
#include "..\Public\Event.h"
#include "Points.h"

namespace EventManager
{
	EventManager& EventManager::Get()
	{
		static EventManager instance;
		return instance;
	}

	FString& EventManager::GetServerName()
	{
		return ServerName;
	}

	EventState EventManager::GetEventState()
	{
		return CurrentEvent ? CurrentEvent->GetState() : EventState::Finished;
	}

	FString EventManager::GetCurrentEventName()
	{
		return CurrentEvent ? CurrentEvent->GetName() : L"";
	}

	bool EventManager::IsEventOverrideJoinAndLeave()
	{
		return CurrentEvent ? CurrentEvent->IsEventOverrideJoinAndLeave() : false;
	}

	int EventManager::GetArkShopEntryFee()
	{
		return CurrentEvent ? CurrentEvent->GetArkShopEntryFee() : 0;
	}

	void EventManager::AddEvent(Event* event)
	{
		Events.Add(event);
	}

	void EventManager::RemoveEvent(Event* event)
	{
		if (CurrentEvent && CurrentEvent == event)
		{
			CurrentEvent = nullptr;
			if (!UseSchedule) NextEventTime = timeGetTime() + FMath::RandRange(MinStartEvent, MaxStartEvent);
		}
		Events.Remove(event);
	}

	int EventManager::GetEventsCount()
	{
		return Events.Num();
	}

	bool EventManager::StartEvent(const int EventID)
	{
		if (CurrentEvent) return false;
		if (Map.IsEmpty()) ArkApi::GetApiUtils().GetShooterGameMode()->GetMapName(&Map);
		if (EventID == -1)
		{
			CurrentEvent = Events[FMath::RandRange(0, Events.Num() - 1)];
			CurrentEvent->InitConfig(JoinCommand, ServerName, Map);
		}
		else if (EventID < Events.Num())
		{
			CurrentEvent = Events[EventID];
			CurrentEvent->InitConfig(JoinCommand, ServerName, Map);
		}
		if (LogToConsole) Log::GetLog()->info("{} Event Started!", (CurrentEvent->GetName().IsEmpty() ? "" : CurrentEvent->GetName().ToString().c_str()));
		return true;
	}

	EventPlayer* EventManager::FindPlayer(long long PlayerID)
	{
		auto* Player = Players.FindByPredicate([&](EventPlayer& evplayer) { return evplayer.PlayerID == PlayerID; });
		return Player;
	}

	bool EventManager::AddPlayer(AShooterPlayerController* player)
	{
		if (!FindPlayer(player->LinkedPlayerIDField()))
		{
			Players.Add(EventPlayer(player->LinkedPlayerIDField(), player));
			return true;
		}
		return false;
	}

	bool EventManager::HasPlayer(int PlayerID)
	{
		return FindPlayer(PlayerID) != nullptr;
	}

	bool EventManager::RemovePlayer(AShooterPlayerController* player)
	{
		const int32 Removed = Players.RemoveAll([&](EventPlayer& evplayer) { return evplayer.PlayerID == player->LinkedPlayerIDField(); });
		return Removed != 0;
	}

	void EventManager::Update()
	{
		if (Events.Num() == 0) return;
		if (CurrentEvent)
		{
			if (CurrentEvent->GetState() != Finished) CurrentEvent->Update();
			else
			{
				if (LogToConsole) Log::GetLog()->info("{} Event Ended!", CurrentEvent->GetName().ToString().c_str());
				Players.Empty();
				CurrentEvent = nullptr;
				if (!UseSchedule) NextEventTime = timeGetTime() + FMath::RandRange(MinStartEvent, MaxStartEvent);
			}
		}
		else if (EventStartAuto && !UseSchedule && timeGetTime() > NextEventTime)
		{
			StartEvent();
		}
	}

	void EventManager::TeleportEventPlayers(const bool ApplyFairHp, const bool ApplyFairMovementSpeed, const bool ApplyFairMeleeDamage, const bool DisableInputs, const bool WipeInventoryOrCheckIsNaked, const bool PreventDinos, SpawnsMap& Spawns)
	{
		int TeamCount = (int)Spawns.size(), TeamIndex = 0;
		if (TeamCount < 1) return;
		TArray<int> SpawnIndexs;
		if (TeamCount > 1)
		{
			EventTeamData.Empty();
			for (int i = 0; i < TeamCount; i++)
			{
				SpawnIndexs.Add(0);
				EventTeamData.Add(EventTeam());
			}
			TeamBased = true;
		}
		else
		{
			for (int i = 0; i < TeamCount; i++) SpawnIndexs.Add(0);
			TeamBased = false;
		}
		const float MovementSpeed = CurrentEvent->GetMovementSpeed();
		FVector Pos;
		for (auto& itr : Players)
		{
			if (itr.ASPC && itr.ASPC->PlayerStateField() && itr.ASPC->GetPlayerCharacter() && !itr.ASPC->GetPlayerCharacter()->IsDead() && !itr.ASPC->GetPlayerCharacter()->bIsSleeping()() && !itr.ASPC->GetPlayerCharacter()->IsSitting(false)
				&& !itr.ASPC->GetPlayerCharacter()->bIsCarriedAsPassenger()() && !itr.ASPC->GetPlayerCharacter()->bIsCarried()() && !itr.ASPC->GetPlayerCharacter()->bIsBeingDragged()() && !itr.ASPC->GetPlayerCharacter()->bIsDragging()()
				&& !itr.ASPC->GetPlayerCharacter()->bIsDraggingWithGrapHook()() && !itr.ASPC->GetPlayerCharacter()->bIsImmobilized()() && !itr.ASPC->GetPlayerCharacter()->bIsBeingDraggedByDino()()
				&& (!PreventDinos && itr.ASPC->GetPlayerCharacter()->GetRidingDino() || !itr.ASPC->GetPlayerCharacter()->GetRidingDino()))
			{
				if (WipeInventoryOrCheckIsNaked)
				{
					UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(itr.ASPC->CheatManagerField());
					if (cheatManager) cheatManager->ClearPlayerInventory((int)itr.ASPC->LinkedPlayerIDField(), true, true, true);
				}
				else
				{
					const FString& NakedChkMsg = CheckIfPlayersNaked(itr.ASPC).value_or("");
					if (!NakedChkMsg.IsEmpty())
					{
						ArkApi::GetApiUtils().SendChatMessage(itr.ASPC, EventManager::Get().GetServerName(), *NakedChkMsg);
						itr.Delete = true;
						continue;
					}
				}

				if (DisableInputs)
				{
					itr.ASPC->GetPlayerCharacter()->bPreventMovement() = true;
					itr.ASPC->GetPlayerCharacter()->bPreventJump() = true;
				}

				itr.StartPos = ArkApi::GetApiUtils().GetPosition(itr.ASPC);

				Pos = Spawns[TeamIndex][SpawnIndexs[TeamIndex]];
				SpawnIndexs[TeamIndex]++;

				itr.Team = TeamBased ? TeamIndex + 1 : 0;

				UPrimalCharacterStatusComponent* charStatus = itr.ASPC->GetPlayerCharacter()->GetCharacterStatusComponent();
				if (charStatus)
				{
					if (ApplyFairHp)
					{
						float* health = charStatus->CurrentStatusValuesField()();
						itr.EventPlayerStats.health = *health;
						*health = 100.f;
					}

					float* torpor = charStatus->CurrentStatusValuesField()() + 2;
					*torpor = 1;

					if (ApplyFairMovementSpeed)
					{
						float* speed = charStatus->CurrentStatusValuesField()() + 9;
						itr.EventPlayerStats.speed = *speed;
						*speed = MovementSpeed;
					}

					if (ApplyFairMeleeDamage)
					{
						float* melee = charStatus->CurrentStatusValuesField()() + 8;
						itr.EventPlayerStats.melee = *melee;
						*melee = 100;
					}
				}

				itr.ASPC->SetPlayerPos(Pos.X, Pos.Y, Pos.Z);

				if (TeamCount > 1)
				{
					EventTeamData[TeamIndex].Alive++;
					TeamIndex++;
					if (TeamIndex == TeamCount) TeamIndex = 0;
				}

				if (SpawnIndexs[TeamIndex] == Spawns[TeamIndex].Num()) SpawnIndexs[TeamIndex] = 0;
			}
			else
			{
				itr.Delete = true;
			}
		}

		Players.RemoveAll([&](const auto& evplayer) { return evplayer.Delete; });
	}

	void EventManager::TeleportWinningEventPlayersToStart(const bool WipeInventory)
	{
		UShooterCheatManager* cheatManager;
		for (auto& itr : Players)
		{
			if (itr.ASPC && itr.ASPC->GetPlayerCharacter())
			{
				if (WipeInventory)
				{
					cheatManager = static_cast<UShooterCheatManager*>(itr.ASPC->CheatManagerField());
					if (cheatManager) cheatManager->ClearPlayerInventory((int)itr.ASPC->LinkedPlayerIDField(), true, true, true);
				}
				ResetPlayerStats(&itr, false);
				itr.ASPC->SetPlayerPos(itr.StartPos.X, itr.StartPos.Y, itr.StartPos.Z);
			}
		}
	}

	void EventManager::EnableEventPlayersInputs()
	{
		for (auto& itr : Players)
		{
			if (itr.ASPC && itr.ASPC->PlayerStateField() && itr.ASPC->GetPlayerCharacter() && !itr.ASPC->GetPlayerCharacter()->IsDead())
			{
				itr.ASPC->GetPlayerCharacter()->bPreventMovement() = false;
				itr.ASPC->GetPlayerCharacter()->bPreventJump() = false;
			}
		}
	}

	std::optional<FString> EventManager::CheckIfPlayersNaked(AShooterPlayerController* Player)
	{
		if (!Player || !Player->GetPlayerCharacter() || Player->GetPlayerCharacter()->IsDead()) return PlayerDeadMsg;
		UPrimalInventoryComponent* Inv = Player->GetPlayerCharacter()->MyInventoryComponentField();
		if (!Inv) return InventoryNotFoundMsg;
		FString FoundItem;
		TArray<UPrimalItem *> Items = Inv->InventoryItemsField();
		for (const auto& item : Items)
			if (item && !item->bIsEngram()() && !item->bIsInitialItem()())
			{
				item->GetItemName(&FoundItem, false, false, nullptr);
				return FString::Format(*MustBeNakedMsg, *FoundItem);
			}

		Items = Inv->EquippedItemsField();
		for (const auto& item : Items)
			if (item && !item->bIsEngram()() && !item->bIsInitialItem()())
			{
				item->GetItemName(&FoundItem, false, false, nullptr);
				return FString::Format(*MustBeNakedMsg, *FoundItem);
			}

		Items = Inv->ItemSlotsField();
		for (const auto& item : Items)
			if (item && !item->bIsEngram()() && !item->bIsInitialItem()())
			{
				item->GetItemName(&FoundItem, false, false, nullptr);
				return FString::Format(*MustBeNakedMsg, *FoundItem);
			}

		return {};
	}

	int EventManager::GetRandomIndexNonRecurr(int TotalSize)
	{
		TotalSize = TotalSize - 1;
		int EquipIndex = 0;
		if (TotalSize > 1)
		{
			EquipIndex = (int)FMath::RandRange(0, TotalSize);
			if (EquipIndex == LastEquipmentIndex)
			{
				for (int i = 0; i < 5; i++)
				{
					if ((EquipIndex = (int)FMath::RandRange(0, TotalSize)) != LastEquipmentIndex) break;
				}
			}
		}
		else
		{
			EquipIndex = LastEquipmentIndex++;
			if (EquipIndex > TotalSize) EquipIndex = 0;
		}
		LastEquipmentIndex = EquipIndex;
		return EquipIndex;
	}

	void EventManager::GiveEventPlayersEquipment(const EventEquipment& Equipment)
	{
		UShooterCheatManager* cheatManager;
		for (auto& itr : Players)
		{
			if (!itr.ASPC || !itr.ASPC->GetPlayerCharacter() || itr.ASPC->GetPlayerCharacter()->IsDead()) continue;

			cheatManager = static_cast<UShooterCheatManager*>(itr.ASPC->CheatManagerField());
			for (int i = 0; i < (int)EventArmourType::Max; i++)
			{
				if (Equipment.Armour[i].Quantity != 0)
				{
					FString BP = Equipment.Armour[i].BP;
					if (cheatManager) cheatManager->GiveItemToPlayer((int)itr.ASPC->LinkedPlayerIDField(), &BP, Equipment.Armour[i].Quantity, Equipment.Armour[i].Quality, false);
					//itr.ASPC->GiveItem(&BP, Equipment.Armour[i].Quantity, Equipment.Armour[i].Quality, false);
				}
			}

			for (const auto& Item : Equipment.Items)
			{
				FString BP = Item.BP;
				if (cheatManager) cheatManager->GiveItemToPlayer((int)itr.ASPC->LinkedPlayerIDField(), &BP, Item.Quantity, Item.Quality, false);
				//itr.ASPC->GiveItem(&BP, Item.Quantity, Item.Quality, false);
			}

			int SlotIndex = 0;
			UPrimalInventoryComponent* Inv = itr.ASPC->GetPlayerCharacter()->MyInventoryComponentField();
			if (!Inv) return;
			TArray<UPrimalItem *> Items = Inv->InventoryItemsField();
			for (const auto& item : Items)
			{
				if (item && !item->IsInBlueprint() && !item->bIsEngram()())
				{
					switch (item->MyItemTypeField())
					{
					case EPrimalItemType::Weapon:
						item->AddToSlot(SlotIndex++, true);
						item->UpdatedItem();
						break;
					}
					if (SlotIndex == 10) break;
				}
			}
		}
	}

	void EventManager::ResetPlayerStats(EventPlayer* Player, bool PlayerDied)
	{
		Player->ASPC->bInputEnabled() = true;
		if (TeamBased && PlayerDied)
		{
			const int Team = Player->Team - 1;
			if (Team < EventTeamData.Num()) EventTeamData[Team].Alive--;
		}

		if (Player->ASPC->GetPlayerCharacter() && Player->ASPC->GetPlayerCharacter()->GetCharacterStatusComponent())
		{
			UPrimalCharacterStatusComponent* charStatus = Player->ASPC->GetPlayerCharacter()->GetCharacterStatusComponent();

			float* health = charStatus->CurrentStatusValuesField()();
			*health = Player->EventPlayerStats.health == -1.f ? 99.f : (Player->EventPlayerStats.health < 99.f ? 99.f : (Player->EventPlayerStats.health-1)); //for some reason to fix broken legs after event when your health was 100 you had to -1 health maybe find a function later to resync stuff related to injuries for now -1 health works.

			float* torpor = charStatus->CurrentStatusValuesField()() + 2;
			*torpor = 1;

			if (Player->EventPlayerStats.melee != -1.f)
			{
				float* melee = charStatus->CurrentStatusValuesField()() + 8;
				*melee = Player->EventPlayerStats.melee;
			}

			if (Player->EventPlayerStats.speed != -1.f)
			{
				float* speed = charStatus->CurrentStatusValuesField()() + 9;
				*speed = Player->EventPlayerStats.speed;
			}

			Player->ASPC->GetPlayerCharacter()->bIsSleeping() = false;
			Player->ASPC->GetPlayerCharacter()->bIsDead() = false;
			charStatus->ServerSyncReplicatedValues();
		}
	}

	void EventManager::SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg)
	{
		FChatMessage chat_message = FChatMessage();
		chat_message.SenderName = sender_name;
		chat_message.Message = msg;
		for (EventPlayer ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientChatMessage(chat_message);
	}

	void EventManager::SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
		float display_time, UTexture2D* icon, FString& msg)
	{
		for (EventPlayer ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientServerSOTFNotificationCustom(&msg, color, display_scale, display_time, icon, nullptr);
	}

	bool EventManager::GetEventQueueNotifications()
	{
		return EventQueueNotifications;
	}

	void EventManager::InitConfigs(const FString& ServerName, const FString& JoinCommand, int EventStartMinuteMin, int EventStartMinuteMax, bool DebugLogToConsole
		, const FString& PlayerDeadMsg, const FString& InventoryNotFoundMsg, const FString& MustBeNakedMsg, bool StartEventOnServerStart, bool EventStartAuto)
	{
		this->ServerName = ServerName;
		this->JoinCommand = JoinCommand;
		MinStartEvent = (EventStartMinuteMin * 60000);
		MaxStartEvent = (EventStartMinuteMax * 60000);
		LogToConsole = DebugLogToConsole;
		this->PlayerDeadMsg = PlayerDeadMsg;
		this->InventoryNotFoundMsg = InventoryNotFoundMsg;
		this->MustBeNakedMsg = MustBeNakedMsg;
		if (!StartEventOnServerStart) NextEventTime = timeGetTime() + FMath::RandRange(MinStartEvent, MaxStartEvent);
		this->EventStartAuto = EventStartAuto;
	}

	bool EventManager::ArkShopSpendPoints(int amount, int PlayerID)
	{
		return ArkShop::Points::SpendPoints(amount, ArkApi::GetApiUtils().GetShooterGameMode()->GetSteamIDForPlayerID(PlayerID));
	}

	int EventManager::ArkShopGetPoints(int PlayerID)
	{
		return ArkShop::Points::GetPoints(ArkApi::GetApiUtils().GetShooterGameMode()->GetSteamIDForPlayerID(PlayerID));
	}

	void EventManager::ArkShopAddPoints(int amount, int PlayerID)
	{
		const uint64 steam_id = ArkApi::GetApiUtils().GetShooterGameMode()->GetSteamIDForPlayerID(PlayerID);
		if (LogToConsole) Log::GetLog()->info("SteamID: {} adding {} Points!", steam_id, amount);
		ArkShop::Points::AddPoints(amount, steam_id);
	}

	bool EventManager::CanTakeDamage(long long AttackerID, long long VictimID)
	{
		if (!CurrentEvent) return true;
		if (CurrentEvent->GetState() == EventState::Fighting || CurrentEvent->GetState() == EventState::WaitForFight)
		{
			EventPlayer* Attacker, *Victim;
			if ((Attacker = FindPlayer(AttackerID)) && (Victim = FindPlayer(VictimID)) && AttackerID != VictimID)
			{
				return CurrentEvent->GetState() == EventState::Fighting && (Attacker->Team == 0 || Attacker->Team != Victim->Team);
			}
		}
		return true;
	}

	bool EventManager::OnPlayerDied(long long AttackerID, long long VictimID)
	{
		if (VictimID != -1)
		{
			EventPlayer* Player;
			if (AttackerID != VictimID && AttackerID != -1 && (Player = FindPlayer(AttackerID))) Player->Kills++;

			if ((Player = FindPlayer(VictimID)))
			{
				if (Player->ASPC && Player->ASPC->GetPlayerCharacter())
				{
					UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(Player->ASPC->CheatManagerField());
					if (cheatManager) cheatManager->ClearPlayerInventory((int)Player->ASPC->LinkedPlayerIDField(), true, true, true);
					ResetPlayerStats(Player);
					Player->ASPC->SetPlayerPos(Player->StartPos.X, Player->StartPos.Y, Player->StartPos.Z);
				}
				Players.RemoveAll([&](EventPlayer& evplayer) { return evplayer.PlayerID == Player->PlayerID; });
				return true;
			}
		}
		return false;
	}

	void EventManager::OnPlayerLogg(AShooterPlayerController* Player)
	{
		if (CurrentEvent && CurrentEvent->GetState() > EventState::TeleportingPlayers)
		{
			if (EventPlayer* EPlayer; (EPlayer = FindPlayer(Player->LinkedPlayerIDField())))
			{
				if (CurrentEvent->KillOnLoggout()) Player->ServerSuicide_Implementation();
				else if (EPlayer->ASPC && EPlayer->ASPC->GetPlayerCharacter())
				{
					UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(EPlayer->ASPC->CheatManagerField());
					if (cheatManager) cheatManager->ClearPlayerInventory((int)EPlayer->ASPC->LinkedPlayerIDField(), true, true, true);
					ResetPlayerStats(EPlayer);
					EPlayer->ASPC->SetPlayerPos(EPlayer->StartPos.X, EPlayer->StartPos.Y, EPlayer->StartPos.Z);
				}
				else Player->ServerSuicide_Implementation();
				RemovePlayer(Player);
			}
		}
	}

	bool EventManager::IsEventProtectedStructure(const FVector& StructurePos)
	{
		for (Event* Evt : Events) if (Evt->IsEventProtectedStructure(StructurePos)) return true;
		return false;
	}

	// Free function
	IEventManager& Get()
	{
		return EventManager::Get();
	}
}