#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
bool PlayerKillsEnabled, DinoKillsEnabled, DisplayInChatbox;
int DisplayType;
float DisplayScale, DisplayDelay;
FLinearColor DisplayColour;
FString ServerName;
std::wstring Messages[4];

const float trunc_decs(const float& f, int decs)
{
	int i1 = static_cast<int>(floor(f));
	float rmnd = f - i1;
	int i2 = static_cast<int>(rmnd * pow(10, decs));
	float f1 = static_cast<float>(i2 / pow(10, decs));
	return i1 + f1;
}

const FString GetTribeName(AShooterPlayerController* player_controller)
{
	AShooterPlayerState* player_state = static_cast<AShooterPlayerState*>(player_controller->PlayerStateField()());
	if (player_state && player_state->MyPlayerDataStructField()()) return player_state->MyTribeDataField()().TribeName;
	return FString("");
}