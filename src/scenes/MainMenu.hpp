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

	void init();
	void processEvent(const sf::Event& event);
	void processInput();
	void update();
	void render();
	void pause();
	void resume();
	entt::registry& getRegistry();

private:
	GlobalData* m_data;
	entt::registry m_reg;

	entt::entity m_wallpaper;
};