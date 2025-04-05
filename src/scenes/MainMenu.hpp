#pragma once 

#include "../scenes/IScene.hpp"
#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../common/Logger.hpp"
#include "../common/Entity.hpp"
#include "../components/Component.hpp"
#include <entt/entity/registry.hpp>

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
    void accept(ISceneVisitor* visitor) override;
    entt::registry& getRegistry() override;

private:
    GlobalData* m_data;
    entt::registry m_reg;

    entt::entity m_wallpaper;
};