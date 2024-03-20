#pragma once

#include "Modifier.hpp"

struct InstantDamage : public Modifier
{
	inline const ModifierType type() const
	{
		return ModifierType::INSTANT;
	}

	inline void apply(std::any& status) override
	{

	}
};