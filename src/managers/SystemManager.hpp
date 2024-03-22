#pragma once

#include "../systems/System.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/EventSystem.hpp"
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
        m_sequentialSystems.push_back(m_systems[typeid(T)].get());
    }

    template<typename T>
    T* getSystem() 
    {
        if (m_systems.count(typeid(T)))
            return static_cast<T*>(m_systems[typeid(T)].get());
        return nullptr;
    }

    template<typename T>
    void removeSystem()
    {
        if (m_systems.count(typeid(T)))
        {
            std::string_view type = m_systems[typeid(T)]->name();

            const auto it = std::find_if(m_sequentialSystems.begin(), m_sequentialSystems.end(), [type](System* system) {
                return system->name() == type;
            });

            m_sequentialSystems.erase(it);
            m_systems.erase(typeid(T));
        }
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
            else if (system->name() == "EventSystem")
            {
                std::cout << GREEN << "[System] | EventSystem: " << duration << "ns\n" << RESET;
            }
            else if (system->name() == "WayPointSystem")
            {
                std::cout << GREEN << "[System] | WayPointSystem: " << duration << "ns\n" << RESET;
            }
        }
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
    std::vector<System*> m_sequentialSystems;
};

