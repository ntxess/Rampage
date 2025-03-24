#pragma once

#include "../common/GlobalData.hpp"
#include "../components/Component.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <random>
#include <fstream>
#include <algorithm>
#include <sstream>

class GameOfLifeSim : public IScene
{
public:
	GameOfLifeSim(GlobalData* sysData);
	~GameOfLifeSim();

	void init();
	void processEvent(const sf::Event& event);
	void processInput();
	void update();
	void render();
	void pause();
	void resume();
	entt::registry& getRegistry();

	void drawOptions();
	int getNeighbors(std::vector<std::vector<int>>& board, int i, int j);
	void readFile(const std::string& fileName);

private:
	GlobalData* m_data;
	entt::registry m_reg;

	std::vector<std::vector<int>> m_gridWorld;
	std::vector<std::vector<int>> m_buffer;
	bool drawMode = false;
	bool mouseHold = false;
};