#pragma once 

#include "../core/ApplicationContext.hpp"
#include "../core/Commands.hpp"
#include "../core/Components.hpp"
#include "../core/Managers.hpp"
#include "../core/Systems.hpp"
#include "../core/interface/IScene.hpp"
#include "../core/interface/ISceneVisitor.hpp"
#include "../core/util/DataStore.hpp"
#include "../core/util/Logger.hpp"
#include "../scene/Scenes.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <random>

class Sandbox final : public IScene
{
public:
    Sandbox();
    Sandbox(ApplicationContext* sysData);
    ~Sandbox();

    void init() override final;
    void processEvent(const sf::Event& event) override final;
    void processInput() override final;
    void update() override final;
    void render() override final;
    void pause() override final;
    void resume() override final;
    void setApplicationContext(ApplicationContext* context) override final;
    void accept(ISceneVisitor* visitor) override final;
    entt::registry& getRegistry() override final;

    SystemManager* getSystemManager();

private:
    void checkBoundary(const sf::Vector2u& boundary, sf::Sprite& obj);

private:
    ApplicationContext* m_appContext;
    entt::registry m_reg;
    entt::entity m_player;
    SystemManager m_system;
};