#pragma once
#include <Windows.h>
#include "API\ARK\Ark.h"
bool PlayerCanTrack;
std::wstring PlayerTrackDinoSyntaxError, PlayerTrackDinoSyntaxError1;
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size = wstr.length() + 1;
	std::string strTo(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size = str.length() + 1;
	std::wstring wstrTo(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size);
	return wstrTo;
}