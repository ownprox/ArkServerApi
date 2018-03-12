#include "EventMan.h"
#include "..\Public\Event.h"

EventMan& EventMan::Get()
{
	static EventMan instance;
	return instance;
}

void EventMan::AddEvent(Event event)
{
	Events.push_back(event);
}

void EventMan::RemoveEvent(Event event)
{
	const FString EventName = event.GetName();
	EventItr Itr = std::find_if(Events.begin(), Events.end(), [EventName](Event e) -> bool { return e.GetName().Equals(EventName); });
	if (Itr != Events.end()) Events.erase(Itr);
}


bool EventMan::AddPlayer(long long PlayerID, AShooterPlayerController* player)
{
	if (IsEventRunning() && CurrentEvent != nullptr && CurrentEvent->GetState() == EventState::WaitingForPlayers && FindPlayer(PlayerID) == nullptr)
	{
		Players.push_back(EventPlayer(PlayerID, player));
		return true;
	}
	return false;
}

EventPlayer* EventMan::FindPlayer(long long PlayerID)
{
	EventPlayerArrayItr Itr = std::find_if(Players.begin(), Players.end(), [PlayerID](EventPlayer p) -> bool { return p.PlayerID == PlayerID; });
	if (Itr != Players.end()) return &(*Itr);
	return nullptr;
}

bool EventMan::RemovePlayer(long long PlayerID, bool ByCommand)
{
	if (IsEventRunning() && CurrentEvent != nullptr && (CurrentEvent->GetState() == EventState::WaitingForPlayers && ByCommand || !ByCommand))
	{
		EventPlayerArrayItr Itr = std::find_if(Players.begin(), Players.end(), [PlayerID](EventPlayer p) -> bool { return p.PlayerID == PlayerID; });
		if (Itr != Players.end())
		{
			Players.erase(Itr);
			return true;
		}
	}
	return false;
}

bool EventMan::StartEvent(const int EventID)
{
	if (CurrentEvent != nullptr) return false;
	if (EventID == -1)
	{
		int Rand = 0;
		CurrentEvent = &Events[Rand];
		CurrentEvent->Init(Map);
		EventRunning = true;
	}
	else if (EventID < Events.size())
	{
		CurrentEvent = &Events[EventID];
		CurrentEvent->Init(Map);
		EventRunning = true;
	}
	return true;
}

void EventMan::Update()
{
	if (!ArkApi::GetApiUtils().GetShooterGameMode() || Events.size() == 0) return;
	if (IsEventRunning() && CurrentEvent != nullptr)
	{
		if (CurrentEvent->GetState() != Finnished) CurrentEvent->Update();
		else
		{
			EventRunning = false;
			CurrentEvent = nullptr;
			NextEventTime = timeGetTime() + 300000;
		}
	}
	else if (timeGetTime() > NextEventTime)
	{
		if (Map.IsEmpty()) ArkApi::GetApiUtils().GetShooterGameMode()->GetMapName(&Map);
		StartEvent();
		NextEventTime = timeGetTime() + 0;//RandomNumber(7200000, 21600000);
	}
}

void EventMan::TeleportEventPlayers(const bool TeamBased, const bool WipeInventory, const bool PreventDinos, const TArray<FVector> TeamA, const TArray<FVector> TeamB)
{
	try
	{
		int TeamAPosCount = 0, TeamBPosCount = 0;
		bool TeamSwitch = false;
		FVector Pos;
		for (EventPlayerArrayItr itr = Players.begin(); itr != Players.end(); itr++)
		{
			if (TeamAPosCount == TeamA.Num()) TeamAPosCount = 0;
			if (TeamBPosCount == TeamB.Num()) TeamBPosCount = 0;

			if (itr->ASPC && itr->ASPC->PlayerStateField()() && itr->ASPC->GetPlayerCharacter() && !itr->ASPC->GetPlayerCharacter()->IsDead() && (!PreventDinos && itr->ASPC->GetPlayerCharacter()->GetRidingDino() != nullptr || itr->ASPC->GetPlayerCharacter()->GetRidingDino() == nullptr))
			{
				if (WipeInventory)
				{
					UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(itr->ASPC->CheatManagerField()());
					if (cheatManager) cheatManager->ClearPlayerInventory((int)itr->ASPC->LinkedPlayerIDField()(), true, true, true);
				}

				itr->StartPos = ArkApi::GetApiUtils().GetPosition(itr->ASPC);
				
				if (!TeamBased)
				{
					Pos = TeamA[TeamAPosCount++];
					itr->ASPC->SetPlayerPos(Pos.X, Pos.Y, Pos.Z);
				}
				else
				{
					Pos = TeamSwitch ? TeamA[TeamAPosCount++] : TeamB[TeamBPosCount++];
					itr->Team = TeamSwitch ? EventTeam::Blue : EventTeam::Red;
					itr->ASPC->SetPlayerPos(Pos.X, Pos.Y, Pos.Z);
					TeamSwitch = !TeamSwitch;
				}
			}
			else itr = Players.erase(itr);
		}
		if (CurrentEvent) CurrentEvent->SetStartPlayerCount((int)Players.size());
	}
	catch (...)
	{
		//		std::cout << "Event Manager Exception: TeleportPlayers" << std::endl;
	}
}

