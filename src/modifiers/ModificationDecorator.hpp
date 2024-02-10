#pragma once

#include "../common/CommonEnum.hpp"
#include "Modification.hpp"

struct ModificationDecorator : public Modification
{
public:
	ModificationDecorator(std::unique_ptr<Modification> newMod)
		: mods(std::move(newMod)) 
	{}

	inline const ModifierType type() const 
	{
		return ModifierType::BASE;
	}

	inline void apply() override
	{
		mods->apply();
	}

protected:
	std::unique_ptr<Modification> mods;
};