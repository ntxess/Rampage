#pragma once

#include "CommonEnum.hpp"
#include "SystemData.hpp"
#include "SceneManager.hpp"
#include "SpriteManager.hpp"
#include "WaypointManager.hpp"
#include "FileManager.hpp"
#include "Logger.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include <memory>
#include <iostream>

class Engine
{
private:
    std::unique_ptr<SystemData> sys_data;
    FileManager     mngr_file;
    SceneManager    mngr_scene;
    SpriteManager   mngr_sprite;
    WaypointManager mngr_waypoint;
    Logger			mngr_logger;

public:
    Engine();
    ~Engine() = default;

    void run();
    void test();

private:
    SystemStatus init();
    SystemStatus configureWindow();
    void renderEngine();
    void physicEngine();
    void soundEngine();
    void resourceEngine();
};
