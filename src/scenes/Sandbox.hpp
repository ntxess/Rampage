#pragma once 

#include "../scenes/IScene.hpp"
#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../common/Entity.hpp"
#include "../managers/SystemManager.hpp"
#include <entt/entity/registry.hpp>

// Testing
#include <SFML/Graphics/RectangleShape.hpp>
#include <mutex>
#include <random>
#include "../common/commands/ICommand.hpp"
#include "../common/commands/Movement.hpp"
#include "../scenes/MainMenu.hpp"

class Sandbox : public IScene
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

	int j = 0;
	std::unique_ptr<Entity> m_object;
	std::mutex mtx;

	//std::unique_ptr<CollisionSystem> m_collisionSystem;
	//std::unique_ptr<EventSystem> m_eventSystem;

};