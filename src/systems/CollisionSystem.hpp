#pragma once

#include "System.hpp"
#include "../common/QuadTree.hpp"
#include "../common/events/Event.hpp"
#include "../components/Component.hpp"
#include <memory>
#include <concurrent_queue.h>

class CollisionSystem : public System
{
public:
	CollisionSystem(const sf::Vector2f& center, const sf::Vector2f& size, std::reference_wrapper<concurrency::concurrent_queue<std::shared_ptr<Event>>> queue)
		: m_left(center.x - (size.x / 2))
		, m_top(center.y - (size.y / 2))
		, m_width(size.x)
		, m_height(size.y)
		, m_globalQueue(queue)
	{}

	constexpr std::string_view name()
	{
		return "CollisionSystem";
	}

	void update(entt::registry& reg, const float& dt = 0.f)
	{
		quadTreeUpdate(reg);  // Rebuild the quadtree for querying collisions
		collisionUpdate(reg); // Find and mark all collided entity with a tag
	}

	void quadTreeUpdate(entt::registry& reg)
	{
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
			sf::FloatRect sourceHitbox = reg.get<Hitbox>(source).getBounds();
			std::vector<entt::entity> receiverList = m_quadTree->queryRange(reg, sourceHitbox);
			Team sourceTeamTag = reg.get<TeamTag>(source).tag;

			for (auto receiver : receiverList)
			{
				Team receiverTeamTag = reg.get<TeamTag>(receiver).tag;
				if (sourceTeamTag == receiverTeamTag)
					continue;
			}
		}
	}

private:
	float m_left;
	float m_top;
	float m_width;
	float m_height;
	std::reference_wrapper<concurrency::concurrent_queue<std::shared_ptr<Event>>> m_globalQueue;
	std::unique_ptr<QuadTree> m_quadTree;
};
