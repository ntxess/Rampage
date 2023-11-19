#pragma once 

#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../common/Scene.hpp"
#include <entt/entt.hpp>

class Sandbox : public Scene
{
private:
	GlobalData* m_data;
	entt::registry m_reg;

public:
	Sandbox() = delete;
	Sandbox(GlobalData* sysData);
	Sandbox(const Sandbox&) = delete;
	~Sandbox() = default;

	void init();
	void processEvent(const sf::Event& event);
	void processInput();
	void update();
	void render();
	void pause();
	void resume();
	entt::registry& getRegistry();
};