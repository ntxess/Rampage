#pragma once

#include "Event.hpp"
#include "EventHandler.hpp"

struct EventOvertimeDamage : public Event
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

struct EventOvertimeDamageHandler : public EventHandler
{
	void onEvent(Event* evnt)
	{
		//evnt->entityToBeDestroyed
		//std::cout << "Destruction Event Triggered\n";
	}
};