bool EventMan::CanTakeDamage(long long AttackerID, long long VictimID)
{
	if (AttackerID == VictimID) return true;
	EventPlayer* Attacker, *Victim;
	if ((Attacker = FindPlayer(AttackerID)) != nullptr && (Victim = FindPlayer(VictimID)) != nullptr)
	{
		if (Attacker->Team != EventTeam::All && Attacker->Team == Victim->Team) return false;
		else if (CurrentEvent != nullptr && CurrentEvent->GetState() != EventState::Fighting) return false;
	}
	return true;
}

void EventMan::OnPlayerDied(long long AttackerID, long long VictimID)
{
	if (AttackerID != -1 && AttackerID != VictimID)
	{
		EventPlayer* Attacker;
		if ((Attacker = FindPlayer(AttackerID)) != nullptr) Attacker->Kills++;
	}
	RemovePlayer(VictimID, false);
}

void EventMan::OnPlayerLogg(AShooterPlayerController* Player)
{
	EventPlayer* EPlayer;
	if ((EPlayer = FindPlayer(Player->LinkedPlayerIDField()())) != nullptr)
	{
		Player->SetPlayerPos(EPlayer->StartPos.X, EPlayer->StartPos.Y, EPlayer->StartPos.Z);
		RemovePlayer(Player->LinkedPlayerIDField()(), false);
	}
}

bool EventMan::IsEventProtectedStructure(const FVector& StructurePos)
{
	for (Event Evt : Events) if (Evt.IsEventProtectedStructure(StructurePos)) return true;
	return false;
}

/*
void EventManager::OnWonEvent(bool Solo)
{
	try
	{
		if (Players.size() > 0)
		{
			if (Solo)
			{
				EventPlayer* Player = &Players[0];
				if (Player != NULL && Player->ASPC && Player->ASPC->PlayerStateField()() /*&& Player->ASPC->GetPlayerCharacter() && Player->ASPC->GetPlayerCharacter()->GetReplicatedCurrentHealthField() > 1*//*)
				{
					//TeleportToPos(Player->ASPC, Player->StartPos);

					//	Tools::SendColoredMessageToAll(TEXT("[Event] %s %s is The Winner!"), Tools::GetChatColour(Tools::Colours::Oranage), CurrentEvent->GetName().GetW(), GetCharacterNameW(Player->ASPC));
					if (GetBetAmount() > 0) //Bet Based Event Rewards
					{
						int TotalAtStart = 0;
						if (CurrentEvent) TotalAtStart = CurrentEvent->GetStartedPlayerCount();
						UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(Player->ASPC->CheatManagerField()());
						if (cheatManager)
						{
							cheatManager->ClearPlayerInventory(Player->ASPC->LinkedPlayerIDField()(), true, true, true);
							int WonAmount = GetBetAmount() * TotalAtStart;
							for (; WonAmount > 0; WonAmount -= 200)
								if (WonAmount > 200) cheatManager->GiveItemNumToPlayer(Player->ASPC->LinkedPlayerIDField()(), 74, 200, 0, false);
								else cheatManager->GiveItemNumToPlayer(Player->ASPC->LinkedPlayerIDField()(), 74, WonAmount, 0, false);
						}
					}
				}
			}
		}
		Players.clear();
	}
	catch (...)
	{
		//		std::cout << "Event Manager Exception: OnWonEvent" << std::endl;
	}
}*/