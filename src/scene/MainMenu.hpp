#pragma once 

#include "../core/Components.hpp"
#include "../core/GlobalData.hpp"
#include "../core/interface/IScene.hpp"
#include "../core/interface/ISceneVisitor.hpp"
#include "../core/util/Logger.hpp"
#include "../scene/Scenes.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"

class MainMenu final : public IScene
{
public:
    MainMenu();
    MainMenu(GlobalData* sysData);

    void init() override final;
    void processEvent(const sf::Event& event) override final;
    void processInput() override final;
    void update() override final;
    void render() override final;
    void pause() override final;
    void resume() override final;
    void addData(GlobalData* data) override final;
    void accept(ISceneVisitor* visitor) override final;
    entt::registry& getRegistry() override final;

private:
    GlobalData* m_data;
    entt::registry m_reg;

    entt::entity m_wallpaper;
};