#pragma once

#include <entt/entt.hpp>
#include <string_view>

class System
{
public:
	virtual ~System() = default;
	virtual constexpr std::string_view name() = 0;
	virtual void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null) = 0;
};

