#pragma once

#include "../common/IComponentVisitor.hpp"

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void accept(IComponentVisitor* visitor) const = 0;
};;