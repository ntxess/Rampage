#pragma once

#include "IComponent.hpp"
#include "EffectsList.hpp"
#include "../common/IComponentVisitor.hpp"
#include <entt/entity/entity.hpp>
#include <chrono>

class StatusModEvent : public IComponent
{
public:
	StatusModEvent();
	StatusModEvent(const entt::entity source, const entt::entity receiver, const EffectType type, Effects* effect);
	StatusModEvent(const StatusModEvent& other);
	StatusModEvent(StatusModEvent&& other) noexcept;
	StatusModEvent& operator=(const StatusModEvent& other);

	void accept(IComponentVisitor* visitor) const override;

	entt::entity sourceID;
	entt::entity receiverID;
	EffectType effectType;
	Effects* effect;
	std::chrono::steady_clock::time_point timeStart;
	std::chrono::milliseconds timeElapsed;
};