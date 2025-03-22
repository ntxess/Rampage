#include "CollisionSystem.hpp"

CollisionSystem::CollisionSystem(entt::registry& reg, const sf::Vector2f& center, const sf::Vector2u& size)
	: m_quadTree(std::make_unique<QuadTree>(sf::FloatRect(center.x, center.y, static_cast<float>(size.x), static_cast<float>(size.y))))
{
	const auto& view = reg.view<Sprite>();
	for (const auto& entity : view)
		m_quadTree->insert(reg, entity);
}

constexpr std::string_view CollisionSystem::name() const
{
	return "CollisionSystem";
}

void CollisionSystem::update(entt::registry& reg, const float& dt, const entt::entity ent)
{
	LOG_TRACE(Logger::get()) << "Entering CollisionSystem::update()";

	quadTreeUpdate(reg);  // Rebuild the quadtree for querying collisions
	collisionUpdate(reg); // Find and mark all collided entity with a tag

	LOG_TRACE(Logger::get()) << "Leaving CollisionSystem::update()";
}

void CollisionSystem::quadTreeUpdate(entt::registry& reg)
{
	const auto& view = reg.view<Sprite, UpdateEntityEvent>();
	for (const auto& entity : view)
	{
		m_quadTree->remove(reg, entity);
		m_quadTree->insert(reg, entity);
	}
}

void CollisionSystem::collisionUpdate(entt::registry& reg)
{
	// Event-driven collision update
	const auto& view = reg.view<Sprite, UpdateEntityEvent>();
	for (const auto& sourceID : view)
	{
		// Query all neighboring entity for collision
		const sf::FloatRect& sourceHitbox = reg.get<Sprite>(sourceID).getGlobalBounds();
		std::vector<entt::entity> receiverList = m_quadTree->queryRange(reg, sourceHitbox);
		const Team& sourceTeamTag = reg.get<TeamTag>(sourceID).tag;

		for (const auto& receiverID : receiverList)
		{
			if (!reg.get<UpdateEntityEvent>(sourceID).isReady()) continue;

			// Check if entity is on the same team
			Team receiverTeamTag = reg.get<TeamTag>(receiverID).tag;
			if (sourceTeamTag == receiverTeamTag)
				continue;

			// For all of the source entity modifiers, apply effects to receiver
			if (reg.valid(sourceID) && reg.all_of<EffectsList>(sourceID))
			{
				for (auto& [effectType, effect] : reg.get<EffectsList>(sourceID).effectsList)
				{
					// Get the receiver status and apply effects
					if (reg.valid(receiverID) && reg.all_of<EntityStatus>(receiverID))
					{
						StatusModEvent statusModEvent(sourceID, receiverID, effectType, &effect);
						entt::entity statusModEventID = reg.create();

						reg.emplace_or_replace<StatusModEvent>(statusModEventID, statusModEvent);

						LOG_INFO(Logger::get())
							<< "Collision Event ID [" << static_cast<unsigned int>(statusModEventID)
							<< "]: Entity " << static_cast<unsigned int>(sourceID)
							<< " collided with entity " << static_cast<unsigned int>(receiverID);
					}
				}
			}
		}

		// QuadTree update completed, delete dirty flags
		reg.remove<UpdateEntityEvent>(sourceID);
	}

	// Polling collision update
	const auto& pollingView = reg.view<Sprite, UpdateEntityPolling>();
	for (const auto& sourceID : pollingView)
	{
		// Query all neighboring entity for collision
		const sf::FloatRect& sourceHitbox = reg.get<Sprite>(sourceID).getGlobalBounds();
		std::vector<entt::entity> receiverList = m_quadTree->queryRange(reg, sourceHitbox);
		const Team& sourceTeamTag = reg.get<TeamTag>(sourceID).tag;

		for (const auto& receiverID : receiverList)
		{
			if (!reg.get<UpdateEntityPolling>(sourceID).isReady()) continue;

			// Check if entity is on the same team
			Team receiverTeamTag = reg.get<TeamTag>(receiverID).tag;
			if (sourceTeamTag == receiverTeamTag)
				continue;

			// For all of the source entity modifiers, apply effects to receiver
			if (reg.valid(sourceID) && reg.all_of<EffectsList>(sourceID))
			{
				for (auto& [effectType, effect] : reg.get<EffectsList>(sourceID).effectsList)
				{
					// Get the receiver status and apply effects
					if (reg.valid(receiverID) && reg.all_of<EntityStatus>(receiverID))
					{
						StatusModEvent statusModEvent(sourceID, receiverID, effectType, &effect);
						entt::entity statusModEventID = reg.create();

						reg.emplace_or_replace<StatusModEvent>(statusModEventID, statusModEvent);

						LOG_INFO(Logger::get())
							<< "Collision Event ID [" << static_cast<unsigned int>(statusModEventID)
							<< "]: Entity [" << static_cast<unsigned int>(sourceID)
							<< "] collided with entity [" << static_cast<unsigned int>(receiverID) << "]";
					}
				}
			}
		}
	}
}

void CollisionSystem::remove(entt::registry& reg, const entt::entity ent)
{
	m_quadTree->remove(reg, ent);
}
