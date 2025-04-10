#pragma once

#include "../util/WayPoint.hpp"
#include "../interface/IComponent.hpp"
#include "../interface/IComponentVisitor.hpp"
#include "entt/entity/entity.hpp"
#include <memory>

class MovementPattern : public IComponent
{
public:
    MovementPattern();
    MovementPattern(WayPoint* wp, bool repeat = false);

    void accept(IComponentVisitor* visitor, entt::entity entityID) override;

    WayPoint* movePattern;
    WayPoint* currentPath;
    float distance;
    bool repeat;
};