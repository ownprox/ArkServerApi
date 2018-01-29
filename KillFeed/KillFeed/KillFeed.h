#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
bool PlayerKillsEnabled, DinoKillsEnabled, DisplayInChatbox;
int DisplayType;
float DisplayScale, DisplayDelay;
FLinearColor DisplayColour;
std::wstring Messages[4];

UTexture2D* GetWeaponIcon(FString WeaponName)
{
	UTexture2D* WeaponIcon = nullptr;
	if (int WepNameStart, WepNameEnd; WeaponName.FindChar('p', WepNameStart) && WeaponName.FindChar('_', WepNameEnd))
	{
		WeaponName = WeaponName.Mid(WepNameStart+1, WepNameEnd-3);
		if (!WeaponName.Equals("Fist_"))
		{
			WeaponName = "/Game/PrimalEarth/CoreBlueprints/Weapons/Icons/" + WeaponName + "Icon." + WeaponName + "Icon";
			WeaponIcon = reinterpret_cast<UTexture2D*>(Globals::StaticLoadObject(UTexture2D::StaticClass(), nullptr, *WeaponName, nullptr, 0, 0, true));
		}
	}
	return WeaponIcon;
}

const float trunc_decs(const float& f, int decs)
{
	int i1 = floor(f);
	float rmnd = f - i1;
	int i2 = static_cast<int> (rmnd * pow(10, decs));
	float f1 = i2 / pow(10, decs);
	return i1 + f1;
}

static FString GetTribeName(AShooterPlayerController* player_controller)
{
	AShooterPlayerState* player_state = static_cast<AShooterPlayerState*>(player_controller->PlayerStateField()());
	if (player_state && player_state->MyPlayerDataStructField()())
		return player_state->MyTribeDataField()().TribeName;

	return FString("");
}