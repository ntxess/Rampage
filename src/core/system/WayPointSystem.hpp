#pragma once

#include "../Components.hpp"
#include "../interface/ISystem.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
#include <numbers>
#include <string_view>

class WayPointSystem : public ISystem
{
public:
    WayPointSystem();

    constexpr std::string_view name() const;
    void update(entt::registry& reg, const float& dt = 0.f);
};