#pragma once

#include "entt/entity/entity.hpp"

struct CollisionEvent
{
	CollisionEvent(entt::entity source, entt::entity reciever)
		: sourceID(source)
		, receiverID(reciever)
	{}

	const entt::entity sourceID;
	const entt::entity receiverID;
};