#include "CollisionSystem.hpp"

CollisionSystem::CollisionSystem(entt::registry& reg, const sf::Vector2f& center, const sf::Vector2u& size)
	: m_quadTree(std::make_unique<QuadTree>(sf::FloatRect(center.x, center.y, size.x, size.y)))
{
	auto view = reg.view<Sprite>();

	for (auto entity : view)
		m_quadTree->insert(reg, entity);
}

constexpr std::string_view CollisionSystem::name()
{
	return "CollisionSystem";
}

void CollisionSystem::update(entt::registry& reg, const float& dt, entt::entity ent)
{
	//auto start = std::chrono::high_resolution_clock::now();

	quadTreeUpdate(reg);  // Rebuild the quadtree for querying collisions

	//auto stop = std::chrono::high_resolution_clock::now();

	//auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
	//std::cout << RED << "[Update] | QuadTree Update: " << duration << "ns\n" << RESET;

	//start = std::chrono::high_resolution_clock::now();

	collisionUpdate(reg); // Find and mark all collided entity with a tag

	//stop = std::chrono::high_resolution_clock::now();
	//duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
	//std::cout << BLUE << "[Update] | Collision Update: " << duration << "ns\n" << RESET;
}

void CollisionSystem::quadTreeUpdate(entt::registry& reg)
{
	auto view = reg.view<Sprite, DirtyMovement>();
	for (auto entity : view)
	{
		m_quadTree->remove(reg, entity);
		m_quadTree->insert(reg, entity);
	}
}

void CollisionSystem::collisionUpdate(entt::registry& reg)
{
	// auto view = reg.view<Hitbox>(); // For future use when we use HITBOX
	auto view = reg.view<Sprite, DirtyMovement>();
	for (auto source : view)
	{
		// Query all neighboring entity for collision
		// sf::FloatRect sourceHitbox = reg.get<Hitbox>(source).getBounds(); // For future use when we use HITBOX
		const sf::FloatRect& sourceHitbox = reg.get<Sprite>(source).getGlobalBounds();
		std::vector<entt::entity> receiverList = m_quadTree->queryRange(reg, sourceHitbox);
		const Team& sourceTeamTag = reg.get<TeamTag>(source).tag;

		for (auto receiver : receiverList)
		{
			// Check if entity is on the same team
			Team receiverTeamTag = reg.get<TeamTag>(receiver).tag;
			if (sourceTeamTag == receiverTeamTag)
				continue;

			// If not on the same team, emplace new CollisionEvent on reciever for later processing
			reg.emplace<CollisionEvent>(receiver, source);
		}
		reg.remove<DirtyMovement>(source);
	}
}

void CollisionSystem::render(sf::RenderWindow& rw)
{
}

void CollisionSystem::remove(entt::registry& reg, const entt::entity ent)
{
	m_quadTree->remove(reg, ent);
}
