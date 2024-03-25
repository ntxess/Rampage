#pragma once

#include "Effects.hpp"
#include <vector>

enum class EffectType
{
	NULLTYPE,
	INSTANT,
	OVERTIME,
	TIMED,
};

struct EffectsList
{
	std::vector<std::pair<EffectType, Effects>> effectsList;
};