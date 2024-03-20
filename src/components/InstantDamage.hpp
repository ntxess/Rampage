#pragma once

#include "Modifier.hpp"

struct InstantDamage : public Modifier
{
	inline const ModifierType type() const
	{
		return ModifierType::BASE;
	}

	inline void apply(std::any& status) override
	{

	}
};