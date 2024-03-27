#pragma once

#include "entt/entity/entity.hpp"

struct CollisionEvent
{
	CollisionEvent(entt::entity source, entt::entity reciever)
		: sourceID(source)
		, receiverID(reciever)
	{}

	entt::entity sourceID;
	entt::entity receiverID;
};