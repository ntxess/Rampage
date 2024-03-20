#include "WayPointSystem.hpp"

WayPointSystem::WayPointSystem()
{
}

constexpr std::string_view WayPointSystem::name()
{
    return "WayPointSystem";
}

void WayPointSystem::update(entt::registry& reg, const float& dt, entt::entity ent)
{
    //auto group = reg.group<MovementPattern, Speed>(entt::get<AnimatedSprite>);
    //for (auto entity : group)
    //{
    //    auto [wpc, spd, sp] = group.get<MovementPattern, Speed, AnimatedSprite>(entity);

    //    Waypoint* headPtr = wpc.currentPath;
    //    Waypoint* nextPtr = headPtr->nextWP.get();

    //    if (nextPtr == nullptr)
    //    {
    //        if (wpc.repeat)
    //        {
    //            wpc.currentPath = wpc.movePattern;
    //            wpc.distance = 0.f;
    //        }
    //        continue;
    //    }

    //    wpc.distance += spd.current * dt;
    //    if (wpc.distance > nextPtr->distanceTotal)
    //        wpc.currentPath = nextPtr;

    //    sf::Vector2f unitDist;
    //    unitDist.x = (nextPtr->location.x - headPtr->location.x) / headPtr->distanceToNext;
    //    unitDist.y = (nextPtr->location.y - headPtr->location.y) / headPtr->distanceToNext;

    //    sf::Vector2f velocity;
    //    velocity.x = unitDist.x * spd.current * dt;
    //    velocity.y = unitDist.y * spd.current * dt;

    //    float theta = (atan2(velocity.y, velocity.x)) * (180.f / float(std::numbers::pi));
    //    sp.sprite.setRotation(theta + 90);
    //    sp.sprite.move(velocity);
    //}
}
