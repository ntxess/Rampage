#pragma once

enum class EventType
{
	BASE,
	INSTANT,
	OVERTIME,
	DELAYED,
	AILMENT
};

struct Event
{
	virtual ~Event() = default;
	virtual EventType type() const = 0;
	virtual void update(/*EntityStatus& entityStatus*/) = 0;
	virtual bool complete() = 0;
};

