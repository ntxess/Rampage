#pragma once
#include "entt/entt.hpp"

class Entity
{
private:
	const entt::entity m_entityId;
	entt::registry& m_reg;

public:
	Entity(std::reference_wrapper<entt::registry> reg);
	Entity(std::reference_wrapper<entt::registry> reg, entt::entity handle);
	Entity(const Entity& other) = default;
	~Entity();

	operator bool() const { return m_entityId != (entt::entity)0; }

	const entt::entity getId() const;

	template<typename T, typename... Args>
	T& addComponent(Args&&... args);

	template<typename T>
	T& getComponent() const;

	template<typename T>
	bool hasComponent() const;

	template<typename T>
	void removeComponent();
};

template<typename T, typename... Args>
inline T& Entity::addComponent(Args&&... args)
{
	return m_reg->emplace<T>(m_entityId, std::forward<Args>(args)...);
}

template<typename T>
inline T& Entity::getComponent() const
{
	return m_reg->get<T>(m_entityId);
}

template<typename T>
inline bool Entity::hasComponent() const
{
	return m_reg->all_of<T>(m_entityId);
}

template<typename T>
inline void Entity::removeComponent()
{
	m_reg->remove<T>(m_entityId);
}