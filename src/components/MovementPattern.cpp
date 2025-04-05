#include "MovementPattern.hpp"

void MovementPattern::accept(IComponentVisitor* visitor, entt::entity entityID)
{
    visitor->visit(this, entityID);
}