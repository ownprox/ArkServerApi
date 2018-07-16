#include "EventManager.h"
#pragma comment(lib, "ArkApi.lib")
#include <fstream>
#include "json.hpp"
DECLARE_HOOK(APrimalStructure_IsAllowedToBuild, int, APrimalStructure*, APlayerController*, FVector, FRotator, FPlacementData*, bool, FRotator, bool);
DECLARE_HOOK(APrimalCharacter_TakeDamage, float, APrimalCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(APrimalStructure_TakeDamage, float, APrimalStructure*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(AShooterCharacter_Die, bool, AShooterCharacter*, float, FDamageEvent*, AController*, AActor*);
DECLARE_HOOK(AShooterGameMode_Logout, void, AShooterGameMode*, AController*);

FString EventJoinCommand, EventLeaveCommand, EventAdminStartEventConsoleCommand;
FString Messages[5];

void EventManagerUpdate()
{
	if (!ArkApi::GetApiUtils().GetShooterGameMode()) return;
	EventManager::Get().Update();
}

int _cdecl Hook_APrimalStructure_IsAllowedToBuild(APrimalStructure* _this, APlayerController* PC, FVector AtLocation, FRotator AtRotation
	, FPlacementData * OutPlacementData, bool bDontAdjustForMaxRange, FRotator PlayerViewRotation, bool bFinalPlacement)
{
	return APrimalStructure_IsAllowedToBuild_original(_this, PC, AtLocation, AtRotation, OutPlacementData, bDontAdjustForMaxRange
		, PlayerViewRotation, bFinalPlacement);
}

const long long GetPlayerID(APrimalCharacter* _this)
{
	AShooterCharacter* shooterCharacter = static_cast<AShooterCharacter*>(_this);
	return (shooterCharacter && shooterCharacter->GetPlayerData()) ? shooterCharacter->GetPlayerData()->MyDataField()->PlayerDataIDField() : -1;
}

const long long GetPlayerID(AController* _this)
{
	AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(_this);
	return Player ? Player->LinkedPlayerIDField() : 0;
}

float _cdecl Hook_APrimalCharacter_TakeDamage(APrimalCharacter* _this, float Damage, FDamageEvent* DamageEvent, AController* EventInstigator
	, AActor* DamageCauser)
{
	return (EventManager::Get().IsEventRunning() && EventInstigator && _this && !EventInstigator->IsLocalController()
		&& _this->IsA(AShooterCharacter::GetPrivateStaticClass()) ? (EventManager::Get().CanTakeDamage(GetPlayerID(EventInstigator), GetPlayerID(_this))
			? APrimalCharacter_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser) : 0)
		: APrimalCharacter_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser));
}

float _cdecl Hook_APrimalStructure_TakeDamage(APrimalStructure* _this, float Damage, FDamageEvent* DamageEvent, AController* EventInstigator
	, AActor* DamageCauser)
{
	return EventManager::Get().IsEventProtectedStructure(_this->RootComponentField() ? _this->RootComponentField()->RelativeLocationField()
		: FVector(0, 0, 0)) ? 0 : APrimalStructure_TakeDamage_original(_this, Damage, DamageEvent, EventInstigator, DamageCauser);
}

bool _cdecl Hook_AShooterCharacter_Die(AShooterCharacter* _this, float KillingDamage, FDamageEvent* DamageEvent, AController* Killer, AActor* DamageCauser)
{
	return (EventManager::Get().IsEventRunning() && Killer && _this && !Killer->IsLocalController() && Killer->IsA(AShooterCharacter::GetPrivateStaticClass())
		&& EventManager::Get().OnPlayerDied(GetPlayerID(Killer), GetPlayerID(_this))) ? 0 : AShooterCharacter_Die_original(_this, KillingDamage, DamageEvent, Killer, DamageCauser);
}

void _cdecl Hook_AShooterGameMode_Logout(AShooterGameMode* _this, AController* Exiting)
{
	if (EventManager::Get().IsEventRunning() && Exiting && Exiting->IsA(AShooterPlayerController::StaticClass()))
	{
		AShooterPlayerController* Player = static_cast<AShooterPlayerController*>(Exiting);
		if (Player)	EventManager::Get().OnPlayerLogg(Player);
	}
	AShooterGameMode_Logout_original(_this, Exiting);
}

void JoinEvent(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	if (EventManager::Get().IsEventRunning() && !EventManager::Get().IsEventOverrideJoinAndLeave() && EventManager::Get().GetEventState() == EventState::WaitingForPlayers)
	{
		if (EventManager::Get().OnlyNakeds())
		{
			const FString& NakedChkMsg = EventManager::Get().CheckIfPlayersNaked(player).value_or("");
			if (!NakedChkMsg.IsEmpty())
			{
				ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *NakedChkMsg);
				return;
			}
		}
		if (EventManager::Get().AddPlayer(player))
		{
			if (EventManager::Get().GetEventQueueNotifications()) ArkApi::GetApiUtils().SendChatMessageToAll(EventManager::Get().GetServerName(), *Messages[0], *ArkApi::GetApiUtils().GetCharacterName(player), *EventManager::Get().GetCurrentEventName());
			ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *Messages[1]);
		} else ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *Messages[2], *EventManager::Get().GetCurrentEventName());
	}
}

