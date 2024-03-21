#pragma once

#include <any>

enum class ModifierType
{
	NULLTYPE,
	INSTANT,
	OVERTIME,
	EFFECT,
	DEBUFF,
};

struct Modifier
{
	virtual ~Modifier() = default;
	virtual ModifierType type() const = 0;
	virtual void applyTo(EntityStatus* status, int val) = 0;
};