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
		if (Map.IsEmpty())
		{
			ArkApi::GetApiUtils().GetShooterGameMode()->GetMapName(&Map);
			Map = Map.Replace(L"_P", L"");
		}
		if (EventID == -1)
		{
			CurrentEvent = Events[CurrentEventID++];
			CurrentEvent->InitConfig(JoinCommand, ServerName, Map);
			if (CurrentEventID >= Events.Num())
				CurrentEventID = 0;
		}
		else if (EventID < Events.Num())
		{
			CurrentEvent = Events[EventID];
			CurrentEvent->InitConfig(JoinCommand, ServerName, Map);
		}
		else
		{
			CurrentEvent = Events[FMath::RandRange(0, Events.Num() - 1)];
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

	bool EventManager::HasPlayer(const int PlayerID)
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
		else if (ScheduleCheckCounter++ >= 20)
		{
			ScheduleCheckCounter = 0;
			time(&rawtime);
			localtime_s(&timeinfo, &rawtime);
			timeinfo.tm_min = timeinfo.tm_hour = timeinfo.tm_sec = 0;
			const time_t Tm = mktime(&timeinfo);
			const auto Now = std::chrono::system_clock::from_time_t(Tm);
			const auto RealNow = std::chrono::system_clock::now();

			for (int i = 0; i < Schedules.Num(); i++)
			{
				const auto StartTime = Now + std::chrono::hours((Schedules[i].StartDay - timeinfo.tm_wday + 7) % 7 * 24) + std::chrono::
					minutes(
					(Schedules[i].StartHour - timeinfo.tm_hour + 24) % 24 * 60);
				const auto EndTime = StartTime + std::chrono::minutes(2);

				if ((RealNow >= StartTime || StartTime > EndTime) && RealNow < EndTime)
				{
					int EventID = -1;
					if (Schedules[i].EventID == -1)
					{
						Schedules[i].EventID = GetEventID(Schedules[i].EventName);
						if (Schedules[i].EventID == -1)
						{
							Log::GetLog()->warn("{} Event Not Found!");
							return;
						}
					}
					StartEvent(Schedules[i].EventID);
					return;
				}
			}
		}
		else if (EventStartAuto && timeGetTime() > NextEventTime)
		{
			StartEvent();
		}
	}

	bool EventManager::TeleportEventPlayers(const bool ApplyFairHp, const bool ApplyFairMovementSpeed, const bool ApplyFairMeleeDamage, const bool DisableInputs, const bool WipeInventoryOrCheckIsNaked, const bool PreventDinos, SpawnsMap& Spawns)
	{
		int TeamCount = (int)Spawns.size(), TeamIndex = 0;
		if (TeamCount < 1) return false;
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
		bool RemovePlayers = false;
		UShooterCheatManager* cheatManager;
		for (auto& player : Players)
		{
			if (player.ASPC && player.ASPC->PlayerStateField() && player.ASPC->GetPlayerCharacter() && !player.ASPC->GetPlayerCharacter()->bIsSleeping()() && !player.ASPC->GetPlayerCharacter()->IsSitting(false)
				&& !player.ASPC->GetPlayerCharacter()->bIsCarriedAsPassenger()() && !player.ASPC->GetPlayerCharacter()->bIsCarried()() && !player.ASPC->GetPlayerCharacter()->bIsBeingDragged()() && !player.ASPC->GetPlayerCharacter()->bIsDragging()()
				&& !player.ASPC->GetPlayerCharacter()->bIsDraggingWithGrapHook()() && !player.ASPC->GetPlayerCharacter()->bIsImmobilized()() && !player.ASPC->GetPlayerCharacter()->bIsBeingDraggedByDino()())
			{
				if (PreventDinos && player.ASPC->GetPlayerCharacter()->GetRidingDino())
				{
					/*player.DinoID1 = player.ASPC->GetPlayerCharacter()->GetRidingDino()->DinoID1Field();
					player.DinoID2 = player.ASPC->GetPlayerCharacter()->GetRidingDino()->DinoID2Field();*/
					player.DinoSeat = player.ASPC->GetPlayerCharacter()->GetRidingDino()->GetSeatIndexForPassenger(player.ASPC->GetPlayerCharacter());
					player.ASPC->GetPlayerCharacter()->GetRidingDino()->ServerCallStay_Implementation();
					if (player.DinoSeat >= 0)
					{
						player.ASPC->GetPlayerCharacter()->GetRidingDino()->RemovePassenger(player.ASPC->GetPlayerCharacter(), true, false);
					}
					else
					{
						player.ASPC->GetPlayerCharacter()->GetRidingDino()->ServerClearRider_Implementation(0);
					}
				}

				player.ASPC->GetPlayerCharacter()->ClearMountedDino(false);
				cheatManager = static_cast<UShooterCheatManager*>(player.ASPC->CheatManagerField());

				if (cheatManager)
				{
					cheatManager->ClearMyBuffs();
					cheatManager->ClearCryoSickness();
				}

				if (WipeInventoryOrCheckIsNaked)
				{
					if (cheatManager) cheatManager->ClearPlayerInventory((int)player.ASPC->LinkedPlayerIDField(), true, true, true);
				}
				else
				{
					const FString& NakedChkMsg = CheckIfPlayersNaked(player.ASPC).value_or("");
					if (!NakedChkMsg.IsEmpty())
					{
						ArkApi::GetApiUtils().SendChatMessage(player.ASPC, EventManager::Get().GetServerName(), *NakedChkMsg);
						if (LogToConsole) Log::GetLog()->info("Removing: {} {} ({})!", ArkApi::GetApiUtils().GetCharacterName(player.ASPC).ToString().c_str(), NakedChkMsg.ToString(), Players.Num());
						RemovePlayers = player.Delete = true;
						continue;
					}
				}

				player.StartPos = ArkApi::GetApiUtils().GetPosition(player.ASPC);

				UPrimalCharacterStatusComponent* charStatus = player.ASPC->GetPlayerCharacter()->GetCharacterStatusComponent();
				if (charStatus)
				{
					if (ApplyFairHp)
					{
						float* health = charStatus->CurrentStatusValuesField()();
						player.EventPlayerStats.health = *health;
						*health = 100.f;
					}

					float* torpor = charStatus->CurrentStatusValuesField()() + 2;
					*torpor = 1;

					if (ApplyFairMovementSpeed)
					{
						float* speed = charStatus->CurrentStatusValuesField()() + 9;
						player.EventPlayerStats.speed = *speed;
						*speed = MovementSpeed;
					}

					if (ApplyFairMeleeDamage)
					{
						float* melee = charStatus->CurrentStatusValuesField()() + 8;
						player.EventPlayerStats.melee = *melee;
						*melee = 100;
					}
				}

			}
			else
			{
				if (GetArkShopEntryFee() != 0)
					ArkShopAddPoints(GetArkShopEntryFee(), (int)player.PlayerID);

				RemovePlayers = player.Delete = true;

				if (LogToConsole) Log::GetLog()->info("Removing: {} dont meet conditions ({})!", ArkApi::GetApiUtils().GetCharacterName(player.ASPC).ToString().c_str(), Players.Num());
			}
		}

		if (RemovePlayers)
			Players.RemoveAll([&](const auto& evplayer) { return evplayer.Delete; });

		if (Players.Num() < CurrentEvent->GetPlayersNeededToStart())
		{
			if (GetArkShopEntryFee() != 0)
			{
				for (auto& player : Players)
					ArkShopAddPoints(GetArkShopEntryFee(), (int)player.PlayerID);
			}
			return false;
		}

		for (auto& player : Players)
		{
			if (DisableInputs)
			{
				player.ASPC->GetPlayerCharacter()->bPreventMovement() = true;
				player.ASPC->GetPlayerCharacter()->bPreventJump() = true;
			}

			Pos = Spawns[TeamIndex][SpawnIndexs[TeamIndex]];
			SpawnIndexs[TeamIndex]++;

			player.Team = TeamBased ? TeamIndex + 1 : 0;
			player.ASPC->SetPlayerPos(Pos.X, Pos.Y, Pos.Z);
			player.TeledPos = Pos;

			if (TeamCount > 1)
			{
				EventTeamData[TeamIndex].Alive++;
				TeamIndex++;
				if (TeamIndex == TeamCount) TeamIndex = 0;
			}

			if (SpawnIndexs[TeamIndex] == Spawns[TeamIndex].Num()) SpawnIndexs[TeamIndex] = 0;
		}
		return true;
	}

	void EventManager::TeleportHome(const EventPlayer& player, const bool WipeInventory, const bool PlayerDied)
	{
		if (TeamBased && PlayerDied)
		{
			const int Team = player.Team - 1;
			if (Team < EventTeamData.Num()) EventTeamData[Team].Alive--;
		}
		if (WipeInventory)
		{
			UShooterCheatManager* cheatManager;
			cheatManager = static_cast<UShooterCheatManager*>(player.ASPC->CheatManagerField());
			if (cheatManager) cheatManager->ClearPlayerInventory((int)player.ASPC->LinkedPlayerIDField(), true, true, true);
		}
		ResetPlayerStats(player, false);
		player.ASPC->SetPlayerPos(player.StartPos.X, player.StartPos.Y, player.StartPos.Z);
		/*if (player.DinoID1 != -1)
		{
			APrimalDinoCharacter* Dino = APrimalDinoCharacter::FindDinoWithID(ArkApi::GetApiUtils().GetWorld(), player.DinoID1, player.DinoID2);
			if (Dino)
			{
				if (player.DinoSeat >= 0)
				{
					if (Dino->IsPassengerSeatAvailable(player.DinoSeat))
						Dino->AddPassenger(player.ASPC->GetPlayerCharacter(), player.DinoSeat, true, true);
					else
					{
						const int SeatIndex = Dino->GetSeatIndexForPassenger(player.ASPC->GetPlayerCharacter());
						if(SeatIndex >= 0)
							Dino->AddPassenger(player.ASPC->GetPlayerCharacter(), SeatIndex, true, true);
					}
				}
				else
				{
					player.ASPC->GetPlayerCharacter()->SetRidingDino(Dino);
				}
			}
		}*/
	}

	void EventManager::TeleportWinningEventPlayersToStart(const bool WipeInventory)
	{
		for (auto& player : Players)
		{
			if (player.ASPC && player.ASPC->GetPlayerCharacter())
				TeleportHome(player, WipeInventory, false);
		}
	}

	void EventManager::EnableInputs()
	{
		bool RemovePlayers = false;
		for (auto& player : Players)
		{
			if (player.ASPC && player.ASPC->PlayerStateField() && player.ASPC->GetPlayerCharacter())
			{
				player.ASPC->GetPlayerCharacter()->bPreventMovement() = false;
				player.ASPC->GetPlayerCharacter()->bPreventJump() = false;
			}
			else
			{
				RemovePlayers = player.Delete = true;
			}
		}

		if (RemovePlayers)
		{
			Players.RemoveAll([&](const auto& evplayer) { return evplayer.Delete; });
		}

		const int PlayersNeeded = (CurrentEvent && CurrentEvent->GetPlayersNeededToStart() || 1);
		bCanRewardWinner = Players.Num() >= PlayersNeeded;
	}


	std::optional<FString> EventManager::CheckIfPlayersNaked(AShooterPlayerController* Player)
	{
		if (!Player || !Player->GetPlayerCharacter()) return PlayerDeadMsg;
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
		int EquipIndex = 0;
		if (TotalSize > 0)
		{
			EquipIndex = (int)FMath::RandRange(0, TotalSize);
			if (EquipIndex == LastEquipmentIndex)
			{
				for (int i = 0; i < TotalSize+1; i++)
				{
					if ((EquipIndex = (int)FMath::RandRange(0, TotalSize)) != LastEquipmentIndex) break;
				}
			}
		}
		else
		{
			return 0;
		}
		LastEquipmentIndex = EquipIndex;
		return EquipIndex;
	}

	void EventManager::UpdateItemColours(const short ItemColour, const TArray<UPrimalItem*>& Items)
	{
		for (const auto& item : Items)
		{
			if (item && !item->bIsEngram()() && !item->bIsInitialItem()())
			{
				for (int i = 0; i < 6; i++)
				{
					switch (item->MyItemTypeField())
					{
					case EPrimalItemType::Equipment:
					case EPrimalItemType::Weapon:
						*(item->bUseItemColorField()() + i) = true;
						*(item->ItemColorIDField()() + i) = ItemColour;
						break;
					}
				}
				item->UpdatedItem(false);
			}
		}
	}


	void EventManager::SetItemStatValue(UPrimalItem* item, EPrimalItemStat::Type item_stat_type, const float new_value)
	{
		*(item->ItemStatValuesField()() + item_stat_type) = 1;
		*(item->ItemStatValuesField()() + item_stat_type) = static_cast<unsigned short>((item->GetItemStatModifier(item_stat_type) * new_value) / 2);
		switch (item_stat_type)
		{
		case EPrimalItemStat::MaxDurability:
			if (item->bUseItemDurability()())
				item->ItemDurabilityField() = item->GetItemStatModifier(item_stat_type);
			break;
		}
	}

	void EventManager::GiveEventPlayersEquipment(const EventEquipment& Equipment)
	{
		bool RemovePlayers = false;
		for (auto& player : Players)
		{
			if (!player.ASPC || !player.ASPC->GetPlayerCharacter() || FVector::Distance(ArkApi::GetApiUtils().GetPosition(player.ASPC), player.TeledPos) > 4000)
			{
				RemovePlayers = player.Delete = true;
				if (LogToConsole) Log::GetLog()->info("{} removed was not teleported, removed!", (player.ASPC ? ArkApi::GetApiUtils().GetCharacterName(player.ASPC).ToString() : ""));
				continue;
			}

			for (const auto& Armour : Equipment.Armour)
			{
				if (Armour.Quantity != 0)
				{
					FString BP = Armour.BP;
					TArray<UPrimalItem*> SpawnedItems;
					player.ASPC->GiveItem(&SpawnedItems, &BP, Armour.Quantity, Armour.Quality, false, true, 0);
					if ((Armour.Dura > 0 || Armour.Armour > 0) && SpawnedItems.Num() > 0)
					{
						SetItemStatValue(SpawnedItems[0], EPrimalItemStat::Armor, Armour.Armour);
						SetItemStatValue(SpawnedItems[0], EPrimalItemStat::MaxDurability, Armour.Dura);
						SpawnedItems[0]->UpdatedItem(false);
					}
				}
			}

			for (const auto& Item : Equipment.Items)
			{
				FString BP = Item.BP;
				TArray<UPrimalItem*> SpawnedItems;
				player.ASPC->GiveItem(&SpawnedItems, &BP, Item.Quantity, Item.Quality, false, false, 0);
				if ((Item.Dura > 0 || Item.AmmoClip > 0 || Item.DMG > 0) && SpawnedItems.Num() > 0)
				{
					SetItemStatValue(SpawnedItems[0], EPrimalItemStat::MaxDurability, Item.Dura);
					SetItemStatValue(SpawnedItems[0], EPrimalItemStat::WeaponClipAmmo, Item.AmmoClip);
					SetItemStatValue(SpawnedItems[0], EPrimalItemStat::WeaponDamagePercent, Item.DMG);
					SpawnedItems[0]->UpdatedItem(false);
				}
			}

			UPrimalInventoryComponent* Inv = player.ASPC->GetPlayerCharacter()->MyInventoryComponentField();
			if (!Inv) continue;

			if (TeamBased)
			{
				const short ItemCol = player.Team == 1 ? 59 : 61;
				UpdateItemColours(ItemCol, Inv->InventoryItemsField());
				UpdateItemColours(ItemCol, Inv->EquippedItemsField());
			}

			int SlotIndex = 0;
			const auto& Items = Inv->InventoryItemsField();
			for (const auto& item : Items)
			{
				if (item && !item->IsInBlueprint() && !item->bIsEngram()())
				{
					switch (item->MyItemTypeField())
					{
					case EPrimalItemType::Weapon:
						item->AddToSlot(SlotIndex++, true);
						item->WeaponClipAmmoField() = item->WeaponTotalAmmoField();
						if (SlotIndex == 1)
							item->EquippedItem();
						item->UpdatedItem(false);
						break;
					}
					if (SlotIndex == 10) break;
				}
			}
		}

		if (RemovePlayers)
		{
			Players.RemoveAll([&](const auto& evplayer) 
			{ 
				if (evplayer.Delete)
				{
					TeleportHome(evplayer, false, true);
					return true;
				}
				return false; 
			});
		}
	}

	void EventManager::ResetPlayerStats(EventPlayer player, const bool PlayerDied)
	{
		player.ASPC->bInputEnabled() = true;

		if (player.ASPC->GetPlayerCharacter() && player.ASPC->GetPlayerCharacter()->GetCharacterStatusComponent())
		{
			UPrimalCharacterStatusComponent* charStatus = player.ASPC->GetPlayerCharacter()->GetCharacterStatusComponent();

			float* health = charStatus->CurrentStatusValuesField()();
			*health = player.EventPlayerStats.health == -1.f ? 99.f : (player.EventPlayerStats.health < 99.f ? 99.f : (player.EventPlayerStats.health-1)); //for some reason to fix broken legs after event when your health was 100 you had to -1 health maybe find a function later to resync stuff related to injuries for now -1 health works.

			float* torpor = charStatus->CurrentStatusValuesField()() + 2;
			*torpor = 1;

			if (player.EventPlayerStats.melee != -1.f)
			{
				float* melee = charStatus->CurrentStatusValuesField()() + 8;
				*melee = player.EventPlayerStats.melee;
			}

			if (player.EventPlayerStats.speed != -1.f)
			{
				float* speed = charStatus->CurrentStatusValuesField()() + 9;
				*speed = player.EventPlayerStats.speed;
			}

			player.ASPC->GetPlayerCharacter()->bIsSleeping() = false;
			player.ASPC->GetPlayerCharacter()->bIsDead() = false;
			charStatus->ServerSyncReplicatedValues();
		}
	}

	void EventManager::SendChatMessageToAllEventPlayersInternal(const FString& sender_name, const FString& msg)
	{
		FChatMessage chat_message = FChatMessage();
		chat_message.SenderName = sender_name;
		chat_message.Message = msg;
		for (const auto& ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientChatMessage(chat_message);
	}

	void EventManager::SendNotificationToAllEventPlayersInternal(FLinearColor color, float display_scale,
		float display_time, UTexture2D* icon, FString& msg)
	{
		for (const auto& ePlayer : Players) if (ePlayer.ASPC) ePlayer.ASPC->ClientServerSOTFNotificationCustom(&msg, color, display_scale, display_time, icon, nullptr);
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
				if (Player->ASPC && Player->ASPC->GetPlayerCharacter()) TeleportHome(*Player, true, true);
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
				else if (EPlayer->ASPC && EPlayer->ASPC->GetPlayerCharacter()) TeleportHome(*EPlayer, true, true);
				else Player->ServerSuicide_Implementation();
				RemovePlayer(Player);
			}
		}
	}

	bool EventManager::IsEventProtectedStructure(const FVector& StructurePos)
	{
		for (const auto& Evt : Events) 
			if (Evt->IsEventProtectedStructure(StructurePos)) 
				return true;
		return false;
	}

	// Free function
	IEventManager& Get()
	{
		return EventManager::Get();
	}
}