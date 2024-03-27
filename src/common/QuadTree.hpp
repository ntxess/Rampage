#pragma once

#include "../components/Sprite.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <entt/entity/registry.hpp>
#include <vector>
#include <memory>

constexpr size_t MAX_DEPTH = 6;
constexpr size_t QT_NODE_CAPACITY = 4;

class QuadTree
{
public:
	QuadTree(const sf::FloatRect& rect, const size_t depth = 0);

	bool insert(const entt::registry& registry, const entt::entity entity);
	void subdivide();
	std::vector<entt::entity> queryRange(const entt::registry& registry, const sf::FloatRect& range);
	void remove(const entt::registry& registry, const entt::entity entity);
	void clear();
	void render(sf::RenderWindow& rw);
	sf::ConvexShape outlineBoundary(size_t pointCount, sf::ConvexShape quadBoundary);

private:
	sf::FloatRect m_boundary;
	size_t m_depth;
	bool m_divided;
	std::vector<entt::entity> m_nodes;
	std::unique_ptr<QuadTree> m_northWest;
	std::unique_ptr<QuadTree> m_northEast;
	std::unique_ptr<QuadTree> m_southWest;
	std::unique_ptr<QuadTree> m_southEast;
	sf::ConvexShape m_rectangle;
};

