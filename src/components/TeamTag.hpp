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
	Team tag;

	TeamTag(Team tag) : tag(tag) {}
};