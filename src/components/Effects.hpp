#pragma once

#include <string>
#include <chrono>

struct Effects
{
	std::string statusToModify;
	float modificationVal = 0;
	std::chrono::milliseconds maxDuration = std::chrono::milliseconds(0);
	std::chrono::milliseconds tickRate = std::chrono::milliseconds(0);
};
