#pragma once

#include "../interface/IComponent.hpp"
#include "../interface/IComponentVisitor.hpp"
#include "entt/entity/entity.hpp"
#include <utility>
#include <vector>

enum class EffectType;
class Effects;

class EffectsList : public IComponent
{
public:
    void accept(IComponentVisitor* visitor, entt::entity entityID) override;

    std::vector<std::pair<EffectType, Effects>> effectsList;
};