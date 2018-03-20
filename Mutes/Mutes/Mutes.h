#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
#include "hdr/sqlite_modern_cpp.h"

sqlite::database& GetDB()
{
	static sqlite::database db(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/Mutes/Mutes.db");
	return db;
}

FString Messages[5];

AShooterPlayerController* FindPlayerFromCharacterName(const FString& character_name)
{
	const int NameLength = character_name.Len();
	int LastNameLenDiff = 999;
	FString char_name;
	AShooterPlayerController* Player = nullptr, *PlayerCon;
	const auto& player_controllers = ArkApi::GetApiUtils().GetWorld()->PlayerControllerListField()();
	for (TWeakObjectPtr<APlayerController> player_controller : player_controllers)
	{
		PlayerCon = static_cast<AShooterPlayerController*>(player_controller.Get());
		if (!PlayerCon) continue;
		char_name = ArkApi::GetApiUtils().GetCharacterName(PlayerCon);
		if (!char_name.IsEmpty() && char_name.StartsWith(character_name, ESearchCase::Type::IgnoreCase) && LastNameLenDiff > (char_name.Len() - NameLength))
		{
			Player = PlayerCon;
			LastNameLenDiff = char_name.Len() - NameLength;
		}
	}
	return Player;
}