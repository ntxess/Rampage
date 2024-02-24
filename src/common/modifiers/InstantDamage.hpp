#pragma once

#include "Modification.hpp"

struct InstantDamage : public Modification
{
	ModifierType type() const
	{
		return ModifierType::INSTANT;
	}

	void update(/*EntityStatus& entityStatus*/)
	{
		//entityStatus -= 
	}
 
private:
	bool modificationStatus = false;
};