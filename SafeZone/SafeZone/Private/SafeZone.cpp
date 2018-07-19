#include "../Public/SafeZone.h"

#include <Permissions.h>

#include "SafeZones.h"
#include "SafeZoneManager.h"

namespace SafeZones
{
	bool SafeZone::IsOverlappingActor(AActor* other) const
	{
		return actors.Find(other) != INDEX_NONE;
	}

	void SafeZone::SendNotification(AShooterPlayerController* player, const FString& message,
	                                const FLinearColor& color) const
	{
		if (screen_notifications)
		{
			const float display_scale = config["NotificationScale"];
			const float display_time = config["NotificationDisplayTime"];

			ArkApi::GetApiUtils().SendNotification(player, color, display_scale, display_time,
			                                       nullptr, *message);
		}

		if (chat_notifications)
		{
			ArkApi::GetApiUtils().SendChatMessage(player, name,
			                                      fmt::format(L"<RichColor Color=\"{0}, {1}, {2}, {3}\">{4}</>", color.R,
			                                                  color.G, color.B, color.A, *message).c_str());
		}
	}

	void SafeZone::OnEnterSafeZone(AActor* other_actor)
	{
		if (!other_actor)
			return;

		if (other_actor->IsA(AShooterCharacter::GetPrivateStaticClass()))
		{
			AShooterPlayerController* player = ArkApi::GetApiUtils().FindControllerFromCharacter(
				static_cast<AShooterCharacter*>(other_actor));
			if (player)
			{
				if (prevent_leaving || prevent_entering)
				{
					auto& players_pos = SafeZoneManager::Get().players_pos;
					if (players_pos.find(player) != players_pos.end())
					{
						auto& player_pos = players_pos[player];

						player_pos.zone = this;
						player_pos.in_zone = true;

						if (!CanJoinZone(player))
						{
							SendNotification(player, messages[3], fail_color);

							const FVector& last_pos = player_pos.outzone_pos;
							player->SetPlayerPos(last_pos.X, last_pos.Y, last_pos.Z);
							return;
						}
					}
					else
					{
						players_pos[player] = {this, true, player->DefaultActorLocationField(), player->DefaultActorLocationField()};
					}
				}

				SendNotification(player, FString::Format(*messages[0], *name),
				                 success_color);
			}
		}
		else if (kill_wild_dinos && other_actor->TargetingTeamField() < 50000 &&
			other_actor->IsA(APrimalDinoCharacter::GetPrivateStaticClass()))
		{
			APrimalDinoCharacter* dino = static_cast<APrimalDinoCharacter*>(other_actor);
			dino->Suicide();
		}

		// Execute callbacks
		for (const auto& callback : on_actor_begin_overlap)
		{
			callback(other_actor);
		}
	}

	void SafeZone::OnLeaveSafeZone(AActor* other_actor)
	{
		if (!other_actor)
			return;

		if (other_actor->IsA(AShooterCharacter::GetPrivateStaticClass()))
		{
			AShooterPlayerController* player = ArkApi::GetApiUtils().FindControllerFromCharacter(
				static_cast<AShooterCharacter*>(other_actor));
			if (player)
			{
				if (prevent_leaving || prevent_entering)
				{
					auto& players_pos = SafeZoneManager::Get().players_pos;
					auto& player_pos = players_pos[player];

					if (player_pos.zone && !player_pos.zone->CanJoinZone(player))
					{
						player_pos.zone = nullptr;
						return;
					}

					if (prevent_leaving)
					{
						player_pos.in_zone = false;

						SendNotification(player, messages[4], fail_color);

						const FVector& last_pos = player_pos.inzone_pos;
						player->SetPlayerPos(last_pos.X, last_pos.Y, last_pos.Z);

						actors.Add(other_actor);
						return;
					}

					player_pos.zone = nullptr;
				}

				SendNotification(player, FString::Format(*messages[1], *name),
				                 fail_color);
			}
		}

		// Execute callbacks
		for (const auto& callback : on_actor_end_overlap)
		{
			callback(other_actor);
		}
	}

	bool SafeZone::CanJoinZone(AShooterPlayerController* player) const
	{
		bool result = true;

		if (prevent_entering)
		{
			const uint64 steam_id = ArkApi::IApiUtils::GetSteamIdFromController(player);

			result = Permissions::IsPlayerHasPermission(steam_id, "SafeZones." + name);
		}

		for (const auto& callback : can_join_zone)
		{
			result = callback(player);
		}

		return result;
	}
}
