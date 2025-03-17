#pragma once

#include "Effects.hpp"
#include <vector>

enum class EffectType
{
	NULLTYPE,
	INSTANT,
	OVERTIME,
	TIMEDTEMP,
};

struct EffectsList
{
	std::vector<std::pair<EffectType, Effects>> effectsList;
};