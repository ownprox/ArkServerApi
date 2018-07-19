#include "SafeZoneManager.h"

#include <Permissions.h>

#include "SafeZones.h"

namespace SafeZones
{
	SafeZoneManager& SafeZoneManager::Get()
	{
		static SafeZoneManager instance;
		return instance;
	}

	void SafeZoneManager::ReadSafeZones()
	{
		const auto& safe_zones = config["SafeZones"];
		for (const auto& safe_zone : safe_zones)
		{
			auto config_position = safe_zone["Position"];
			auto config_success_color = safe_zone["SuccessNotificationColor"];
			auto config_fail_color = safe_zone["FailNotificationColor"];

			std::string str_name = safe_zone["Name"];
			FString name = str_name.c_str();

			FVector position{config_position[0], config_position[1], config_position[2]};
			int radius = safe_zone["Radius"];
			bool prevent_pvp = safe_zone["PreventPVP"];
			bool prevent_structure_damage = safe_zone["PreventStructureDamage"];
			bool prevent_building = safe_zone["PreventBuilding"];
			bool kill_wild_dinos = safe_zone["KillWildDinos"];
			bool prevent_leaving = safe_zone["PreventLeaving"];
			bool prevent_entering = safe_zone["PreventEntering"];

			bool enable_events = safe_zone["EnableEvents"];
			bool screen_notifications = safe_zone["ScreenNotifications"];
			bool chat_notifications = safe_zone["ChatNotifications"];

			FLinearColor success_color{
				config_success_color[0], config_success_color[1], config_success_color[2], config_success_color[3]
			};
			FLinearColor fail_color{config_fail_color[0], config_fail_color[1], config_fail_color[2], config_fail_color[3]};

			std::vector<FString> messages;
			for (const auto& msg : safe_zone["Messages"])
			{
				messages.emplace_back(ArkApi::Tools::Utf8Decode(msg).c_str());
			}

			// Add zone name to the default zones list
			default_safezones_.Add(name);

			CreateSafeZone(std::make_shared<SafeZone>(name, position, radius, prevent_pvp, prevent_structure_damage,
			                                          prevent_building, kill_wild_dinos, prevent_leaving, prevent_entering,
			                                          enable_events, screen_notifications, chat_notifications, success_color,
			                                          fail_color, messages));
		}
	}

	bool SafeZoneManager::CreateSafeZone(const std::shared_ptr<SafeZone>& safe_zone)
	{
		const auto find_zone = FindZoneByName(safe_zone->name);
		if (find_zone)
			return false;

		all_safezones_.Add(safe_zone);

		return true;
	}

	bool SafeZoneManager::RemoveSafeZone(const FString& name)
	{
		const auto find_zone = FindZoneByName(name);
		if (!find_zone)
			return false;

		for (auto iter = players_pos.begin(); iter != players_pos.end();)
		{
			if (iter->second.zone == find_zone.get())
			{
				players_pos.erase(iter);
				break;
			}

			++iter;
		}

		all_safezones_.RemoveSingle(find_zone);
		default_safezones_.RemoveSingle(name);

		return true;
	}

	/*ATriggerBase* SafeZoneManager::SpawnSphere(FVector& location, int radius, const std::string& type)
	{
		FActorSpawnParameters spawn_parameters;
		FRotator rotation{0, 0, 0};

		UClass* sphere_class = static_cast<UClass*>(TClassCompiledInDefer_ATriggerSphere::Register());

		ATriggerBase* trigger_sphere = static_cast<ATriggerBase*>(ArkApi::GetApiUtils().GetWorld()->SpawnActor(
			sphere_class, &location, &rotation,
			&spawn_parameters));

		USphereComponent* sphere_component = static_cast<USphereComponent*>(
			trigger_sphere->CollisionComponentField()().Object);

		const ECollisionResponse collision_type = //type == "Overlap"
			//? 
			ECollisionResponse::ECR_Overlap;
		//: ECollisionResponse::ECR_Block;

		sphere_component->bGenerateOverlapEvents() = true;
		sphere_component->SetCollisionResponseToAllChannels(collision_type);
		sphere_component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		sphere_component->SetCollisionObjectType(ECollisionChannel::ECC_OverlapAll_Deprecated);

		sphere_component->SetSphereRadius(static_cast<float>(radius), true);

		return trigger_sphere;
	}*/

