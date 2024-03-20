#pragma once

#include "System.hpp"
#include "../components/Component.hpp"

class EventSystem : public System
{
public:
	EventSystem();

	constexpr std::string_view name();
	void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null);

private:
	void resolveEffects();
};