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
	virtual ModifierType type() = 0;
	virtual void apply(std::any& status) = 0;
};