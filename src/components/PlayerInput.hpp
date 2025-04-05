#pragma once 

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"
#include "../common/commands/ICommand.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <entt/entity/registry.hpp>
#include <unordered_map>

class PlayerInput : public IComponent
{
public:
	void accept(IComponentVisitor* visitor) const override;

	void processInput(entt::registry& reg);

	std::unordered_map<sf::Keyboard::Key, ICommand*> input;
};