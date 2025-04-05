#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"
#include <string>
#include <unordered_map>

class EntityStatus : public IComponent
{
public:
    void accept(IComponentVisitor* visitor, entt::entity entityID) override;

    std::unordered_map<std::string, float> values;
};