#pragma once

#include <chrono>

struct StatusModEvent
{
	StatusModEvent() = default;

	StatusModEvent(const entt::entity source, const entt::entity receiver, const EffectType type, Effects* effect)
		: sourceID(source)
		, receiverID(receiver)
		, effectType(type)
		, effect(effect)
		, timeStart(std::chrono::steady_clock::now())
		, timeElapsed(0)
	{}

	StatusModEvent(const StatusModEvent& other)
		: sourceID(other.sourceID)
		, receiverID(other.receiverID)
		, effectType(other.effectType)
		, effect(other.effect)
		, timeStart(other.timeStart)
		, timeElapsed(other.timeElapsed)
	{}

	StatusModEvent(StatusModEvent&& other) noexcept
		: sourceID(std::move(other.sourceID))
		, receiverID(std::move(other.receiverID))
		, effectType(std::move(other.effectType))
		, effect(std::move(other.effect))
		, timeStart(std::move(other.timeStart))
		, timeElapsed(std::move(other.timeElapsed))
	{}

	StatusModEvent& operator=(const StatusModEvent& other)
	{
		if (this != &other)
		{
			sourceID = other.sourceID;
			receiverID = other.receiverID;
			effectType = other.effectType;
			effect = other.effect;
			timeStart = other.timeStart;
			timeElapsed = other.timeElapsed;
		}

		return *this;
	}

	entt::entity sourceID;
	entt::entity receiverID;
	EffectType effectType;
	Effects* effect;
	std::chrono::steady_clock::time_point timeStart;
	std::chrono::milliseconds timeElapsed;
};