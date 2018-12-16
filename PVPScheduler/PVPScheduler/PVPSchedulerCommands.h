#pragma once
/*
inline void PVP(AShooterPlayerController* player, FString* message, int mode)
{
	if (!player || !player->PlayerStateField() || !player->GetPlayerCharacter() || !player->GetPlayerCharacter()->
	                                                                                          bIsServerAdminField())
		return;

	int EndDaya = 0, EndHoura = 0;
	if (PVPEnabled)
	{
		TArray<FString> Parsed;
		message->ParseIntoArray(Parsed, L" ", true);
		if (Parsed.IsValidIndex(2))
		{
			try
			{
				EndDaya = std::stoi(Parsed[1].ToString().c_str());
				EndHoura = std::stoi(Parsed[2].ToString().c_str());
			}
			catch (...)
			{
			}

			//EndDay = EndDaya;
			//EndHour = EndHoura;
		}
		else
		{
			ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(1, 0, 0), "Incorrect Syntax: /pvp <EndDay> <EndHour>");
			return;
		}
	}

	PVPEnabled = !PVPEnabled;

	ArkApi::GetApiUtils().SendServerMessage(player, FLinearColor(0, 1, 0), "PVP: {}",
	                                        (PVPEnabled ? "Enabled" : "Disabled"));
}*/

inline void PVPReloadConfig(APlayerController* player, FString*, bool)
{
	InitConfig();

	ArkApi::GetApiUtils().SendServerMessage(static_cast<AShooterPlayerController*>(player), FLinearColor(0, 1, 0), "Config Reloaded!");
}

inline void InitCommands()
{
	//ArkApi::GetCommands().AddChatCommand("/pvp", &PVP);
	ArkApi::GetCommands().AddConsoleCommand("pvpreload", &PVPReloadConfig);
}

inline void RemoveCommands()
{
	//ArkApi::GetCommands().RemoveChatCommand("/pvp");
	ArkApi::GetCommands().RemoveConsoleCommand("pvpreload");
}
