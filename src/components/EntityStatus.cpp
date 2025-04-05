#include "EntityStatus.hpp"

void EntityStatus::accept(IComponentVisitor* visitor) const
{
	visitor->visit(this);
}
