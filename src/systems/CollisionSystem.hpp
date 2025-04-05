#pragma once

#include "System.hpp"
#include "../common/Logger.hpp"
#include "../common/QuadTree.hpp"
#include "../components/Component.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <memory>

class CollisionSystem : public System
{
public:
	CollisionSystem(entt::registry& reg, const sf::Vector2f& center, const sf::Vector2u& size);

	constexpr std::string_view name() const;
	void update(entt::registry& reg, const float& dt = 0.f, const entt::entity ent = entt::null);
	void remove(entt::registry& reg, const entt::entity ent);
	void draw(sf::RenderTexture& rt);

private:
	void quadTreeUpdate(entt::registry& reg);
	void collisionUpdate(entt::registry& reg);

private:
	std::unique_ptr<QuadTree> m_quadTree;
};