#pragma once

#include "IModifier.hpp"

struct InstantModifier : public IModifier
{
	inline ModifierType type() const override
	{
		return ModifierType::INSTANT;
	}

	inline void applyTo(EntityStatus* status, const Effects& effect) override
	{
		if (status)
		{

		}
	}
};