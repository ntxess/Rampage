#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"
#include <chrono>

class UpdateEntityEvent : public IComponent
{
public:
	UpdateEntityEvent();
	UpdateEntityEvent(std::chrono::milliseconds duration);

	void accept(IComponentVisitor* visitor, entt::entity entityID) override;

	bool isReady();

	std::chrono::steady_clock::time_point timeStart;
	std::chrono::milliseconds maxDuration;
};
