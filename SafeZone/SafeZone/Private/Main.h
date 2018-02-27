#pragma once
#include "json.hpp"
#include <fstream>
nlohmann::json SafeZoneConfig;
int NotificationCheckSeconds;
std::wstring ClaimItemsCommand;
class Timer
{
public:
	template <class callable, class... arguments>
	Timer(int after, callable&& f, arguments&&... args)
	{
		std::function<typename std::result_of<callable(arguments ...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
		std::thread([after, task]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(after));
			task();
		}).detach();
	}
};
