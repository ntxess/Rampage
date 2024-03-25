#pragma once

#include "System.hpp"
#include "../common/Entity.hpp"
#include "../components/Component.hpp"

enum class EventStatus
{
	FAILED,
	INCOMPLETE,
	COMPLETE,
};

class EventSystem : public System
{
public:
	EventSystem();

	constexpr std::string_view name();
	void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null);

	EventStatus apply(const EffectType modType, EntityStatus* stats, const Effects* effect);
	EventStatus instantEvent();
	EventStatus overTimeEvent();
	EventStatus fixedTimeEvent();
};