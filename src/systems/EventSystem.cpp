#include "EventSystem.hpp"

EventSystem::EventSystem(time_t delayTime, time_t watchdogTime)
	: m_delayTime(delayTime)
	, m_eventWatchdogTime(watchdogTime)
{}

constexpr std::string_view EventSystem::name()
{
	return "EventSystem";
}

void EventSystem::update(entt::registry& reg, const float& dt, const entt::entity ent)
{
	// For events that require further processing (aka non-INSTANT type events)
	auto statusModView = reg.view<StatusModEvent>();
	for (auto eventID : statusModView)
	{
		const entt::entity receiverID = reg.get<StatusModEvent>(eventID).receiverID;

		if (reg.valid(receiverID) && reg.all_of<EntityStatus>(receiverID))
		{
			const EffectType& effectType = reg.get<StatusModEvent>(eventID).effectType;
			StatusModEvent& statusModEvent = reg.get<StatusModEvent>(eventID);

			if (apply(reg, effectType, statusModEvent) == EventStatus::COMPLETE)
			{
				// Event completed processesing
				reg.destroy(eventID);
			}
		}
		else
		{
			// Destroy event if it receiver is invalid
			reg.destroy(eventID);
		}
	}
	
	auto collisionView = reg.view<CollisionEvent>();
	for (auto receiverID : collisionView)
	{
		const entt::entity sourceID = reg.get<CollisionEvent>(receiverID).sourceID;

		// For all of the source entity modifiers, apply effects to receiver
		if (reg.valid(sourceID) && reg.all_of<EffectsList>(sourceID))
		{
			for (auto& [effectType, effect] : reg.get<EffectsList>(sourceID).effectsList)
			{
				// Get the receiver status and apply effects
				if (reg.valid(receiverID) && reg.all_of<EntityStatus>(receiverID))
				{
					StatusModEvent statusModEvent(sourceID, receiverID, effectType, &effect);

					if (apply(reg, effectType, statusModEvent) == EventStatus::INCOMPLETE)
					{
						entt::entity statusModEventID = reg.create();
						reg.emplace_or_replace<StatusModEvent>(statusModEventID, statusModEvent);
					}
				}

				// Event completed processesing
				reg.remove<CollisionEvent>(receiverID);
			}
		}
	}
}

EventSystem::EventStatus EventSystem::apply(entt::registry& reg, const EffectType effectType, StatusModEvent& statusModEvent)
{
	EntityStatus& receiverStatus = reg.get<EntityStatus>(statusModEvent.receiverID);

	switch (effectType)
	{
	case EffectType::NULLTYPE:
		break;

	case EffectType::INSTANT:
		return instantEvent(receiverStatus, statusModEvent);

	case EffectType::OVERTIME:
		return overTimeEvent(receiverStatus, statusModEvent);

	case EffectType::TIMED:
		return fixedTimeEvent(receiverStatus, statusModEvent);

	default:
		break;
	}

	return EventStatus::COMPLETE;
}

EventSystem::EventStatus EventSystem::instantEvent(EntityStatus& receiverStatus, StatusModEvent& statusModEvent)
{
	if (receiverStatus.value.count(statusModEvent.effect->statusToModify))
		receiverStatus.value[statusModEvent.effect->statusToModify] += statusModEvent.effect->modificationVal;

	return EventStatus::COMPLETE;
}

EventSystem::EventStatus EventSystem::overTimeEvent(EntityStatus& receiverStatus, StatusModEvent& statusModEvent)
{
	double currentTime = difftime(time(NULL), statusModEvent.timeStart);

	if (m_eventWatchdogTime < currentTime || statusModEvent.timeElapsed > statusModEvent.effect->duration)
		return EventStatus::COMPLETE;

	if (currentTime > m_delayTime)
	{
		statusModEvent.timeStart = time(NULL);
		statusModEvent.timeElapsed += static_cast<time_t>(currentTime);

		if (receiverStatus.value.count(statusModEvent.effect->statusToModify))
			receiverStatus.value[statusModEvent.effect->statusToModify] += statusModEvent.effect->modificationVal;

		return EventStatus::INCOMPLETE;
	}

	return EventStatus::INCOMPLETE;
}

EventSystem::EventStatus EventSystem::fixedTimeEvent(EntityStatus& receiverStatus, StatusModEvent& statusModEvent)
{
	//if (eventProgress)
	//{
	//	if (receiverStatus.value.count(effect.statusToModify))
	//		receiverStatus.value[effect.statusToModify] += effect.modificationVal;
	//}
	//else
	//{
	//	if (receiverStatus.value.count(effect.statusToModify))
	//		receiverStatus.value[effect.statusToModify] += effect.modificationVal;
	//}
	return EventStatus::COMPLETE;
}