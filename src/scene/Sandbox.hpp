#pragma once 

#include "../core/Commands.hpp"
#include "../core/Components.hpp"
#include "../core/GlobalData.hpp"
#include "../core/Managers.hpp"
#include "../core/Systems.hpp"
#include "../core/interface/IScene.hpp"
#include "../core/interface/ISceneVisitor.hpp"
#include "../core/util/Logger.hpp"
#include "../scene/Scenes.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <random>

class Sandbox : public IScene
{
public:
    Sandbox();
    Sandbox(GlobalData* sysData);
    ~Sandbox();

    void init() override;
    void processEvent(const sf::Event& event) override;
    void processInput() override;
    void update() override;
    void render() override;
    void pause() override;
    void resume() override;
    void addData(GlobalData* data) override;
    void accept(ISceneVisitor* visitor) override;
    entt::registry& getRegistry() override;

    SystemManager* getSystemManager();

private:
    void checkBoundary(const sf::Vector2u& boundary, sf::Sprite& obj);

private:
    GlobalData* m_data;
    entt::registry m_reg;
    entt::entity m_player;
    SystemManager m_system;
};