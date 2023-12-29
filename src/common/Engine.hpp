#pragma once

#include "../scenes/Sandbox.hpp"
#include "CommonEnum.hpp"
#include "GlobalData.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <memory>

class Engine
{
public:
    Engine();

    void run();

private:
    SystemStatus init();
    SystemStatus configureWindow();
    void physicThread();
    void renderThread();
    void audioThread();
    void resourceThread();

    std::shared_ptr<GlobalData> sysData;
    std::unique_ptr<std::thread> physic;
    std::unique_ptr<std::thread> render;
    std::unique_ptr<std::thread> audio;
    std::unique_ptr<std::thread> resource;
};

