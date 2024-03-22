#pragma once

#include "IModifier.hpp"

struct OvertimeModifier : public IModifier
{
	inline ModifierType type() const override
	{
		return ModifierType::OVERTIME;
	}

	inline void applyTo(EntityStatus* status, const Effects& effect) override
	{
		if (status)
		{

		}
	}
};