	bool SafeZoneManager::CanBuild(APlayerController* player, const FVector& location, bool notification)
	{
		const bool admins_ignore = config["AdminsIgnoreRestrictions"];

		const uint64 steam_id = ArkApi::IApiUtils::GetSteamIdFromController(player);
		if (admins_ignore && Permissions::IsPlayerInGroup(steam_id, "Admins"))
			return true;

		for (const auto& safe_zone : all_safezones_)
		{
			if (safe_zone->prevent_building && FVector::Distance(safe_zone->position, location) <= safe_zone->radius)
			{
				if (FString msg = safe_zone->messages[2];
					!msg.IsEmpty() && notification)
				{
					safe_zone->SendNotification(static_cast<AShooterPlayerController*>(player), msg, safe_zone->fail_color);
				}

				return false;
			}
		}

		return true;
	}

	bool SafeZoneManager::CheckActorAction(AActor* actor, int type)
	{
		bool is_protected = false;

		for (const auto& safe_zone : all_safezones_)
		{
			if (safe_zone->IsOverlappingActor(actor))
			{
				switch (type)
				{
				case 1:
					is_protected = safe_zone->prevent_pvp;
					break;
				case 2:
					is_protected = safe_zone->prevent_structure_damage;
					break;
				}
				break;
			}
		}

		return is_protected;
	}

	void SafeZoneManager::UpdateOverlaps()
	{
		UWorld* world = ArkApi::GetApiUtils().GetWorld();
		if (!world)
			return;

		for (const auto& safe_zone : all_safezones_)
		{
			// Make a copy of the old actors array
			TArray<AActor*> old_actors = safe_zone->actors;

			TArray<AActor*> new_actors;

			TArray<AActor*> actors_ignore;
			TArray<TEnumAsByte<enum EObjectTypeQuery>> types;

			UKismetSystemLibrary::SphereOverlapActors_NEW(world, safe_zone->position, static_cast<float>(safe_zone->radius),
			                                              &types, nullptr, &actors_ignore, &new_actors);

			// Update safe zone actors list
			safe_zone->actors = new_actors;

			if (safe_zone->enable_events)
			{
				for (int i = 0; i < old_actors.Num() && new_actors.Num() > 0; ++i)
				{
					const bool allow_shrinking = false;
					if (new_actors.RemoveSingleSwap(old_actors[i], allow_shrinking) > 0)
					{
						old_actors.RemoveAtSwap(i, 1, allow_shrinking);
						--i;
					}
				}

				// old_actors now contains only previous overlaps
				for (const auto& actor : old_actors)
				{
					safe_zone->OnLeaveSafeZone(actor);
				}

				// new_actors now contains only new overlaps
				for (const auto& actor : new_actors)
				{
					safe_zone->OnEnterSafeZone(actor);
				}
			}
		}

		// Update player positions
		const auto& player_controllers = world->PlayerControllerListField();
		for (TWeakObjectPtr<APlayerController> player_controller : player_controllers)
		{
			AShooterPlayerController* player = static_cast<AShooterPlayerController*>(player_controller.Get());
			if (player)
			{
				const FVector pos = player->DefaultActorLocationField();

				if (players_pos.find(player) == players_pos.end())
				{
					players_pos[player] = {nullptr, false, pos, pos};
					continue;
				}

				auto& player_pos = players_pos[player];
				if (player_pos.zone && player_pos.in_zone)
				{
					if (!player_pos.zone->CanJoinZone(player))
					{
						const FVector& last_pos = player_pos.outzone_pos;
						player->SetPlayerPos(last_pos.X, last_pos.Y, last_pos.Z);
					}

					player_pos.inzone_pos = pos;
				}
				else
				{
					if (player_pos.zone && !player_pos.in_zone && player_pos.zone->IsOverlappingActor(player->CharacterField()))
					{
						player_pos.in_zone = true;
						continue;
					}

					player_pos.outzone_pos = pos;
				}
			}
		}
	}

	std::shared_ptr<SafeZone> SafeZoneManager::FindZoneByName(const FString& name)
	{
		const auto safe_zone = all_safezones_.FindByPredicate([&name](const auto& safe_zone)
		{
			return safe_zone->name == name;
		});

		if (!safe_zone)
			return nullptr;

		return *safe_zone;
	}

	TArray<std::shared_ptr<SafeZone>>& SafeZoneManager::GetAllSafeZones()
	{
		return all_safezones_;
	}

	TArray<FString>& SafeZoneManager::GetDefaultSafeZones()
	{
		return default_safezones_;
	}

	// Free function
	ISafeZoneManager& GetSafeZoneManager()
	{
		return SafeZoneManager::Get();
	}
}
