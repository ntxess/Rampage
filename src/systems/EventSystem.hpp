#pragma once

#include "System.hpp"
#include "../components/Component.hpp"

class EventSystem : public System
{
public:
	EventSystem() {}

	constexpr std::string_view name()
	{
		return "EventSystem";
	}

	void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null)
	{
		auto view = reg.view<CollisionEvent>();
		for (auto event : view)
		{
			entt::entity source = reg.get<CollisionEvent>(event).sourceID;
			entt::entity receiver = reg.get<CollisionEvent>(event).receiverID;
			



		}
	}

private:
	void resolveEffects()
	{

	}
};