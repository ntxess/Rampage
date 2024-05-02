#pragma once

#include "System.hpp"
#include "../common/Entity.hpp"
#include "../common/QuadTree.hpp"
#include "../components/Component.hpp"
#include <memory>
#include <mutex>

// Debug
#include <iostream>

class CollisionSystem : public System
{
public:
	CollisionSystem(const sf::Vector2f& center, const sf::Vector2u& size);

	constexpr std::string_view name();
	void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null);
	void render(sf::RenderWindow& rw);

private:
	void quadTreeUpdate(entt::registry& reg);
	void collisionUpdate(entt::registry& reg);

private:
	float m_left;
	float m_top;
	unsigned int m_width;
	unsigned int m_height;
	std::unique_ptr<QuadTree> m_quadTree;
};
