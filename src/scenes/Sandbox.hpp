#pragma once 

#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../scenes/Scene.hpp"
#include "../common/Entity.hpp"
#include "../managers/SystemManager.hpp"
#include <entt/entity/registry.hpp>
#include <concurrent_queue.h>

// Testing
#include <thread>
#include <chrono>
#include <mutex>
#include <filesystem>
#include "../common/events/Event.hpp"

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

	concurrency::concurrent_queue<std::shared_ptr<Event>> m_eventQueue;

	// Testing
	int j = 0;
	std::unique_ptr<Entity> m_object;
	std::mutex mtx;
};