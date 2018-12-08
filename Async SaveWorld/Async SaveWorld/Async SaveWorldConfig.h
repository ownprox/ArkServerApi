#pragma once
#include <fstream>
#include "json.hpp"

void InitConfig()
{
	std::ifstream file(ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/Async SaveWorld/config.json");
	nlohmann::json json;
	file >> json;
	file.close();
	std::string data;
	data = json["CpuThreads"];

	TArray<int> EnabledCPUThreads;
	FString CpuThreadString = ArkApi::Tools::Utf8Decode(data).c_str();
	if (!CpuThreadString.IsEmpty())
	{
		int ThreadCount = std::thread::hardware_concurrency();
		TArray<FString> CPUThreadArray;
		CpuThreadString.ParseIntoArray(CPUThreadArray, L",", true);
		for (const FString& CpuThread : CPUThreadArray)
		{
			try
			{
				int ThreadIndex = std::stoi(*CpuThread);
				if (ThreadIndex >= ThreadCount)
				{
					Log::GetLog()->warn("You tried using Cpu Thread #{} but the max threads your server supports is {}.", ThreadIndex, ThreadCount);
				} else EnabledCPUThreads.Add(ThreadIndex);
			}
			catch (const std::exception&)
			{
				Log::GetLog()->error("Syntax error in CpuThreads Example: 0,4,5");
				break;
			}
		}
		
		if (EnabledCPUThreads.Num() > 0)
		{
			for (int i = 0; i < ThreadCount; i++)
			{
				DWORD_PTR Enabled = static_cast<DWORD_PTR>(EnabledCPUThreads.Contains(i) ? 1 : 0);
				CPUAffinityMask |= (Enabled << i);
				if (Enabled == 1)
				{
					if (!CPUAffinityEnabled) Log::GetLog()->info("CPU Affinity Enabled!");
					Log::GetLog()->info("Using CPU Thread[{}]!", i);
					CPUAffinityEnabled = true;
				}
			}
		}
	}
}