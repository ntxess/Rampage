#pragma once

struct StatusModEvent
{
	StatusModEvent(const entt::entity source, const entt::entity receiver, const EffectType type, const Effects* effect)
		: sourceID(source)
		, receiverID(receiver)
		, effectType(type)
		, effect(effect)
		, timeStart(time(NULL))
		, timeElapsed(time_t(0))
	{}

	const entt::entity sourceID;
	const entt::entity receiverID;
	const EffectType effectType;
	const Effects* effect;
	const time_t timeStart;
	time_t timeElapsed;
};