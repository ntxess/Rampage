#pragma once 

#include "../scenes/IScene.hpp"
#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../common/Logger.hpp"
#include "../common/commands/Movement.hpp"
#include "../components/Component.hpp"
#include "../managers/SystemManager.hpp"
#include <entt/entity/registry.hpp>

// Testing
#include "../scenes/MainMenu.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <random>

class Sandbox : public IScene
{
public:
	Sandbox(GlobalData* sysData);
	~Sandbox();

	void init() override;
	void processEvent(const sf::Event& event) override;
	void processInput() override;
	void update() override;
	void render() override;
	void pause() override;
	void resume() override;
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