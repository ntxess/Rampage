#pragma once

#include "System.hpp"
#include "../common/QuadTree.hpp"
#include "../components/Component.hpp"
#include <memory>

class CollisionSystem : public System
{
public:
	CollisionSystem(const sf::Vector2f& center, const sf::Vector2f& size)
		: m_left(center.x - (size.x / 2)), m_top(center.y - (size.y / 2)), m_width(size.x), m_height(size.y)
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

	void quadTreeUpdate(entt::registry& reg)
	{
		//m_quadTree = std::make_unique<QuadTree>(sf::FloatRect(m_left, m_top, m_width, m_height));
		//auto view = reg.view<Hitbox>();

		//for (auto entity : view)
		//	m_quadTree->Insert(reg, entity);
	}

	void collisionUpdate(entt::registry& reg)
	{
		//auto view = reg.view<Hitbox>();
		//for (auto collider : view)
		//{
		//	auto& colliderHitbox = reg.get<Sprite>(collider).sprite;
		//	std::vector<entt::entity> entityCollidedWith = m_quadTree->QueryRange(reg, colliderHitbox.getGlobalBounds());
		//	auto colliderTeamTag = reg.get<TeamTag>(collider).tag;

		//	for (auto collided : entityCollidedWith)
		//	{
		//		auto collidedTeamTag = reg.get<TeamTag>(collided).tag;
		//		if (colliderTeamTag == collidedTeamTag)
		//			continue;

		//		// When an object hits another object it retains the id of the object it collided.
		//		// However, this system in which emplacing/replacing collisions means that collisions are 
		//		// resolved at a Last In First Out order.
		//		reg.emplace_or_replace<CollidedTag>(collider, collided);
		//		reg.emplace_or_replace<CollidedTag>(collided, collider);
		//	}
		//}
	}

private:
	float m_left;
	float m_top;
	float m_width;
	float m_height;
	std::unique_ptr<QuadTree> m_quadTree;
};
