#pragma once

#include "../components/Sprite.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <entt/entity/registry.hpp>
#include <vector>
#include <memory>

constexpr size_t MAX_DEPTH = 4;
constexpr size_t QT_NODE_CAPACITY = 4;

class QuadTree
{
public:
	QuadTree(const sf::FloatRect& rect, const int depth = 0);

	bool insert(entt::registry& reg, const entt::entity ent);
	void subdivide();
	std::vector<entt::entity> queryRange(entt::registry& reg, const sf::FloatRect& range);
	bool remove(entt::registry& reg, const entt::entity ent);
	void clear();
	void draw(sf::RenderTexture& rt);

private:
	const sf::FloatRect m_boundary;
	const int m_depth;
	bool m_divided;
	std::vector<entt::entity> m_nodes;
	std::unique_ptr<QuadTree> m_northWest;
	std::unique_ptr<QuadTree> m_northEast;
	std::unique_ptr<QuadTree> m_southEast;
	std::unique_ptr<QuadTree> m_southWest;
};

