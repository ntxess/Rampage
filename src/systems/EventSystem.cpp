#include "EventSystem.hpp"

EventSystem::EventSystem()
{}

constexpr std::string_view EventSystem::name()
{
	return "EventSystem";
}

void EventSystem::update(entt::registry& reg, const float& dt, entt::entity ent)
{
	auto statusModGroup = reg.group<StatusModEvent>();
	for (auto event : statusModGroup)
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
	
	auto collisionGroup = reg.group<CollisionEvent>();
	for (auto event : collisionGroup)
	{
		const entt::entity sourceID = reg.get<CollisionEvent>(event).sourceID;
		const entt::entity receiverID = reg.get<CollisionEvent>(event).receiverID;

		// For all of the source entity modifiers, apply effects to receiver
		for (auto& [effectType, effect] : reg.get<EffectsList>(sourceID).effectsList)
		{
			// Get the receiver status and apply effects
			if (reg.all_of<EntityStatus>(receiverID))
			{
				EntityStatus& receiverStatus = reg.get<EntityStatus>(receiverID);
				if (apply(effectType, receiverStatus, effect) == EventStatus::INCOMPLETE)
				{
					Entity statusModEvent(reg);
					statusModEvent.addComponent<StatusModEvent>(sourceID, receiverID, effectType, &effect);
				}
			}

			// Event completed processesing
			reg.destroy(event);
		}
	}
}

EventStatus EventSystem::apply(const EffectType effectType, EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress)
{
	switch (effectType)
	{
	case EffectType::NULLTYPE:
		break;

	case EffectType::INSTANT:
		instantEvent(stats, effect);
		break;

	case EffectType::OVERTIME:
		overTimeEvent(stats, effect, eventProgress);
		break;

	case EffectType::TIMED:
		fixedTimeEvent(stats, effect, eventProgress);
		break;

	default:
		break;
	}

	return EventStatus::FAILED;
}

EventStatus EventSystem::instantEvent(EntityStatus& stats, const Effects& effect)
{
	if (stats.value.count(effect.statusToModify))
		stats.value[effect.statusToModify] += effect.modificationVal;
	return EventStatus::FAILED;
}

EventStatus EventSystem::overTimeEvent(EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress)
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

EventStatus EventSystem::fixedTimeEvent(EntityStatus& stats, const Effects& effect, StatusModEvent* eventProgress)
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