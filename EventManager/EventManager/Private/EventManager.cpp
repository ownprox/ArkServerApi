#include "EventManager.h"
#include "..\Public\Event.h"

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

	FString& EventManager::GetCurrentEventName()
	{
		if (CurrentEvent) return CurrentEvent->GetName();
		return FString();
	}

	bool EventManager::IsEventOverrideJoinAndLeave()
	{
		if (CurrentEvent) return CurrentEvent->IsEventOverrideJoinAndLeave();
		return false;
	}

	void EventManager::AddEvent(Event* event)
	{
		Events.Add(event);
	}

	void EventManager::RemoveEvent(Event* event)
	{
		if (CurrentEvent && CurrentEvent == event)
		{
			EventRunning = false;
			CurrentEvent = nullptr;
			//NextEventTime = timeGetTime() + 300000;
		}
		Events.Remove(event);
	}

	bool EventManager::StartEvent(const int EventID)
	{
		if (CurrentEvent) return false;
		if (EventID == -1)
		{
			int Rand = 0;
			CurrentEvent = Events[Rand];
			CurrentEvent->InitConfig(JoinEventCommand, ServerName, Map);
			EventRunning = true;
		}
		else if (EventID < Events.Num())
		{
			CurrentEvent = Events[EventID];
			CurrentEvent->InitConfig(JoinEventCommand, ServerName, Map);
			EventRunning = true;
		}
		if (LogToConsole) Log::GetLog()->info("{} Event Started!", CurrentEvent->GetName().ToString().c_str());
		return true;
	}

	EventPlayer* EventManager::FindPlayer(long long PlayerID)
	{
		auto* Player = Players.FindByPredicate([&](EventPlayer& evplayer) { return evplayer.PlayerID == PlayerID; });
		return Player;
	}

	bool EventManager::AddPlayer(AShooterPlayerController* player)
	{
		if (IsEventRunning() && CurrentEvent && CurrentEvent->GetState() == EventState::WaitingForPlayers && !FindPlayer(player->LinkedPlayerIDField()()))
		{
			Players.Add(EventPlayer(player->LinkedPlayerIDField()(), player));
			return true;
		}
		return false;
	}

	bool EventManager::RemovePlayer(AShooterPlayerController* player)
	{
		if (IsEventRunning() && CurrentEvent)
		{
			const int32 Removed = Players.RemoveAll([&](EventPlayer& evplayer) { return evplayer.PlayerID == player->LinkedPlayerIDField()(); });
			return Removed != 0;
		}
		return false;
	}

	void EventManager::Update()
	{
		if (Events.Num() == 0) return;
		if (IsEventRunning() && CurrentEvent)
		{
			if (CurrentEvent->GetState() != Finished) CurrentEvent->Update();
			else
			{
				if (LogToConsole) Log::GetLog()->info("{} Event Ended!", CurrentEvent->GetName().ToString().c_str());
				EventRunning = false;
				CurrentEvent = nullptr;
				NextEventTime = timeGetTime() + 300000;
			}
		}
		else //if (timeGetTime() > NextEventTime)
		{
			if (Map.IsEmpty()) ArkApi::GetApiUtils().GetShooterGameMode()->GetMapName(&Map);
			StartEvent();
			NextEventTime = timeGetTime() + 0;//RandomNumber(7200000, 21600000);
		}
	}

	void EventManager::TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, SpawnsMap& Spawns, const int StartTeam)
	{
		int TeamCount = (int)Spawns.size(), TeamIndex = StartTeam;
		if (TeamCount < 1) return;
		TArray<int> SpawnIndexs;
		for (int i = 0; i < TeamCount; i++) SpawnIndexs.Add(0);
		FVector Pos;
		for (auto& itr : Players)
		{
			if (itr.ASPC && itr.ASPC->PlayerStateField()() && itr.ASPC->GetPlayerCharacter() && !itr.ASPC->GetPlayerCharacter()->IsDead() && (!PreventDinos && itr.ASPC->GetPlayerCharacter()->GetRidingDino() || !itr.ASPC->GetPlayerCharacter()->GetRidingDino()))
			{
				if (WipeInventory)
				{
					UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(itr.ASPC->CheatManagerField()());
					if (cheatManager) cheatManager->ClearPlayerInventory((int)itr.ASPC->LinkedPlayerIDField()(), true, true, true);
				}

				itr.StartPos = ArkApi::GetApiUtils().GetPosition(itr.ASPC);

				Pos = Spawns[TeamIndex][SpawnIndexs[TeamIndex]];
				SpawnIndexs[TeamIndex]++;

				itr.Team = TeamIndex;
				itr.ASPC->SetPlayerPos(Pos.X, Pos.Y, Pos.Z);

				if (TeamCount > 1)
				{
					TeamIndex++;
					if (TeamIndex == TeamCount) TeamIndex = StartTeam;
				}

				if (SpawnIndexs[TeamIndex] == Spawns[TeamIndex].Num()) SpawnIndexs[TeamIndex] = 0;
			}
		}

		Players.RemoveAll([&](EventPlayer& evplayer) { return !evplayer.ASPC || !evplayer.ASPC->GetPlayerCharacter(); });
	}

	void EventManager::TeleportWinningEventPlayersToStart()
	{
		for (auto& itr : Players)
		{
			if (itr.ASPC && itr.ASPC->PlayerStateField()() && itr.ASPC->GetPlayerCharacter() && !itr.ASPC->GetPlayerCharacter()->IsDead())
				itr.ASPC->SetPlayerPos(itr.StartPos.X, itr.StartPos.Y, itr.StartPos.Z);
		}
		Players.Empty();
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

	bool EventManager::CanTakeDamage(long long AttackerID, long long VictimID)
	{
		if (!CurrentEvent) return true; 
		EventPlayer* Attacker, *Victim;
		return ( (Attacker = FindPlayer(AttackerID)) && (Victim = FindPlayer(VictimID)) && AttackerID != VictimID &&
			(Attacker->Team != 0 && Attacker->Team == Victim->Team || CurrentEvent->GetState() != EventState::Fighting));
	}

	void EventManager::OnPlayerDied(long long AttackerID, long long VictimID)
	{
		if (AttackerID != -1 && AttackerID != VictimID)
		{			
			if (EventPlayer* Attacker; (Attacker = FindPlayer(AttackerID))) Attacker->Kills++;
		}
		Players.RemoveAll([&](EventPlayer& evplayer) { return evplayer.PlayerID == VictimID; });
	}

	void EventManager::OnPlayerLogg(AShooterPlayerController* Player)
	{
		if (CurrentEvent && CurrentEvent->GetState() > EventState::TeleportingPlayers)
		{			
			if (EventPlayer* EPlayer; (EPlayer = FindPlayer(Player->LinkedPlayerIDField()())))
			{
				if (CurrentEvent->KillOnLoggout()) Player->ServerSuicide_Implementation();
				else Player->SetPlayerPos(EPlayer->StartPos.X, EPlayer->StartPos.Y, EPlayer->StartPos.Z);
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