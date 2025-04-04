#pragma once 

#include "../common/commands/ICommand.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <entt/entity/entity.hpp>
#include <unordered_map>

struct PlayerInput
{
	void processInput(entt::registry& reg)
	{
		for (auto& [key, action] : input)
		{
			if (sf::Keyboard::isKeyPressed(key))
			{
				// entity registry is passes in for convinience when creating more complex actions
				action->execute(reg); 
			}
		}
	}

	std::unordered_map<sf::Keyboard::Key, ICommand*> input;
};