#pragma once

#include <any>

enum class ModifierType
{
	BASE,
};

struct Modifier
{
	virtual ~Modifier() = default;
	virtual ModifierType type() = 0;
	virtual void apply(std::any& status) = 0;
};