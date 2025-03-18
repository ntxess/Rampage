#pragma once

#include "System.hpp"
#include "../components/Component.hpp"
#include <entt/entity/entity.hpp>

class EventSystem : public System
{
private:
	enum class EventStatus
	{
		INCOMPLETE,
		COMPLETE,
	};

public:
	EventSystem() = default;
	EventSystem(time_t delayTime, time_t watchdogTime);

	constexpr std::string_view name() const;
	void update(entt::registry& reg, const float& dt = 0.f, const entt::entity ent = entt::null);

	EventStatus apply(entt::registry& reg, const EffectType effectType, StatusModEvent& statusModEvent) const;
	EventStatus instantEvent(EntityStatus& receiverStatus, StatusModEvent& statusModEvent) const;
	EventStatus overTimeEvent(EntityStatus& receiverStatus, StatusModEvent& statusModEvent) const;
	EventStatus tempTimedEvent(EntityStatus& receiverStatus, StatusModEvent& statusModEvent) const;

private:
	const time_t m_delayTime;
	const time_t m_eventWatchdogTime;
};