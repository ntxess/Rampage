#include "UpdateEntityEvent.hpp"

UpdateEntityEvent::UpdateEntityEvent()
	: timeStart(std::chrono::steady_clock::now()), maxDuration(0)
{}

UpdateEntityEvent::UpdateEntityEvent(std::chrono::milliseconds duration)
	: timeStart(std::chrono::steady_clock::now()), maxDuration(duration)
{}

void UpdateEntityEvent::accept(IComponentVisitor* visitor) const
{
	visitor->visit(this);
}

bool UpdateEntityEvent::isReady()
{
	const auto currDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeStart);
	if (currDuration <= maxDuration) return false;
	timeStart = std::chrono::steady_clock::now();
	return true;
}
