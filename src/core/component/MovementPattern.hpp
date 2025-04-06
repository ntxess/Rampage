#pragma once

#include "../interface/IComponent.hpp"
#include "../interface/IComponentVisitor.hpp"
#include "entt/entity/entity.hpp"

class MovementPattern : public IComponent
{
public:
    void accept(IComponentVisitor* visitor, entt::entity entityID) override;
};