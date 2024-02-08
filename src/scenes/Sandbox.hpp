#pragma once 

#include "../common/Entity.hpp"
#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../scenes/Scene.hpp"
#include "../managers/SystemManager.hpp"
#include <entt/entt.hpp>
#include <thread>
#include <chrono>
#include <mutex>

class Sandbox : public Scene
{
public:
	Sandbox(GlobalData* sysData);

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
	SystemManager m_system;

	sf::Texture texture;
	std::shared_ptr<Entity> m_background;
};