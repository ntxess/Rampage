#include "SystemManager.hpp"

void SystemManager::update(entt::registry& reg, const float& dt)
{
    for (auto& [id, system] : m_systems)
    {
        auto start = std::chrono::high_resolution_clock::now();
        system->update(reg, dt);
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

        if (system->name() == "CollisionSystem")
        {
            LOG_TRACE(Logger::get()) << "CollisionSystem: " << duration << "ns";
        }
        else if (system->name() == "EventSystem")
        {
            LOG_TRACE(Logger::get()) << "EventSystem: " << duration << "ns";
        }
        else if (system->name() == "WayPointSystem")
        {
            LOG_TRACE(Logger::get()) << "WayPointSystem: " << duration << "ns";
        }
    }
}