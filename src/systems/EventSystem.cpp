#include "EventSystem.hpp"

EventSystem::EventSystem()
{}

constexpr std::string_view EventSystem::name()
{
	return "EventSystem";
}

void EventSystem::update(entt::registry& reg, const float& dt, const entt::entity ent)
{
	// For events that require further processing (aka non-INSTANT type events)
	auto statusModView = reg.view<StatusModEvent>();
	for (auto event : statusModView)
	{
		const entt::entity sourceID = reg.get<StatusModEvent>(event).sourceID;
		const entt::entity receiverID = reg.get<StatusModEvent>(event).receiverID;
		const EffectType effectType = reg.get<StatusModEvent>(event).effectType;
		const Effects& effect = *reg.get<StatusModEvent>(event).effect;

		if (reg.all_of<EntityStatus>(receiverID))
		{
			EntityStatus& receiverStatus = reg.get<EntityStatus>(receiverID);
			StatusModEvent* statusModEvent = &reg.get<StatusModEvent>(event);
			if (apply(effectType, receiverStatus, effect, statusModEvent) == EventStatus::COMPLETE)
			{
				// Event completed processesing
				reg.destroy(event);
			}
		}
		else
		{
			// Destroy event if it becomes stale
			reg.destroy(event);
		}
	}
	
	auto collisionView = reg.view<CollisionEvent>();
	for (auto receiverID : collisionView)
	{
		const entt::entity sourceID = reg.get<CollisionEvent>(receiverID).sourceID;

		// For all of the source entity modifiers, apply effects to receiver
		if (reg.all_of<EffectsList>(sourceID))
		{
			for (auto& [effectType, effect] : reg.get<EffectsList>(sourceID).effectsList)
			{
				// Get the receiver status and apply effects
				if (reg.all_of<EntityStatus>(receiverID))
				{
					EntityStatus& receiverStatus = reg.get<EntityStatus>(receiverID);
					if (apply(effectType, receiverStatus, effect) == EventStatus::INCOMPLETE)
					{
						entt::entity statusModEvent = reg.create();
						reg.emplace_or_replace<StatusModEvent>(statusModEvent, sourceID, receiverID, effectType, &effect);
					}
				}

				// Event completed processesing
				reg.remove<CollisionEvent>(receiverID);
			}
		}
	}
}

EventSystem::EventStatus EventSystem::apply(const EffectType effectType, EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress)
{
	switch (effectType)
	{
	case EffectType::NULLTYPE:
		break;

	case EffectType::INSTANT:
		return instantEvent(stats, effect);

	case EffectType::OVERTIME:
		return overTimeEvent(stats, effect, eventProgress);

	case EffectType::TIMED:
		return fixedTimeEvent(stats, effect, eventProgress);

	default:
		break;
	}

	return EventStatus::FAILED;
}

EventSystem::EventStatus EventSystem::instantEvent(EntityStatus& stats, const Effects& effect)
{
	if (stats.value.count(effect.statusToModify))
		stats.value[effect.statusToModify] += effect.modificationVal;

	return EventStatus::FAILED;
}

EventSystem::EventStatus EventSystem::overTimeEvent(EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress)
{
	if (eventProgress)
	{
		if (stats.value.count(effect.statusToModify))
			stats.value[effect.statusToModify] += effect.modificationVal;
	}
	else
	{
		if (stats.value.count(effect.statusToModify))
			stats.value[effect.statusToModify] += effect.modificationVal;
	}
	return EventStatus::FAILED;
}

EventSystem::EventStatus EventSystem::fixedTimeEvent(EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress)
{
	if (eventProgress)
	{
		if (stats.value.count(effect.statusToModify))
			stats.value[effect.statusToModify] += effect.modificationVal;
	}
	else
	{
		if (stats.value.count(effect.statusToModify))
			stats.value[effect.statusToModify] += effect.modificationVal;
	}
	return EventStatus::FAILED;
}