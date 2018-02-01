#include "Private\EventMan.h"
#include "Private\Event.h"

//swap for chrono
int timeGetTimea()
{
	return 1;
}

EventMan::EventMan()
{
	Instance = this;
	EventRunning = false;
	ServType = ServerType::Ragnarok;
}

EventMan* EventMan::Instance = 0;

EventMan* EventMan::GetInstance()
{
	return Instance;
}

void EventMan::AddEvent(Event* event)
{
	Events.push_back(event);
}

void EventMan::RemoveEvent(Event* event)
{
	EventItr itr = std::find(Events.begin(), Events.end(), event);
	if (itr != Events.end()) Events.erase(itr);
}


bool EventMan::AddPlayer(long long PlayerID, AShooterPlayerController* player)
{
	if (IsEventRunning() && CurrentEvent != NULL && CurrentEvent->GetState() == EventState::WaitingForPlayers && FindPlayer(PlayerID) == nullptr)
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
		CurrentEvent = Events[Rand];
		CurrentEvent->Init(ServType);
		EventRunning = true;
	}
	else if (EventID < Events.size())
	{
		CurrentEvent = Events[EventID];
		CurrentEvent->Init(ServType);
		EventRunning = true;
	}
	return true;
}

void EventMan::Update()
{
	if (!ArkApi::GetApiUtils().GetShooterGameMode() || Events.size() == 0) return;
	if (IsEventRunning() && CurrentEvent != nullptr)
	{
		if (!CurrentEvent->Finnished())
		{
			CurrentEvent->Update();
		}
		else
		{
			EventRunning = false;
			CurrentEvent = nullptr;
			NextEventTime = timeGetTimea() + 300000;
		}
	}
	else if (timeGetTimea() > NextEventTime)
	{
		StartEvent();
		NextEventTime = timeGetTimea() + 0;//RandomNumber(7200000, 21600000);
	}
}

void EventMan::TeleportEventPlayers(const FVector* Positions, const bool TeamBased, const bool IsBet)
{
	try
	{
		UShooterCheatManager* cheatManager;
		int Size = sizeof(Positions) / sizeof(FVector), PosCount = 0;
		for (EventPlayerArrayItr itr = Players.begin(); itr != Players.end(); itr++)
		{
			if (PosCount == Size) PosCount = 0;
			if (itr->ASPC && itr->ASPC->PlayerStateField()() /*&& itr->ASPC->GetPlayerCharacter() && itr->ASPC->GetPlayerCharacter()->GetReplicatedCurrentHealthField() > 1 &&*/ /*!IsRidingDino(itr->ASPC)*/ && (cheatManager = static_cast<UShooterCheatManager*>(itr->ASPC->CheatManagerField()())) != NULL)
			{
				/*int IgnotCount = GetItemCount(itr->ASPC, "Metal Ingot");
				if (IgnotCount != CurrentEvent->GetBetAmount())
				{
				SendMessage(itr->ASPC, TEXT("Nice Try!"), Tools::Colours::Red);
				itr = Players.erase(itr);
				continue;
				}*/
				cheatManager->ClearPlayerInventory(itr->ASPC->LinkedPlayerIDField()(), true, true, true);
				itr->StartPos = ArkApi::GetApiUtils().GetPosition(itr->ASPC);
				//TeleportToPos(itr->ASPC, Positions[PosCount++]);
				if (!TeamBased)
				{
					//	cheatManager->GiveItemNumToPlayer(itr->ASPC->LinkedPlayerIDField()(), EventItem, 3, 0, false);
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