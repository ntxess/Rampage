#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"
#include "Effects.hpp"
#include <vector>

enum class EffectType
{
	NULLTYPE,
	INSTANT,
	OVERTIME,
	TEMPTIMED,
};

class EffectsList : public IComponent
{
public:
	void accept(IComponentVisitor* visitor, entt::entity entityID) override;

	std::vector<std::pair<EffectType, Effects>> effectsList;
};