#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
int TeleportDelay, TPRCoolDownSeconds;
float MaxDistance, TPRCoolDownEnabled;
bool AllowDinoTeleport, TPREnabled, TPRCoolDownForPlayerTeleportedTo;
FString TPR, TPA, TP, TPM, TPD, TPDP, TPP, TT, POS;
std::wstring Messages[20];

struct PlayerS
{
	__int64 SteamID, LastTPRID;
	bool IsTping;
	PlayerS(__int64 SteamID)
	{
		this->SteamID = SteamID;
		this->LastTPRID = -1;
		this->IsTping = false;
	}
};

typedef std::vector<PlayerS> PlayerArray;
typedef PlayerArray::iterator PlayerArrayItr;
PlayerArray Players;

PlayerS* GetPlayer(__int64 SteamID)
{
	PlayerArrayItr player = std::find_if(Players.begin(), Players.end(), [SteamID](const PlayerS & ps) -> bool { return ps.SteamID == SteamID; });
	if (player != Players.end()) return &(*player);
	else
	{
		Players.push_back(PlayerS(SteamID));
		return &(*(Players.end()-1));
	}
}

void RemovePlayer(__int64 SteamID)
{
	PlayerArrayItr player = std::find_if(Players.begin(), Players.end(), [SteamID](const PlayerS & ps) -> bool { return ps.SteamID == SteamID; });
	if (player != Players.end()) Players.erase(player);
}	

class Timer
{
public:
	template <class callable, class... arguments>
	Timer(int after, bool async, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments ...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		if (async)
		{
			std::thread([after, task]()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(after));
				task();
			}).detach();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}
	}
};

struct CoolDownData
{
	uint64 SteamID;
	DWORD64 CoolDown;

	CoolDownData(const uint64 SteamID, const int MuteSecs)
	{
		this->SteamID = SteamID; 
		this->CoolDown = timeGetTime() + (MuteSecs * 1000);
	}

	bool HasCoolDownPassed() { return timeGetTime() > CoolDown; }
};
std::vector<CoolDownData> CoolData;
std::vector<CoolDownData>::iterator GetCoolDownData(const uint64 SteamID) { return std::find_if(CoolData.begin(), CoolData.end(), [SteamID](const CoolDownData& cdd) -> bool { return cdd.SteamID == SteamID; }); }

void AddCoolDown(const uint64 SteamID, const int MuteSecs)
{
	CoolData.push_back(CoolDownData(SteamID, MuteSecs));
}

void ClearCoolDowns()
{
	CoolData.clear();
}

bool IsOnCoolDown(AShooterPlayerController* Player)
{
	auto iter = GetCoolDownData(ArkApi::GetApiUtils().GetSteamIdFromController(Player));
	if (iter != CoolData.end())
	{
		if (iter->HasCoolDownPassed())
		{
			CoolData.erase(iter);
			return false;
		}
		const int CoolDownTime = (int)((iter->CoolDown - timeGetTime()) / 1000), mins = (CoolDownTime > 60 ? (CoolDownTime / 60) : 0);
		if (mins != 0)
		{
			int secs = (mins > 0 ? (CoolDownTime - (mins * 60)) : CoolDownTime);
			if (secs < 0) secs = 0;
			ArkApi::GetApiUtils().SendServerMessage(Player, FLinearColor(1, 0, 0), Messages[13].c_str(), mins, secs);
		}
		else ArkApi::GetApiUtils().SendServerMessage(Player, FLinearColor(1, 0, 0), Messages[14].c_str(), CoolDownTime);
		return true;
	}
	return false;
}

void TeleportToPlayer(AShooterPlayerController* me, PlayerS* p, AShooterPlayerController* him, PlayerS* op, bool check_for_dino, float max_dist = -1)
{
	if (op == nullptr || p == nullptr) return;
	if (!(me && me->GetPlayerCharacter() && !me->GetPlayerCharacter()->IsDead()))
	{
		ArkApi::GetApiUtils().SendServerMessage(him, FLinearColor(255, 0, 0), Messages[1].c_str(), *ArkApi::GetApiUtils().GetCharacterName(me));
		op->IsTping = p->IsTping = false;
		return;
	}

	if (!(him && him->GetPlayerCharacter() && !him->GetPlayerCharacter()->IsDead()))
	{
		ArkApi::GetApiUtils().SendServerMessage(me, FLinearColor(255, 0, 0), Messages[1].c_str(), *ArkApi::GetApiUtils().GetCharacterName(him));
		op->IsTping = p->IsTping = false;
		return;
	}

	if (check_for_dino && ArkApi::GetApiUtils().IsRidingDino(him))
	{
		ArkApi::GetApiUtils().SendServerMessage(me, FLinearColor(255, 0, 0), Messages[15].c_str(), *ArkApi::GetApiUtils().GetCharacterName(him));
		ArkApi::GetApiUtils().SendServerMessage(him, FLinearColor(255, 0, 0), Messages[16].c_str());
		op->IsTping = p->IsTping = false;
		return;
	}

	if (check_for_dino && ArkApi::GetApiUtils().IsRidingDino(me))
	{
		ArkApi::GetApiUtils().SendServerMessage(him, FLinearColor(255, 0, 0), Messages[15].c_str(), *ArkApi::GetApiUtils().GetCharacterName(me));
		ArkApi::GetApiUtils().SendServerMessage(me, FLinearColor(255, 0, 0), Messages[16].c_str());
		op->IsTping = p->IsTping = false;
		return;
	}

	if (max_dist != -1 && FVector::Distance(ArkApi::GetApiUtils().GetPosition(me), ArkApi::GetApiUtils().GetPosition(him)) > max_dist)
	{
		ArkApi::GetApiUtils().SendServerMessage(him, FLinearColor(255, 0, 0), Messages[17].c_str());
		ArkApi::GetApiUtils().SendServerMessage(me, FLinearColor(255, 0, 0), Messages[17].c_str());
		op->IsTping = p->IsTping = false;
		return;
	}

	const FVector pos = him->DefaultActorLocationField();
	me->SetPlayerPos(pos.X, pos.Y, pos.Z);
	if (TPRCoolDownEnabled)
	{
		AddCoolDown(p->SteamID, TPRCoolDownSeconds);
		if(TPRCoolDownForPlayerTeleportedTo) AddCoolDown(op->SteamID, TPRCoolDownSeconds);
	}
	op->IsTping = p->IsTping = false;
}

AShooterPlayerController* FindPlayerFromCharacterName(const FString& character_name)
{
	const int NameLength = character_name.Len();
	int LastNameLenDiff = 999;
	FString char_name;
	AShooterPlayerController* Player = nullptr, *PlayerCon;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField();
	for (TWeakObjectPtr<APlayerController> player_controller : player_controllers)
	{
		PlayerCon = static_cast<AShooterPlayerController*>(player_controller.Get());
		char_name = ArkApi::GetApiUtils().GetCharacterName(PlayerCon);
		if (!char_name.IsEmpty() && char_name.StartsWith(character_name, ESearchCase::Type::IgnoreCase) && LastNameLenDiff > (char_name.Len() - NameLength))
		{
			Player = PlayerCon;
			LastNameLenDiff = char_name.Len() - NameLength;
		}
	}
	return Player;
}