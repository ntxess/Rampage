#pragma once

#include "../systems/System.hpp"
#include "../systems/CollisionSystem.hpp"
#include <memory>
#include <typeindex>
#include <unordered_map>

//DEBUG
#include <iostream>
#include <chrono>

class SystemManager
{
public:
    template<typename T, typename... Args>
    void addSystem(Args&&... args)
    {
        m_systems.emplace(typeid(T), std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    T* getSystem() 
    {
        auto it = m_systems.find(typeid(T));
        if (it != m_systems.end())
            return static_cast<T*>(it->second.get());
        return nullptr;
    }

    void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null)
    {
        for (auto& [id, system] : m_systems)
        {
            auto start = std::chrono::high_resolution_clock::now();
            system->update(reg, dt, ent);
            auto stop = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

            if (system->name() == "CollisionSystem")
            {
                std::cout << GREEN << "[System] | CollisionSystem: " << duration << "ns\n" << RESET;
            }
        }
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};

