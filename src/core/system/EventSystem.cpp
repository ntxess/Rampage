#include "EventSystem.hpp"

EventSystem::EventSystem(std::chrono::milliseconds watchdogTime)
    : m_eventWatchdogTime(watchdogTime)
{}

constexpr std::string_view EventSystem::name() const
{
    return "EventSystem";
}

void EventSystem::update(entt::registry& reg, const float& dt)
{
    LOG_TRACE(Logger::get()) << "Entering EventSystem::update()";

    const auto& statusModView = reg.view<StatusModEvent>();
    for (const auto& eventID : statusModView)
    {
        const entt::entity receiverID = reg.get<StatusModEvent>(eventID).receiverID;

        if (reg.valid(receiverID) && reg.all_of<EntityStatus>(receiverID))
        {
            const EffectType& effectType = reg.get<StatusModEvent>(eventID).effectType;
            StatusModEvent& statusModEvent = reg.get<StatusModEvent>(eventID);
            EntityStatus& receiverStatus = reg.get<EntityStatus>(statusModEvent.receiverID);

            LOG_TRACE(Logger::get()) << "Processing Event ID [" << static_cast<unsigned int>(eventID) << "]";

            if (apply(effectType, statusModEvent, receiverStatus) == EventStatus::COMPLETE)
            {
                // Event completed processesing
                LOG_INFO(Logger::get()) << "Completed processing of Event ID [" << static_cast<unsigned int>(eventID) << "]";

                reg.destroy(eventID);
            }
        }
        else
        {
            // Destroy event if it receiver is invalid
            LOG_WARNING(Logger::get())
                << "Failed processing of Event ID [" << static_cast<unsigned int>(eventID)
                << "]. Entity [" << static_cast<unsigned int>(receiverID) << "] no longer valid";

            reg.destroy(eventID);
        }
    }

    LOG_TRACE(Logger::get()) << "Leaving EventSystem::update()";
}

EventSystem::EventStatus EventSystem::apply(const EffectType effectType, StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const
{
    switch (effectType)
    {
    case EffectType::NULLTYPE:
        break;

    case EffectType::INSTANT:
        return instantEvent(statusModEvent, receiverStatus);

    case EffectType::OVERTIME:
        return overTimeEvent(statusModEvent, receiverStatus);

    case EffectType::TEMPTIMED:
        return tempTimedEvent(statusModEvent, receiverStatus);

    default:
        break;
    }

    return EventStatus::COMPLETE;
}

EventSystem::EventStatus EventSystem::instantEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const
{
    if (receiverStatus.values.count(statusModEvent.effect->targetStat))
    {
        receiverStatus.values[statusModEvent.effect->targetStat] += statusModEvent.effect->modificationVal;
        LOG_DEBUG(Logger::get())
            << "Applying instant event. Modifying \"" << statusModEvent.effect->targetStat
            << "\" of entity [" << static_cast<unsigned int>(statusModEvent.receiverID)
            << "] by " << statusModEvent.effect->modificationVal
            << ". Current val: " << receiverStatus.values.at(statusModEvent.effect->targetStat);
    }

    return EventStatus::COMPLETE;
}

EventSystem::EventStatus EventSystem::overTimeEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const
{
    const auto currDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - statusModEvent.timeStart);

    // TickRate controls the rate at which the effects applies its modification to the receivers stats
    if (currDuration > statusModEvent.effect->tickRate)
    {
        statusModEvent.timeStart = std::chrono::steady_clock::now();
        statusModEvent.timeElapsed += currDuration;

        if (receiverStatus.values.count(statusModEvent.effect->targetStat))
        {
            receiverStatus.values[statusModEvent.effect->targetStat] += statusModEvent.effect->modificationVal;
            LOG_DEBUG(Logger::get())
                << "Applying over-time event. Modifying \"" << statusModEvent.effect->targetStat
                << "\" of entity [" << static_cast<unsigned int>(statusModEvent.receiverID)
                << "] by " << statusModEvent.effect->modificationVal
                << ". Current val: " << receiverStatus.values.at(statusModEvent.effect->targetStat);
        }
    }

    if (m_eventWatchdogTime < currDuration || statusModEvent.timeElapsed >= statusModEvent.effect->maxDuration)
    {
        if (receiverStatus.values.count(statusModEvent.effect->targetStat))
        {
            LOG_DEBUG(Logger::get())
                << "Completed over-time event. \"" << statusModEvent.effect->targetStat
                << "\" stat of entity [" << static_cast<unsigned int>(statusModEvent.receiverID)
                << "] is " << receiverStatus.values.at(statusModEvent.effect->targetStat)
                << ". Time elapsed: " << statusModEvent.timeElapsed << " >= " << statusModEvent.effect->maxDuration;
        }

        return EventStatus::COMPLETE;
    }

    return EventStatus::INCOMPLETE;
}

EventSystem::EventStatus EventSystem::tempTimedEvent(StatusModEvent& statusModEvent, EntityStatus& receiverStatus) const
{
    const auto currDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - statusModEvent.timeStart);

    // If event some how lasts longer than it should, consider it complete and prep for destroy
    if (m_eventWatchdogTime < currDuration)
        return EventStatus::COMPLETE;

    // Only modify the receiverStatus once in this type of event. We can use the timeElapsed variable as a bool to prevent reaccessing this logic
    if (statusModEvent.timeElapsed == std::chrono::milliseconds(0))
    {
        statusModEvent.timeStart = std::chrono::steady_clock::now();
        statusModEvent.timeElapsed += std::chrono::milliseconds(1); // Disable this logic block with this variable

        if (receiverStatus.values.count(statusModEvent.effect->targetStat))
        {
            receiverStatus.values[statusModEvent.effect->targetStat] += statusModEvent.effect->modificationVal;
            LOG_DEBUG(Logger::get())
                << "Applying temporary-timed event. Modifying \"" << statusModEvent.effect->targetStat
                << "\" of entity [" << static_cast<unsigned int>(statusModEvent.receiverID)
                << "] by " << statusModEvent.effect->modificationVal
                << ". Current val: " << receiverStatus.values.at(statusModEvent.effect->targetStat);
        }
    }

    if (currDuration >= statusModEvent.effect->maxDuration)
    {
        if (receiverStatus.values.count(statusModEvent.effect->targetStat))
        {
            receiverStatus.values[statusModEvent.effect->targetStat] += statusModEvent.effect->modificationVal * -1.f;
            LOG_DEBUG(Logger::get())
                << "Completed temporary-timed event. Restoring \"" << statusModEvent.effect->targetStat
                << "\" of entity [" << static_cast<unsigned int>(statusModEvent.receiverID)
                << "] by " << statusModEvent.effect->modificationVal
                << ". Current val: " << receiverStatus.values.at(statusModEvent.effect->targetStat);
        }

        return EventStatus::COMPLETE;
    }

    return EventStatus::INCOMPLETE;
}