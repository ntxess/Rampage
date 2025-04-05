#pragma once

#include <entt/entity/registry.hpp>
#include <string_view>

class System
{
public:
    virtual ~System() = default;
    virtual constexpr std::string_view name() const = 0;
    virtual void update(entt::registry& reg, const float& dt = 0.f, const entt::entity ent = entt::null) = 0;
};