#include "MovementPattern.hpp"

MovementPattern::MovementPattern()
    : movePattern(nullptr), currentPath(nullptr), distance(0.0f), repeat(false) 
{}

MovementPattern::MovementPattern(WayPoint* wp, bool repeat)
    : movePattern(wp), currentPath(wp), distance(0.0f), repeat(repeat)
{}

void MovementPattern::accept(IComponentVisitor* visitor, entt::entity entityID)
{
    visitor->visit(this, entityID);
}