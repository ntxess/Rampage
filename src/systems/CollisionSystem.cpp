#include "CollisionSystem.hpp"

CollisionSystem::CollisionSystem(const sf::Vector2f& center, const sf::Vector2f& size)
	: m_left(0)
	, m_top(0)
	, m_width(size.x)
	, m_height(size.y)
	, m_quadTree(nullptr)
	, m_boundBox()
{}

constexpr std::string_view CollisionSystem::name()
{
	return "CollisionSystem";
}

void CollisionSystem::update(entt::registry& reg, const float& dt, entt::entity ent)
{
	quadTreeUpdate(reg);  // Rebuild the quadtree for querying collisions
	collisionUpdate(reg); // Find and mark all collided entity with a tag

	sf::ConvexShape boundBox;
	m_boundBox = m_quadTree->outlineBoundary(4, boundBox);
}

void CollisionSystem::quadTreeUpdate(entt::registry& reg)
{
	// Create/Update Quadtree with new entity positions
	m_quadTree = std::make_unique<QuadTree>(sf::FloatRect(m_left, m_top, m_width, m_height));

	// auto view = reg.view<Hitbox>(); // For future use when we use HITBOX
	auto view = reg.view<Sprite>();

	for (auto entity : view)
		m_quadTree->insert(reg, entity);
}

void CollisionSystem::collisionUpdate(entt::registry& reg)
{
	// auto view = reg.view<Hitbox>(); // For future use when we use HITBOX
	auto view = reg.view<Sprite>();
	for (auto source : view)
	{
		// Query all neighboring entity for collision
		// sf::FloatRect sourceHitbox = reg.get<Hitbox>(source).getBounds(); // For future use when we use HITBOX
		sf::FloatRect sourceHitbox = reg.get<Sprite>(source).sprite.getGlobalBounds();
		std::vector<entt::entity> receiverList = m_quadTree->queryRange(reg, sourceHitbox);
		Team sourceTeamTag = reg.get<TeamTag>(source).tag;

		for (auto receiver : receiverList)
		{
			// Check if entity is on the same team
			Team receiverTeamTag = reg.get<TeamTag>(receiver).tag;
			if (sourceTeamTag == receiverTeamTag)
				continue;

			// If not on the same team, create new CollisionEvent for later processing
			entt::entity collisionEvent = reg.create();
			reg.emplace<CollisionEvent>(collisionEvent, source, receiver);
		}
	}
}

void CollisionSystem::render(sf::RenderWindow& rw)
{
	//rw.draw(m_boundBox);
	//m_boundBox = sf::ConvexShape();

	m_quadTree->render(rw);
}
