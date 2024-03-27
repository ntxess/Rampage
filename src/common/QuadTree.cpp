#include "QuadTree.hpp"

QuadTree::QuadTree(const sf::FloatRect& rect, const size_t depth)
	: m_boundary(rect)
	, m_depth(depth)
	, m_divided(false)
{
	// Init debug rectangle
	// m_rectangle.setPosition(m_boundary.left, m_boundary.top);
	// m_rectangle.setSize(sf::Vector2f(m_boundary.width, m_boundary.height));
	// m_rectangle.setOutlineThickness(1.0f);
	// m_rectangle.setFillColor(sf::Color::Transparent);
	// m_rectangle.setOutlineColor(sf::Color(0, 150, 100));
	m_rectangle.setPosition(m_boundary.left, m_boundary.top);
	m_rectangle.setPointCount(4);
	m_rectangle.setPoint(0, sf::Vector2f(0, 0)); // Top-Left
	m_rectangle.setPoint(1, sf::Vector2f(0, m_boundary.height)); // Bot-Left
	m_rectangle.setPoint(2, sf::Vector2f(m_boundary.width, m_boundary.height)); // Bot-Right
	m_rectangle.setPoint(3, sf::Vector2f(m_boundary.width, 0)); // Top-Right
	m_rectangle.setOutlineThickness(1.0f);
	m_rectangle.setFillColor(sf::Color::Transparent);
	m_rectangle.setOutlineColor(sf::Color(0, 150, 100));

}

bool QuadTree::insert(const entt::registry& registry, const entt::entity entity)
{
	// Ignore objects that do not belong in this quad tree
	if (!m_boundary.contains(registry.get<Sprite>(entity).sprite.getPosition()))
		return false;

	// If there is space in this quad tree and if doesn't have subdivisions, add the object here
	if (m_nodes.size() <= QT_NODE_CAPACITY)
	{
		m_nodes.push_back(entity);
		return true;
	}

	// Otherwise, subdivide and then add the point to whichever node will accept it
	if (m_depth + 1 < MAX_DEPTH)
	{
		if (!m_divided)
			subdivide();

		// We have to add the points/data contained into this quad array to the new quads if we only want
		// the last node to hold the data
		if (m_northWest->insert(registry, entity)) return true;
		if (m_northEast->insert(registry, entity)) return true;
		if (m_southWest->insert(registry, entity)) return true;
		if (m_southEast->insert(registry, entity)) return true;
	}
	else
	{
		m_nodes.push_back(entity);
		return true;
	}

	// Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
	return false;
}

void QuadTree::subdivide()
{
	float x = m_boundary.left;
	float y = m_boundary.top;
	float width = m_boundary.width;
	float height = m_boundary.height;

	m_northWest = std::make_unique<QuadTree>(sf::FloatRect(x, y, width / 2.0f, height / 2.0f), m_depth + 1);
	m_northEast = std::make_unique<QuadTree>(sf::FloatRect(x + width / 2.0f, y, width / 2.0f, height / 2.0f), m_depth + 1);
	m_southWest = std::make_unique<QuadTree>(sf::FloatRect(x, y + height / 2.0f, width / 2.0f, height / 2.0f), m_depth + 1);
	m_southEast = std::make_unique<QuadTree>(sf::FloatRect(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f), m_depth + 1);

	m_divided = true;
}

std::vector<entt::entity> QuadTree::queryRange(const entt::registry& registry, const sf::FloatRect& range)
{
	std::vector<entt::entity> entityFound;

	if (!m_boundary.intersects(range))
		return entityFound;

	for (auto entity : m_nodes)
	{
		if (range.intersects(registry.get<Sprite>(entity).sprite.getGlobalBounds()))
			entityFound.push_back(entity);
	}

	if (!m_divided)
		return entityFound;

	std::vector<entt::entity> entityVec;

	entityVec = m_northWest->queryRange(registry, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	entityVec = m_northEast->queryRange(registry, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	entityVec = m_southWest->queryRange(registry, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	entityVec = m_southEast->queryRange(registry, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	return entityFound;
}

void QuadTree::remove(const entt::registry& registry, const entt::entity entity)
{
	for (size_t i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i] == entity)
		{
			m_nodes[i] = entt::null;
			return;
		}
	}

	if (!m_divided)
		return;

	m_northWest->remove(registry, entity);
	m_northEast->remove(registry, entity);
	m_southWest->remove(registry, entity);
	m_southEast->remove(registry, entity);
}

void QuadTree::clear()
{
	if (!m_divided)
	{
		m_nodes.clear();
		return;
	}
	else
	{
		m_northWest->clear();
		m_northEast->clear();
		m_southWest->clear();
		m_southEast->clear();
	}

	if (!m_nodes.empty())
		m_nodes.clear();
}

void QuadTree::render(sf::RenderWindow& rw)
{
	rw.draw(m_rectangle);

	if (m_divided)
	{
		m_northWest->render(rw);
		m_northEast->render(rw);
		m_southWest->render(rw);
		m_southEast->render(rw);
	}
}

sf::ConvexShape QuadTree::outlineBoundary(size_t pointCount, sf::ConvexShape quadBoundary)
{
	sf::ConvexShape boundary = quadBoundary;
	size_t boundaryPoints = pointCount;

	boundary.setPosition(m_boundary.left, m_boundary.top);
	boundary.setPointCount(boundaryPoints);
	boundary.setPoint(0, sf::Vector2f(0, 0)); // Top-Left
	boundary.setPoint(1, sf::Vector2f(0, m_boundary.height)); // Bot-Left
	boundary.setPoint(2, sf::Vector2f(m_boundary.width, m_boundary.height)); // Bot-Right
	boundary.setPoint(3, sf::Vector2f(m_boundary.width, 0)); // Top-Right
	boundary.setOutlineThickness(1000.0f);
	boundary.setFillColor(sf::Color::Transparent);
	boundary.setOutlineColor(sf::Color(0, 150, 100));

	if (m_divided)
	{
		pointCount += 4;
		boundary = m_northWest->outlineBoundary(boundaryPoints, boundary);
		pointCount += 4;
		boundary = m_northEast->outlineBoundary(boundaryPoints, boundary);
		pointCount += 4;
		boundary = m_southWest->outlineBoundary(boundaryPoints, boundary);
		pointCount += 4;
		boundary = m_southEast->outlineBoundary(boundaryPoints, boundary);
	}

	return boundary;	
}
