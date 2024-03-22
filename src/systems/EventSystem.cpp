#include "EventSystem.hpp"

EventSystem::EventSystem()
{
	m_resolveModMap[ModifierType::INSTANT] = std::make_unique<InstantModifier>();
	m_resolveModMap[ModifierType::OVERTIME] = std::make_unique<OvertimeModifier>();
	m_resolveModMap[ModifierType::TIMED] = std::make_unique<TimedModifier>();
}

constexpr std::string_view EventSystem::name()
{
	return "EventSystem";
}

void EventSystem::update(entt::registry& reg, const float& dt, entt::entity ent)
{
	auto view = reg.group<CollisionEvent>();
	for (auto event : view)
	{
		entt::entity source = reg.get<CollisionEvent>(event).sourceID;
		entt::entity receiver = reg.get<CollisionEvent>(event).receiverID;

		// For all of the source entity modifiers, apply effects to receiver
		for (const auto& [modType, effect] : reg.get<EffectsList>(source).effectsList)
		{
			// Get the modifier that will be responsible for applying effects on the receiver's specific status
			IModifier* currentModification = m_resolveModMap[modType].get();

			// Get the receiver status and apply effects
			EntityStatus* receiverStatus = &reg.get<EntityStatus>(receiver);
			currentModification->applyTo(receiverStatus, effect);
		}
	}
}