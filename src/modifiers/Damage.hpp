#pragma once

#include "Modification.hpp"
#include "ModificationDecorator.hpp"

struct Damage : ModificationDecorator
{
	Damage(std::unique_ptr<Modification> newMod) 
		: ModificationDecorator(std::move(newMod)) 
	{}

	inline const ModifierType type() const
	{
		return ModifierType::BASE;
	}

	inline void apply() override
	{
		mods->apply();
	}
};