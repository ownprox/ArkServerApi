#pragma once
class DeathMatch : Event
{
public:
	DeathMatch()
	{
		std::cout << "DeathMatch() Called" << std::endl;
		InitDefaults(TEXT("DeathMatch"), FVector(0, 0, 0), 100, 4);
	}

	virtual void Init(ServerType ServType)
	{
		if (!GetSpawnsSet())
		{
			switch (ServType)
			{
			case ServerType::Island1:
				SetSpawns(new FVector[4]{ FVector(1, 1, 1), FVector(1, 1, 1), FVector(1, 1, 1), FVector(1, 1, 1) });
				break;
			case ServerType::Ragnarok1:
				break;
			case ServerType::Center1:
				break;
			case ServerType::Scorched1:
				break;
			}
		}
		std::cout << "DeathMatch::Init() Called" << std::endl;
		Reset();
		AddTime(60);
	}

	virtual void Update()
	{
		switch (GetState())
		{
		case EventState::WaitingForPlayers:
			if (TimePassed())
			{
				if (UpCount() == 4)
				{
					ResetCount();
					if (EventM->CountPlayers() < 2)
					{
						Tools::SendColoredMessageToAll(TEXT("[Event] %s Failed to start needed 2 Players"), Tools::GetChatColour(Tools::Colours::Oranage), GetName().GetW());
						SetState(EventState::Finnished);
					} else SetState(EventState::TeleportingPlayers);
				}
				AddTime(60);
			}
			break;
		case EventState::TeleportingPlayers:
			EventM->TeleportPlayers(GetSpawns());
			EventM->SendMessageToAllEventPlayers(TEXT("[Event] %s Starting in 30 Seconds"), Tools::Colours::Oranage, 20, true, GetName().GetW());
			AddTime(30);
			SetState(EventState::WaitForFight);
			break;
		case EventState::WaitForFight:
			if (TimePassed())
			{
				EventM->SendMessageToAllEventPlayers(TEXT("[Event] %s Started Kill or Be Killed!"), Tools::Colours::Oranage, 10, true, GetName().GetW());
				SetState(EventState::Fighting);
			}
			break;
		case EventState::Fighting:
			break;
		case EventState::Rewarding:
			break;
		}
	}
};