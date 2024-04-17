#pragma once 

#include "../common/commands/ICommand.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <memory>
#include <unordered_map>

struct PlayerInput
{
	std::unordered_map<sf::Keyboard::Key, ICommand*> input;
};