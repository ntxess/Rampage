#pragma once

#include "Event.hpp"

class EventHandler
{
public:
	virtual ~EventHandler() {}
	virtual void onEvent(Event* evnt) = 0;
};