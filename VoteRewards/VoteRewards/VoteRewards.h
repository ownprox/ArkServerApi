#pragma once
#ifdef ATLAS
#include "API\Atlas\Atlas.h"
#pragma comment(lib, "AtlasApi.lib")
#pragma comment(lib, "AtlasShop.lib")
#include "../../../2. AtlasServerApi/AtlasShop/AtlasShop/Public/Points.h"
#else
#include "API\ARK\Ark.h"
#pragma comment(lib, "ArkApi.lib")
#pragma comment(lib, "ArkShop.lib")
#include "../../../MichsPlugins/ArkShop/ArkShop/Public/Points.h"
#endif
#include "Requests.h"
#include <random>

std::default_random_engine intGen;
std::mt19937 floatGen(std::random_device{}());
struct VoteSiteConfig
{
	const char * const Site;
	const char * const Response;
	bool ExactMatch;

	constexpr VoteSiteConfig(const char * const Site, const char * const Response, const bool ExactMatch) : Site(Site), Response(Response), ExactMatch(ExactMatch) {}
};
#ifdef ATLAS
constexpr int TotalVoteSites = 3;
constexpr std::array<VoteSiteConfig, TotalVoteSites> VoteSites{
	VoteSiteConfig("atlasserverslist.com", "Success", false),
	VoteSiteConfig("trackyserver.com", "1", true),
	VoteSiteConfig("atlasserverlist.com", "1", true)
};
#else
constexpr int TotalVoteSites = 3;
constexpr std::array<VoteSiteConfig, TotalVoteSites> VoteSites{
	VoteSiteConfig("ark-servers.net", "1", true),
	VoteSiteConfig("trackyserver.com", "1", true),
	VoteSiteConfig("toparkservers.com", "1", true)};
#endif

bool HasShop = false;
int TotalVoteSitesInConfig = 0;

struct PlayerDataS
{
	long double NextVoteTime[TotalVoteSites];
	bool HasRewarded;
};
std::map<int64, PlayerDataS> PlayerData;

struct VoteQueueS
{
	int VoteSiteIndex;
	std::string VoteKey;
	int64 SteamID;
	AShooterPlayerController* player;
	bool IsLast;
	VoteQueueS(const int VoteSiteIndex, const std::string VoteKey,
		const int64 SteamID, AShooterPlayerController* player, 	const bool IsLast)
		: VoteSiteIndex(VoteSiteIndex), VoteKey(VoteKey),
		SteamID(SteamID), player(player), IsLast(IsLast) {}
};

std::vector<VoteQueueS> VoteQueue;

const int GetVoteSiteIndex(const std::string& VoteSite)
{
	for (int i = 0; i < VoteSites.size(); i++) if (VoteSites[i].Site == VoteSite) return i;
	return -1;
}

float RandomNumber(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(floatGen);
}

int RandomNumber(int min, int max)
{
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(intGen);
}