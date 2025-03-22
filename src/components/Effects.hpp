#pragma once

#include <string>
#include <chrono>

struct Effects
{
	std::string statusToModify;
	float modificationVal;
	std::chrono::milliseconds maxDuration;
	std::chrono::milliseconds tickRate;
};
