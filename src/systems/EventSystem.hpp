#pragma once

#include "System.hpp"
#include "../components/Component.hpp"
#include <entt/entity/entity.hpp>

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
	void update(entt::registry& reg, const float& dt = 0.f, const entt::entity ent = entt::null);

	EventStatus apply(const EffectType effectType, EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress = nullptr);
	EventStatus instantEvent(EntityStatus& stats, const Effects& effect);
	EventStatus overTimeEvent(EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress);
	EventStatus fixedTimeEvent(EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress);
};