#pragma once

#include "Event.hpp"

class EventDispatcher
{
public:
	EventDispatcher() {}
	~EventDispatcher() {}

	template<typename T, typename U>
	void dispatch(Event* evnt, T* instance, void (T::* callbackEvent)(U*));
};

template<typename T, typename U>
void EventDispatcher::dispatch(Event* evnt, T* instance, void (T::* callbackEvent)(U*))
{
	(instance->*callbackEvent)(static_cast<U*>(evnt));
}