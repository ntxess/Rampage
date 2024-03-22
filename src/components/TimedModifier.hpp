#pragma once

#include "IModifier.hpp"

struct TimedModifier : public IModifier
{
	inline ModifierType type() const override
	{
		return ModifierType::TIMED;
	}

	inline void applyTo(EntityStatus* status, const Effects& effect) override
	{
		if (status)
		{

		}
	}
};