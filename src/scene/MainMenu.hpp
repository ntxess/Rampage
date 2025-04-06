#pragma once 

#include "../core/Components.hpp"
#include "../core/GlobalData.hpp"
#include "../core/interface/IScene.hpp"
#include "../core/interface/ISceneVisitor.hpp"
#include "../core/util/Logger.hpp"
#include "../scene/Scenes.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"

class MainMenu : public IScene
{
public:
    MainMenu(GlobalData* sysData);

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

private:
    GlobalData* m_data;
    entt::registry m_reg;

    entt::entity m_wallpaper;
};