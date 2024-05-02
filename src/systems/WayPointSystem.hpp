#pragma once

#include "System.hpp"
#include "../components/Component.hpp"
#include <numbers>

class WayPointSystem : public System
{
public:
    WayPointSystem();

    constexpr std::string_view name();
    void update(entt::registry& reg, const float& dt = 0.f, const entt::entity ent = entt::null);
};

