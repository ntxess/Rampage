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
#include <algorithm>
#include <random>
#include <vector>
#include <future>

using Grid = std::vector<uint8_t>;

class GameOfLifeSim : public IScene
{
public:
    GameOfLifeSim();
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

private:
    const Grid& getCurrentGrid() const;

    inline int index(int x, int y) const
    {
        return y * m_width + x;
    }

    inline uint8_t getCell(const Grid& grid, int x, int y) const
    {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) return 0;
        return grid[index(x, y)];
    }

private:
    GlobalData* m_data;
    entt::registry m_reg;

    int m_width;
    int m_height;
    Grid m_grids[2];
    std::atomic<int> m_currentReadBuffer{ 0 };
};