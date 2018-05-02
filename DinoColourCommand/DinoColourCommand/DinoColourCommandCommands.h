#pragma once

#include <Permissions.h>

#include "DinoColourCommand.h"

inline void ChangeDinoColour(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || RequiresAdmin && !player->bIsAdmin()())
		return;

	const uint64 steam_id = ArkApi::IApiUtils::GetSteamIdFromController(player);
	if (RequiresPermission && !Permissions::IsPlayerHasPermission(steam_id, "DinoColorCommand"))
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0),
		                                        "You don't have permissions to use this command");
		return;
	}

	TArray<FString> Parsed;
	message->ParseIntoArray(Parsed, L" ", true);

	if (Parsed.Num() < 3)
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /sdc <Region> <Colour>");
		return;
	}

	int Region = 0, Colour = 0;
	try
	{
		Region = std::stoi(Parsed[1].ToString().c_str());
		Colour = std::stoi(Parsed[2].ToString().c_str());
	}
	catch (...)
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Incorrect Syntax: /sdc <Region> <Colour>");
	}

	if (Colour < 1 || Colour > 56)
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Dino Colour must be 1 - 56");
		return;
	}
	if (Region < 0 || Region > 5)
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "Dino Region must be 0-5");
		return;
	}

	AActor* Actor = player->GetPlayerCharacter()->GetAimedActor(ECC_GameTraceChannel2, nullptr, 0.0, 0.0, nullptr, nullptr,
	                                                            false, false);
	if (Actor && Actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
	{
		APrimalDinoCharacter* Dino = static_cast<APrimalDinoCharacter*>(Actor);
		if (Dino && !Dino->IsDead())
		{
			AShooterPlayerState* ASPS = static_cast<AShooterPlayerState*>(player->PlayerStateField());
			if (ASPS && ASPS->MyPlayerDataStructField())
			{
				const int teamId = Dino->TargetingTeamField();
				if (teamId != player->TargetingTeamField() || ASPS->MyTribeDataField() &&
					teamId != ASPS->MyTribeDataField()->TribeIDField())
				{
					ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0), "That is not your dinosaur");
					return;
				}

				UFunction* Func = Dino->FindFunctionChecked(FName(L"ForceUpdateColorSets", EFindName::FNAME_Find, false));
				int Args[] = {Region, Colour};
				if (Func) Dino->ProcessEvent(Func, Args);

				FString DinoName = Dino->TamedNameField();
				if (DinoName.Len() == 0) Dino->DinoNameTagField().ToString(&DinoName);

				ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 255, 0), "{} Color updated",
				                                        DinoName.ToString().c_str());
			}
		}
	}
	else
	{
		ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(255, 0, 0),
		                                        "Please face the middle of your screen towards the dino you want to color");
	}
}

inline void InitCommands()
{
	ArkApi::GetCommands().AddChatCommand("/sdc", &ChangeDinoColour);
}

inline void RemoveCommands()
{
	ArkApi::GetCommands().RemoveChatCommand("/sdc");
}
