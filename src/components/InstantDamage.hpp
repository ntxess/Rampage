#pragma once

#include "Modifier.hpp"

struct InstantDamage : public Modifier
{
	inline ModifierType type() const override
	{
		return ModifierType::INSTANT;
	}

	inline void applyTo(EntityStatus* status, int val) override
	{
		if (status)
		{
			status->health -= 1;
		}
	}
};