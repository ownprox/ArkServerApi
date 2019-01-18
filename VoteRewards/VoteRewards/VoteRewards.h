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
#include "VoteRewardsConfig.h"
#include <random>
#include "Requests.h"

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

struct PlayerDataS
{
	long double NextVoteTime[TotalVoteSites];
	bool HasRewarded;
};
std::map<int64, PlayerDataS> PlayerData;


const int GetVoteSiteIndex(const std::string& VoteSite)
{
	for (int i = 0; i < VoteSites.size(); i++) if (VoteSites[i].Site == VoteSite) return i;
	return -1;
}

int RandomNumber(int min, int max)
{
	std::default_random_engine generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(min, max);
	int rnd = distribution(generator);
	return rnd;
}