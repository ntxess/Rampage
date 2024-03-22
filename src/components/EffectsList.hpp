#pragma once

#include "IModifier.hpp"
#include "Effects.hpp"
#include <vector>

struct EffectsList
{
	std::vector<std::pair<ModifierType, Effects>> effectsList;
};