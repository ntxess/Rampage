#pragma once

#include "System.hpp"
#include "../common/Logger.hpp"
#include "../components/Component.hpp"
#include <entt/entity/entity.hpp>
#include <chrono>

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
	EventSystem(std::chrono::milliseconds watchdogTime);

	constexpr std::string_view name() const;
	void update(entt::registry& reg, const float& dt = 0.f, const entt::entity ent = entt::null);

	EventStatus apply(const EffectType effectType, StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;
	EventStatus instantEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;
	EventStatus overTimeEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;
	EventStatus tempTimedEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;

private:
	const std::chrono::milliseconds m_eventWatchdogTime;
};