#include "Movement.hpp"

Movement::Movement(const entt::entity id, sf::Vector2f direction)
	: entityId(id)
	, direction(direction)
{}

void Movement::execute(entt::registry& reg)
{
	std::cout << "Pressed key" << std::endl;
}

