#pragma once

#include "../core/Commands.hpp"
#include "../core/Components.hpp"
#include "../core/GlobalData.hpp"
#include "../core/Managers.hpp"
#include "../core/interface/IScene.hpp"
#include "../core/interface/ISceneVisitor.hpp"
#include "../core/util/Logger.hpp"
#include "../scene/Scenes.hpp"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include <random>
#include <fstream>
#include <algorithm>
#include <sstream>

class GameOfLifeSim : public IScene
{
public:
    GameOfLifeSim(GlobalData* sysData);
    ~GameOfLifeSim();

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

    void drawOptions();
    int getNeighbors(std::vector<std::vector<int>>& board, int i, int j);
    void readFile(std::string_view fileName);

private:
    GlobalData* m_data;
    entt::registry m_reg;

    std::vector<std::vector<int>> m_gridWorld;
    std::vector<std::vector<int>> m_buffer;
    bool drawMode = false;
    bool mouseHold = false;
};