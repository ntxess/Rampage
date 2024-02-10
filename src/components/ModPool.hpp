#pragma once

#include "../modifiers/Modifier.hpp"
#include <vector>

struct ModPool
{
	std::vector<Modifier> pool;

	ModPool() {}

	void add(const Modifier& mods)
	{
		pool.push_back(mods);
	}
};