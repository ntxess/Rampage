#include "Entity.hpp"

Entity::Entity(std::reference_wrapper<entt::registry> reg)
	: m_reg(reg)
{
	m_entityId = m_reg.create();
}

Entity::Entity(std::reference_wrapper<entt::registry> reg, entt::entity handle)
	: m_reg(reg)
	, m_entityId(handle)
{}

Entity::~Entity()
{
	// Entity is responsible for its on destruction
	m_reg.destroy(m_entityId);
}

const entt::entity Entity::getId() const
{
	return m_entityId;
}