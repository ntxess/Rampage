#pragma once

#include "entt/entity/entity.hpp"

struct CollisionEvent
{
	CollisionEvent(entt::entity source)
		: sourceID(source)
	{}

	entt::entity sourceID;
};