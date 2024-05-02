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
	CollisionSystem(entt::registry& reg, const sf::Vector2f& center, const sf::Vector2u& size);

	constexpr std::string_view name();
	void update(entt::registry& reg, const float& dt = 0.f, entt::entity ent = entt::null);
	void render(sf::RenderWindow& rw);
	void remove(entt::registry& reg, const entt::entity ent);

private:
	void quadTreeUpdate(entt::registry& reg);
	void collisionUpdate(entt::registry& reg);

private:
	std::unique_ptr<QuadTree> m_quadTree;
};
