#pragma once

#include <API/ARK/Ark.h>

#ifdef ZONE_EXPORTS
#define ZONE_API __declspec(dllexport)
#else
#define ZONE_API __declspec(dllimport)
#endif

namespace SafeZones
{
	struct SafeZone
	{
		SafeZone(FString name, const FVector& position, int radius, bool prevent_pvp, bool prevent_structure_damage,
		         bool prevent_building, bool kill_wild_dinos, bool prevent_leaving, bool prevent_entering, bool enable_events,
		         bool screen_notifications, bool chat_notifications, const FLinearColor& success_color,
		         const FLinearColor& fail_color, std::vector<FString> messages)
			: name(std::move(name)),
			  position(position),
			  radius(radius),
			  prevent_pvp(prevent_pvp),
			  prevent_structure_damage(prevent_structure_damage),
			  prevent_building(prevent_building),
			  kill_wild_dinos(kill_wild_dinos),
			  prevent_leaving(prevent_leaving),
			  prevent_entering(prevent_entering),
			  enable_events(enable_events),
			  screen_notifications(screen_notifications),
			  chat_notifications(chat_notifications),
			  success_color(success_color),
			  fail_color(fail_color),
			  messages(std::move(messages))
		{
		}

		/**
		 * \brief Safe zone name
		 */
		FString name;

		/**
		 * \brief Sphere position
		 */
		FVector position;

		/**
		 * \brief Sphere radius
		 */
		int radius;

		bool prevent_pvp;
		bool prevent_structure_damage;
		bool prevent_building;

		/**
		 * \brief Kills wild dinos on entering to safe zone, requires events to be enabled
		 */
		bool kill_wild_dinos;

		/**
		 * \brief Doesn't allow players to leave safe zone
		 */
		bool prevent_leaving;

		bool prevent_entering;

		/**
		 * \brief Enables OnEnter/OnLeave events
		 */
		bool enable_events;

		bool screen_notifications;
		bool chat_notifications;

		FLinearColor success_color;
		FLinearColor fail_color;

		std::vector<FString> messages;

		/**
		 * \brief Actors that are currently in safe zone
		 */
		TArray<AActor*> actors;

		// Callbacks

		TArray<std::function<void(AActor*)>> on_actor_begin_overlap;
		TArray<std::function<void(AActor*)>> on_actor_end_overlap;
		TArray<std::function<bool(AShooterPlayerController*)>> can_join_zone;

		// Functions

		ZONE_API bool IsOverlappingActor(AActor* other) const;
		ZONE_API void SendNotification(AShooterPlayerController* player, const FString& message,
		                               const FLinearColor& color) const;

		void OnEnterSafeZone(AActor* other_actor);
		void OnLeaveSafeZone(AActor* other_actor);

		ZONE_API bool CanJoinZone(AShooterPlayerController* player) const;
	};
}
