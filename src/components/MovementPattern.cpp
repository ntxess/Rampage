#include "MovementPattern.hpp"

void MovementPattern::accept(IComponentVisitor* visitor) const
{
	visitor->visit(this);
}