void LeaveEvent(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	if (EventManager::Get().IsEventRunning() && !EventManager::Get().IsEventOverrideJoinAndLeave() && EventManager::Get().GetEventState() == EventState::WaitingForPlayers)
	{
		if (EventManager::Get().RemovePlayer(player))
		{
			if (EventManager::Get().GetEventQueueNotifications()) ArkApi::GetApiUtils().SendChatMessageToAll(EventManager::Get().GetServerName(), *Messages[3], *ArkApi::GetApiUtils().GetCharacterName(player), *EventManager::Get().GetCurrentEventName());
			ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *Messages[4], *EventManager::Get().GetCurrentEventName());
		} else ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *Messages[5], *EventManager::Get().GetCurrentEventName());
	}
}

void StartEvent(APlayerController* player_controller, FString* message, bool LogToFile)
{
	AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller);
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter()) return;
	if (!EventManager::Get().IsEventRunning())
	{
		TArray<FString> Parsed;
		message->ParseIntoArray(Parsed, L" ", true);
		if (!Parsed.IsValidIndex(1)) EventManager::Get().StartEvent();
		else
		{
			int EventID = -1;
			try
			{
				EventID = std::stoi(Parsed[1].ToString().c_str());
			} catch (...) {}

			if (EventID != -1 && EventID < EventManager::Get().GetEventsCount()) EventManager::Get().StartEvent(EventID);
			else EventManager::Get().StartEvent();
		}
		ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *Messages[6], *EventManager::Get().GetCurrentEventName());
	} else ArkApi::GetApiUtils().SendChatMessage(player, EventManager::Get().GetServerName(), *Messages[7], *EventManager::Get().GetCurrentEventName());
}

void Pos(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->bIsAdmin()()) return;

	FVector Pos = player->DefaultActorLocationField();
	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), L"{}, {}, {},", Pos.X, Pos.Y, Pos.Z);
	Log::GetLog()->warn("{}, {}, {}", Pos.X, Pos.Y, Pos.Z);
}

void InitConfig()
{
	const std::string config_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/AAEventManager/Config.json";

	std::ifstream file{ config_path };

	if (!file.is_open()) throw std::runtime_error(fmt::format("Can't open {}", config_path.c_str()).c_str());

	nlohmann::json config;

	file >> config;

	std::string data = config["EventManager"]["JoinCommand"];
	EventJoinCommand = ArkApi::Tools::Utf8Decode(data).c_str();
	data = config["EventManager"]["LeaveCommand"];
	EventLeaveCommand = ArkApi::Tools::Utf8Decode(data).c_str();
	data = config["EventManager"]["AdminStartEventConsoleCommand"];
	EventAdminStartEventConsoleCommand = ArkApi::Tools::Utf8Decode(data).c_str();

	data = config["EventManager"]["ServerName"];

	const int EventStartMinuteMin = config["EventManager"]["EventStartMinuteMin"], EventStartMinuteMax = config["EventManager"]["EventStartMinuteMax"];

	const bool LogToConsole = config["EventManager"]["DebugLogToConsole"];
	
	EventManager::Get().InitConfigs(ArkApi::Tools::Utf8Decode(data).c_str(), EventJoinCommand, EventStartMinuteMin, EventStartMinuteMax, LogToConsole);
	
	int j = 0;
	const auto& Msgs = config["EventManager"]["Messages"];
	for (const auto& Msg : Msgs)
	{
		data = Msg;
		Messages[j++] = ArkApi::Tools::Utf8Decode(data).c_str();
	}
	file.close();
}

void InitEventManager()
{
	Log::Get().Init("Event Manager");
	InitConfig();
	ArkApi::GetHooks().SetHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild, &APrimalStructure_IsAllowedToBuild_original);
	ArkApi::GetHooks().SetHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage, &APrimalCharacter_TakeDamage_original);
	ArkApi::GetHooks().SetHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage, &APrimalStructure_TakeDamage_original);
	ArkApi::GetHooks().SetHook("AShooterCharacter.Die", &Hook_AShooterCharacter_Die, &AShooterCharacter_Die_original);
	ArkApi::GetHooks().SetHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout, &AShooterGameMode_Logout_original);
	ArkApi::GetCommands().AddOnTimerCallback("EventManagerUpdate", &EventManagerUpdate);
	ArkApi::GetCommands().AddChatCommand(EventJoinCommand, &JoinEvent);
	ArkApi::GetCommands().AddChatCommand(EventLeaveCommand, &LeaveEvent);
	ArkApi::GetCommands().AddChatCommand("/tpos", &Pos);
	ArkApi::GetCommands().AddConsoleCommand(EventAdminStartEventConsoleCommand, &StartEvent);
}

void DestroyEventManager()
{
	ArkApi::GetHooks().DisableHook("APrimalStructure.IsAllowedToBuild", &Hook_APrimalStructure_IsAllowedToBuild);
	ArkApi::GetHooks().DisableHook("APrimalCharacter.TakeDamage", &Hook_APrimalCharacter_TakeDamage);
	ArkApi::GetHooks().DisableHook("APrimalStructure.TakeDamage", &Hook_APrimalStructure_TakeDamage);
	ArkApi::GetHooks().DisableHook("AShooterCharacter.Die", &Hook_AShooterCharacter_Die);
	ArkApi::GetHooks().DisableHook("AShooterGameMode.Logout", &Hook_AShooterGameMode_Logout);
	ArkApi::GetCommands().RemoveOnTimerCallback("EventManagerUpdate");
	ArkApi::GetCommands().RemoveChatCommand(EventJoinCommand);
	ArkApi::GetCommands().RemoveChatCommand(EventLeaveCommand);
	ArkApi::GetCommands().RemoveChatCommand("/tpos");
	ArkApi::GetCommands().RemoveConsoleCommand(EventAdminStartEventConsoleCommand);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitEventManager();
		break;
	case DLL_PROCESS_DETACH:
		DestroyEventManager();
		break;
	}
	return TRUE;
}