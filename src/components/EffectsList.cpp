#include "EffectsList.hpp"

void EffectsList::accept(IComponentVisitor* visitor) const
{
	visitor->visit(this);
}
