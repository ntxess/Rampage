#pragma once

#include "Effects.hpp"
#include <any>

enum class ModifierType
{
	NULLTYPE,
	INSTANT,
	OVERTIME,
	TIMED,
};

struct IModifier
{
	virtual ~IModifier() = default;
	virtual ModifierType type() const = 0;
	virtual void applyTo(EntityStatus* status, const Effects& effect) = 0;
};