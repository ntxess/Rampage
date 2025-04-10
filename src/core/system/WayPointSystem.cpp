#include "WayPointSystem.hpp"

WayPointSystem::WayPointSystem()
{}

constexpr std::string_view WayPointSystem::name() const
{
    return "WayPointSystem";
}

void WayPointSystem::update(entt::registry& reg, const float& dt)
{
    const auto& group = reg.group<MovementPattern, EntityStatus>(entt::get<Sprite>);
    for (auto entity : group)
    {
        auto& wpc = group.get<MovementPattern>(entity);
        auto& es = group.get<EntityStatus>(entity);
        auto& sp = group.get<Sprite>(entity);

        //auto [wpc, es, sp] = group.get<MovementPattern, EntityStatus, Sprite>(entity);

        if (!es.values.count("Speed")) return;
        
        const float speed = es.values.at("Speed");

        WayPoint* headPtr = wpc.currentPath;
        WayPoint* nextPtr = headPtr->nextWP;

        if (nextPtr == nullptr)
        {
            if (wpc.repeat)
            {
                wpc.currentPath = wpc.movePattern;
                wpc.distance = 0.f;
            }
            continue;
        }

        wpc.distance += speed * dt;
        if (wpc.distance > nextPtr->distanceTotal)
            wpc.currentPath = nextPtr;

        sf::Vector2f unitDist;
        unitDist.x = (nextPtr->coordinate.x - headPtr->coordinate.x) / headPtr->distanceToNext;
        unitDist.y = (nextPtr->coordinate.y - headPtr->coordinate.y) / headPtr->distanceToNext;

        sf::Vector2f velocity;
        velocity.x = unitDist.x * speed * dt;
        velocity.y = unitDist.y * speed * dt;

        float theta = (std::atan2(velocity.y, velocity.x)) * (180.f / float(std::numbers::pi));
        sp.sprite.setRotation(theta + 90);
        sp.sprite.move(velocity);
    }
}