#include "Commands.h"

#include "SafeZones.h"
#include "SafeZoneManager.h"

namespace SafeZones::Commands
{
	nlohmann::basic_json<> FindZoneConfigByName(const std::string& name)
	{
		nlohmann::basic_json<> safezone_config;

		auto safe_zones = config["SafeZones"];
		for (const auto& safe_zone : safe_zones)
		{
			if (safe_zone["Name"] == name)
			{
				safezone_config = safe_zone;
				break;
			}
		}

		return safezone_config;
	}

	void SZGiveItems(APlayerController*, FString* cmd, bool)
	{
		TArray<FString> parsed;
		cmd->ParseIntoArray(parsed, L" ", true);

		if (parsed.IsValidIndex(1))
		{
			FString name = parsed[1];

			nlohmann::basic_json<> safezone_config = FindZoneConfigByName(name.ToString());
			if (safezone_config.empty())
				return;

			const auto& items_entry = safezone_config.value("ItemsConfig", nlohmann::json::object());

			const auto safe_zone = SafeZoneManager::Get().FindZoneByName(name);
			if (!safe_zone)
				return;

			for (AActor* actor : safe_zone->actors)
			{
				if (!actor->IsA(AShooterCharacter::GetPrivateStaticClass()))
					continue;

				AShooterPlayerController* player = ArkApi::GetApiUtils().FindControllerFromCharacter(
					static_cast<AShooterCharacter*>(actor));
				if (player)
				{
					// Give items
					auto items_map = items_entry.value("Items", nlohmann::json::array());
					for (const auto& item : items_map)
					{
						const int amount = item["Amount"];
						const float quality = item["Quality"];
						const bool force_blueprint = item["ForceBlueprint"];
						std::string blueprint = item["Blueprint"];

						FString fblueprint(blueprint.c_str());

						//player->GiveItem(&fblueprint, amount, quality, force_blueprint);
						UShooterCheatManager* cheatManager = static_cast<UShooterCheatManager*>(player->CheatManagerField());
						if (cheatManager) cheatManager->GiveItemToPlayer((int)player->LinkedPlayerIDField(), &fblueprint, amount, quality, force_blueprint);
					}

					// Give dinos
					auto dinos_map = items_entry.value("Dinos", nlohmann::json::array());
					for (const auto& dino : dinos_map)
					{
						const int level = dino["Level"];
						std::string blueprint = dino["Blueprint"];

						const FString fblueprint(blueprint.c_str());

						ArkApi::GetApiUtils().SpawnDino(player, fblueprint, nullptr, level, true);
					}
				}
			}
		}
	}

	void SZEnterSettings(APlayerController*, FString* cmd, bool)
	{
		TArray<FString> parsed;
		cmd->ParseIntoArray(parsed, L" ", true);

		if (parsed.IsValidIndex(3))
		{
			const FString name = parsed[1];

			const auto safe_zone = SafeZoneManager::Get().FindZoneByName(name);
			if (!safe_zone)
				return;

			bool prevent_entering;
			bool prevent_leaving;

			try
			{
				prevent_entering = std::stoi(*parsed[2]) != 0;
				prevent_leaving = std::stoi(*parsed[3]) != 0;
			}
			catch (const std::exception&)
			{
				return;
			}

			safe_zone->prevent_entering = prevent_entering;
			safe_zone->prevent_leaving = prevent_leaving;
		}
	}

	void SZReloadConfig(APlayerController* player_controller, FString* cmd, bool)
	{
		AShooterPlayerController* shooter_controller = static_cast<AShooterPlayerController*>(player_controller);

		try
		{
			ReadConfig();
		}
		catch (const std::exception& error)
		{
			ArkApi::GetApiUtils().SendServerMessage(shooter_controller, {1, 0, 0}, "Failed to reload config");

			Log::GetLog()->error(error.what());
			return;
		}

		auto& default_safe_zones = SafeZoneManager::Get().GetDefaultSafeZones();
		for (const auto& name : default_safe_zones)
		{
			SafeZoneManager::Get().RemoveSafeZone(name);
		}

		SafeZoneManager::Get().ReadSafeZones();

		ArkApi::GetApiUtils().SendServerMessage(shooter_controller, {0, 1, 0}, "Reloaded config");
	}

	void Init()
	{
		auto& commands = ArkApi::GetCommands();

		commands.AddConsoleCommand("SZGiveItems", &SZGiveItems);
		commands.AddConsoleCommand("SZEnterSettings", &SZEnterSettings);
		commands.AddConsoleCommand("SZReloadConfig", &SZReloadConfig);
	}

	void Clean()
	{
		auto& commands = ArkApi::GetCommands();

		commands.RemoveConsoleCommand("SZGiveItems");
		commands.RemoveConsoleCommand("SZEnterSettings");
		commands.RemoveConsoleCommand("SZReloadConfig");
	}
}
