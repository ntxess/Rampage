#pragma once

#include "CommonEnum.hpp"
#include "SystemData.hpp"
#include "FileManager.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "boost/thread.hpp"
#include <string>
#include <memory>
#include <iostream>

class Engine
{
private:
    std::shared_ptr<SystemData> sysData;
    boost::thread window;
    boost::thread physic;
    boost::thread render;
    boost::thread audio;
    boost::thread resource;
    boost::thread menu;
    FileManager fileManager;

public:
    Engine();
    ~Engine() = default;

    void run();
    void test();

private:
    SystemStatus init();
    SystemStatus configureWindow();
    void windowThread();
    void physicThread();
    void renderThread();
    void soundThread();
    void resourceThread();
    void menuThread();
};
