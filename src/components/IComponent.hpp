#pragma once

#include "../common/IComponentVisitor.hpp"
#include <entt/entity/entity.hpp>

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void accept(IComponentVisitor* visitor, entt::entity entityID) = 0;
};;