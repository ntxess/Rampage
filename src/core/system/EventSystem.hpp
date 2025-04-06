#pragma once

#include "../Components.hpp"
#include "../interface/ISystem.hpp"
#include "../util/Logger.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
#include <chrono>
#include <string_view>

class EventSystem : public ISystem
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
    void update(entt::registry& reg, const float& dt = 0.f);

    EventStatus apply(const EffectType effectType, StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;
    EventStatus instantEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;
    EventStatus overTimeEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;
    EventStatus tempTimedEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const;

private:
    const std::chrono::milliseconds m_eventWatchdogTime;
};