#pragma once

#include "../modifiers/Modification.hpp"
#include <vector>

struct ModPool
{
	std::vector<Modification> pool;

	ModPool() {}

	void add(const Modification& mods)
	{
		pool.push_back(mods);
	}
};