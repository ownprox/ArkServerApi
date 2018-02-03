#pragma once
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include "json.hpp"
#include "API\ARK\Ark.h"
#pragma comment(lib, "Ws2_32.lib")
WSADATA wsaData;
SOCKET Socket;
SOCKADDR_IN SockAddr;
struct hostent *host;
const FString PName = "[Plugin Checker]";
const std::string FindStarter = "muted\">";

class Timer
{
public:
	template <class callable, class... arguments>
	Timer(int after, bool async, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments ...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		if (async)
		{
			std::thread([after, task]()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(after));
				task();
			}).detach();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}
	}
};

const float GetLastestPluginVer(char ResourceURL[])
{
	std::string website_HTML;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 0;
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	host = gethostbyname("arkserverapi.com");
	if (!host) return 0;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) return 0;
	send(Socket, ResourceURL, (int)strlen(ResourceURL), 0);
	char buffer[8192];
	while (recv(Socket, buffer, 8192, 0) > 0) website_HTML += buffer;
	closesocket(Socket);
	WSACleanup();
	if (website_HTML.find("HTTP/1.1 301 Moved Permanently") != std::string::npos)
	{
		const size_t FindPos = website_HTML.find("/arkserverapi.com/");
		if (FindPos != std::string::npos)
		{
			website_HTML = website_HTML.erase(0, (FindPos + 18));
			const size_t EndOfHttp = website_HTML.find('\n');
			if (EndOfHttp != std::string::npos)
			{
				website_HTML = website_HTML.substr(0, EndOfHttp - 2);
				if (website_HTML.length() > 3)
				{
					sprintf_s(ResourceURL, 255, "GET /%s/ HTTP/1.1\r\nHost: arkserverapi.com\r\nConnection: close\r\n\r\n", website_HTML.c_str());
					return GetLastestPluginVer(ResourceURL);
				}
			}
		}
		return 0;
	}
	const size_t FindStart = website_HTML.find(FindStarter);
	if (FindStart != std::string::npos)
	{
		website_HTML = website_HTML.erase(0, (FindStart + FindStarter.length()));
		const size_t FindEnd = website_HTML.find('<');
		if (FindEnd != std::string::npos)
		{
			try
			{
				const float PluginVer = std::stof(website_HTML.substr(0, FindEnd).c_str());
				return PluginVer;
			}
			catch (...) {}
		}
	}
	return 0;
}

void CheckLatestPluginsVersions(AShooterPlayerController* Player)
{
	namespace fs = std::experimental::filesystem;
	const std::string dir_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins";
	nlohmann::json Config;
	char ResourceURL[255];
	for (const auto& dir_name : fs::directory_iterator(dir_path))
	{
		const auto& path = dir_name.path();
		if (!is_directory(path)) continue;
		const auto filename = path.filename().stem().generic_string();
		const auto ConfigName = dir_path + "/" + filename + "/PluginInfo.json";
		if (!fs::exists(ConfigName)) continue;
		std::ifstream file(ConfigName);
		if (!file.is_open()) continue;
		try
		{
			file >> Config;
			const int ResourceId = Config.value("ResourceId", -1);
			if (ResourceId == -1) continue;
			sprintf_s(ResourceURL, 255, "GET /resources/%d/ HTTP/1.1\r\nHost: arkserverapi.com\r\nConnection: close\r\n\r\n", ResourceId);
			const float Version = GetLastestPluginVer(ResourceURL), CurrentVersion = Config["Version"];
			if (Version != 0 && CurrentVersion != Version)
			{
				Log::GetLog()->error("{0} {1:.1f} is outdated, latest version: {2:.1f}", filename.c_str(), CurrentVersion, Version);
				if (Player)	ArkApi::GetApiUtils().SendChatMessage(Player, PName, L"<RichColor Color=\"0.8, 0.8, 0, 1\">{0} {1:.1f} is outdated, latest version: {2:.1f}</>", filename.c_str(), CurrentVersion, Version);
			}
		}
		catch (const std::exception& error)
		{
			Log::GetLog()->error(error.what());
			throw;
		}
		file.close();
	}
}