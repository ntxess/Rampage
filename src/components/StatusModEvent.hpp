#pragma once

struct StatusModEvent
{
	StatusModEvent(const entt::entity source, const entt::entity receiver, const EffectType type, Effects* effect)
		: sourceID(source)
		, receiverID(receiver)
		, effectType(type)
		, effect(effect)
		, timeStart(time(NULL))
		, timeElapsed(time_t(0))
	{}

	entt::entity sourceID;
	entt::entity receiverID;
	EffectType effectType;
	Effects* effect;
	time_t timeStart;
	time_t timeElapsed;
};