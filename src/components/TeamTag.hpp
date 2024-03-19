#pragma once

#include "../common/CommonEnum.hpp"

enum class Team
{
	UNAFFILIATED,
	FRIENDLY,
	ENEMY,
};

struct TeamTag
{
	TeamTag(Team tag) 
		: tag(tag) 
	{}

	Team tag;
};