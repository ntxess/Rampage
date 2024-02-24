#pragma once

enum class ModifierType
{
	BASE,
	INSTANT,
	OVERTIME,
	DELAYED,
	AILMENT
};

struct Modification
{
	virtual ~Modification() = default;
	virtual ModifierType type() const = 0;
	virtual void update(/*EntityStatus& entityStatus*/) = 0;
	virtual bool complete() = 0;
};

