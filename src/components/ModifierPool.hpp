#pragma once

#include "Modifier.hpp"
#include <vector>

struct ModifierPool
{
	std::vector<std::pair<ModifierType, int>> queue;
};