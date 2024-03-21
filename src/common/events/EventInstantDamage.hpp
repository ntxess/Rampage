#pragma once

#include "Event.hpp"
#include "EventHandler.hpp"

struct EventInstantDamage : public Event
{
	EventType type() const
	{
		return EventType::INSTANT;
	}

	void update(/*EntityStatus& entityStatus*/)
	{
		//entityStatus -= 
	}
 
private:
	bool EventStatus = false;
};

struct EventInstantDamageHandler : public EventHandler
{
	void onEvent(Event* evnt)
	{
		//evnt->entityToBeDestroyed
		//std::cout << "Destruction Event Triggered\n";
	}
};