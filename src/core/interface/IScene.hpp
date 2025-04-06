#pragma once

#include "entt/entity/registry.hpp"
#include "SFML/Window/Event.hpp"

class ISceneVisitor;
struct GlobalData;

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
    virtual void addData(GlobalData* data) = 0;
    virtual void accept(ISceneVisitor* visitor) = 0;
    virtual entt::registry& getRegistry() = 0;
};