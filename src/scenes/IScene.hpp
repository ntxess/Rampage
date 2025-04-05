#pragma once

#include "../common/ISceneVisitor.hpp"
#include <SFML/Window/Event.hpp>
#include <entt/entity/registry.hpp>

class IScene
{
public:
	virtual ~IScene() = default;
	virtual void init() = 0;
	virtual void processEvent(const sf::Event& event) = 0;
	virtual void processInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void accept(ISceneVisitor* visitor) = 0;
	virtual entt::registry& getRegistry() = 0;
};