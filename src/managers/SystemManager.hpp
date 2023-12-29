#pragma once

#include "../common/System.hpp"
#include <memory>
#include <typeindex>
#include <unordered_map>

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
        if (m_systems.find(typeid(T)) != m_systems.end())
            return static_cast<T*>(it->second.get());
        return nullptr;
    }

    void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null)
    {
        for (auto& [id, system] : m_systems)
            system->update(reg, dt, ent);
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};

