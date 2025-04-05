#include "PlayerInput.hpp"

void PlayerInput::accept(IComponentVisitor* visitor) const
{
	visitor->visit(this);
}

void PlayerInput::processInput(entt::registry& reg)
{
	for (const auto& [key, action] : input)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			// Entity registry is passes in for convinience when creating more complex actions
			action->execute(reg);
		}
	}
}
