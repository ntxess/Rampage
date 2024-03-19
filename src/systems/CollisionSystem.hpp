#pragma once

#include "System.hpp"
#include "../common/QuadTree.hpp"
#include "../components/Component.hpp"
#include <memory>

class CollisionSystem : public System
{
public:
	CollisionSystem(const sf::Vector2f& center, const sf::Vector2f& size)
		: m_left(center.x - (size.x / 2))
		, m_top(center.y - (size.y / 2))
		, m_width(size.x)
		, m_height(size.y)
		, m_quadTree(nullptr)
	{}

	constexpr std::string_view name()
	{
		return "CollisionSystem";
	}

	void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null)
	{
		quadTreeUpdate(reg);  // Rebuild the quadtree for querying collisions
		collisionUpdate(reg); // Find and mark all collided entity with a tag
	}

private:
	void quadTreeUpdate(entt::registry& reg)
	{
		// Create/Update Quadtree with new entity positions
		m_quadTree = std::make_unique<QuadTree>(sf::FloatRect(m_left, m_top, m_width, m_height));
		auto view = reg.view<Hitbox>();

		for (auto entity : view)
			m_quadTree->insert(reg, entity);
	}

	void collisionUpdate(entt::registry& reg)
	{
		auto view = reg.view<Hitbox>();
		for (auto source : view)
		{
			// Query all neighboring entity for collision
			sf::FloatRect sourceHitbox = reg.get<Hitbox>(source).getBounds();
			std::vector<entt::entity> receiverList = m_quadTree->queryRange(reg, sourceHitbox);
			Team sourceTeamTag = reg.get<TeamTag>(source).tag;

			for (auto receiver : receiverList)
			{
				// Check if entity is on the same team
				Team receiverTeamTag = reg.get<TeamTag>(receiver).tag;
				if (sourceTeamTag == receiverTeamTag)
					continue;

				// If not on the same team, create new CollisionEvent for later processing
				Entity collisionEvent(reg);
				collisionEvent.addComponent<CollisionEvent>(source, receiver);
			}
		}
	}

private:
	float m_left;
	float m_top;
	float m_width;
	float m_height;
	std::unique_ptr<QuadTree> m_quadTree;
};
