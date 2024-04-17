#pragma once

#include "ICommand.hpp"
#include "entt\entity\entity.hpp"
#include "SFML\System\Vector2.hpp"
#include <iostream>

class Movement : public ICommand
{
public:
	Movement(const entt::entity id, sf::Vector2f direction);
	void execute(entt::registry& reg);

private:
	const entt::entity entityId;
	sf::Vector2f direction;
};