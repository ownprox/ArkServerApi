#pragma once
class BattleArena : Event
{
private:
	wchar_t* Creator;
public:
	BattleArena(int IgnotAmount, wchar_t* Creator)
	{
		std::cout << "BattleArena() Called" << std::endl;
		InitDefaults(TEXT("BattleArena"), FVector(0, 0, 0), 100, 4, IgnotAmount);
		this->Creator = Creator;
	}

	virtual void Init(ServerType ServType)
	{
		if (!GetSpawnsSet())
		{
			switch (ServType)
			{
			case ServerType::Island1:
				break;
			case ServerType::Ragnarok1:
				std::cout << "BattleArena::Init() Ragnarok Spawns" << std::endl;
				SetSpawns(new FVector[33]{ FVector(-80008.9, -458289, -4488.32),FVector(-80229.1, -458212, -4488.32),FVector(-80599.3, -458133, -4488.32),FVector(-80906.2, -458068, -4488.32),FVector(-81238, -457998, -4488.32),FVector(-81618.7, -457917, -4488.32),FVector(-82031.5, -457830, -4488.32),FVector(-82115.7, -457535, -4488.32),FVector(-81800.2, -457568, -4488.32),FVector(-81449.2, -457642, -4488.32),FVector(-81105.6, -457715, -4488.32),FVector(-80797.6, -457780, -4488.32),FVector(-80437.5, -457857, -4488.32),FVector(-80042.5, -457941, -4488.32),FVector(-79878, -457492, -4488.32),FVector(-80230, -457327, -4488.32),FVector(-80734.9, -457151, -4488.32),FVector(-81290.7, -457134, -4488.32),FVector(-81665.4, -457044, -4488.32),FVector(-81994.9, -456964, -4488.32),FVector(-81860.2, -456729, -4488.32),FVector(-81112.8, -456836, -4488.32),FVector(-80561.6, -456974, -4488.32),FVector(-80180.5, -457068, -4488.32),FVector(-79843.8, -457027, -4488.32),FVector(-79807.8, -456712, -4488.32),FVector(-80664.5, -456700, -4488.32),FVector(-81564.7, -456596, -4488.32),FVector(-81843.4, -455972, -4497.59),FVector(-81380.7, -456055, -4497.59),FVector(-80716, -456219, -4488.4),FVector(-80059.8, -456322, -4488.38),FVector(-79566.9, -456350, -4497.37) });
				break;
			case ServerType::Center1:
				break;
			case ServerType::Scorched1:
				break;
			}
		}
		std::cout << "BattleArena::Init() Called" << std::endl;
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
				if (UpCount() == 3)
				{
					ResetCount();
					if (EventM->CountPlayers() < 1)
					{
						Tools::SendColoredMessageToAll(TEXT("[Event] %s Failed to start needed 2 Players"), Tools::GetChatColour(Tools::Colours::Oranage), GetName().GetW());
						SetState(EventState::Finnished);
					}
					else SetState(EventState::TeleportingPlayers);
					return;
				}	
				Tools::SendBroadCastMessageToAll(TEXT("[Event] %s Started By %s - Entry Fee: %d Metal Ingots! Must be naked with Ingots in inventory type /join"), 5, GetName().GetW(), Creator, GetBetAmount());
				AddTime(60);
			}
			break;
		case EventState::TeleportingPlayers:
			EventM->TeleportPlayers(GetSpawns(), false, true);
			EventM->SendMessageToAllEventPlayers(TEXT("[Event] %s Starting in 60 Seconds, Equip Your Weapons!"), Tools::Colours::Oranage, 20, true, GetName().GetW());
			AddTime(60);
			SetState(EventState::WaitForFight);
			break;
		case EventState::WaitForFight:
			if (TimePassed())
			{
				EventM->SendMessageToAllEventPlayers(TEXT("[Event] %s Started Kill or Be Killed!"), Tools::Colours::Red, 10, true, GetName().GetW());
				SetState(EventState::Fighting);
				AddTime(5);
			}
			break;
		case EventState::Fighting:
			if (TimePassed())
			{
				int Players = EventM->CountPlayers();
				if (Players <= 1) SetState(EventState::Rewarding);
				else
				{
					EventM->SendMessageToAllEventPlayers(TEXT("[Event] %s %d Players Alive!"), Tools::Colours::Oranage, 0, false, GetName().GetW(), Players);
					AddTime(5);
				}
			}
			break;
		case EventState::Rewarding:
			EventM->OnWonEvent(true);
			SetState(EventState::Finnished);
			break;
		}
	}
};