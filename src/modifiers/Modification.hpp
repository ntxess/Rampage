#pragma once

enum class ModifierType
{
	BASE,
};

struct Modification
{
	virtual ~Modification() = default;
	virtual ModifierType type() = 0;
	virtual void apply() = 0;
};

