#include "QuadTree.hpp"

QuadTree::QuadTree(const sf::FloatRect& rect, const int depth)
	: m_boundary(rect)
	, m_depth(depth)
	, m_divided(false)
{}

bool QuadTree::insert(entt::registry& reg, const entt::entity entity)
{
	// Ignore objects that do not belong in this quad tree
	if (!m_boundary.contains(reg.get<Sprite>(entity).sprite.getPosition()))
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
		if (m_northWest->insert(reg, entity)) return true;
		if (m_northEast->insert(reg, entity)) return true;
		if (m_southEast->insert(reg, entity)) return true;
		if (m_southWest->insert(reg, entity)) return true;
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
	float left = m_boundary.left;
	float top = m_boundary.top;
	float width = m_boundary.width;
	float height = m_boundary.height;

	m_northWest = std::make_unique<QuadTree>(sf::FloatRect(left, top, (width / 2.f), (height / 2.f)), m_depth + 1);
	m_northEast = std::make_unique<QuadTree>(sf::FloatRect((left + (width / 2.f)), top, (width / 2.f), (height / 2.f)), m_depth + 1);
	m_southEast = std::make_unique<QuadTree>(sf::FloatRect((left + (width / 2.f)), (top + (height / 2.f)), (width / 2.f), (height / 2.f)), m_depth + 1);
	m_southWest = std::make_unique<QuadTree>(sf::FloatRect(left, (top + (height / 2.f)), (width / 2.f), (height / 2.f)), m_depth + 1);

	m_divided = true;
}

std::vector<entt::entity> QuadTree::queryRange(entt::registry& reg, const sf::FloatRect& range)
{
	std::vector<entt::entity> entityFound;

	if (!m_boundary.intersects(range))
		return entityFound;

	for (auto entity : m_nodes)
	{
		if (range.intersects(reg.get<Sprite>(entity).getGlobalBounds()))
			entityFound.push_back(entity);
	}

	if (!m_divided)
		return entityFound;

	std::vector<entt::entity> entityVec;

	entityVec = m_northWest->queryRange(reg, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	entityVec = m_northEast->queryRange(reg, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	entityVec = m_southEast->queryRange(reg, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	entityVec = m_southWest->queryRange(reg, range);
	entityFound.insert(entityFound.end(), entityVec.begin(), entityVec.end());

	return entityFound;
}

void QuadTree::remove(entt::registry& reg, const entt::entity entity)
{
	for (size_t i = 0; i < m_nodes.size(); i++)
	{
		if (m_nodes[i] == entity)
		{
			m_nodes.erase(m_nodes.begin() + i);
			return;
		}
	}

	if (!m_divided)
		return;

	m_northWest->remove(reg, entity);
	m_northEast->remove(reg, entity);
	m_southEast->remove(reg, entity);
	m_southWest->remove(reg, entity);
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
		m_southEast->clear();
		m_southWest->clear();
	}

	if (!m_nodes.empty())
		m_nodes.clear();
}