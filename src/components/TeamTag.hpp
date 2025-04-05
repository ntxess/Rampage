#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"

enum class Team
{
    UNAFFILIATED,
    FRIENDLY,
    ENEMY,
};

class TeamTag : public IComponent
{
public:
    TeamTag();
    TeamTag(Team tag);

    void accept(IComponentVisitor* visitor, entt::entity entityID) override;

    Team tag;
};