#pragma once

#include <entt/entt.hpp>

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void execute(entt::registry& reg) = 0;
};