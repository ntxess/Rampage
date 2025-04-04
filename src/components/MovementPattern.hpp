#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"

class MovementPattern : public IComponent
{
public:
	void accept(IComponentVisitor* visitor, entt::entity entityID) override;
};