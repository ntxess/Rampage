#pragma once

#include "../scenes/Sandbox.hpp"
#include "CommonEnum.hpp"
#include "GlobalData.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <memory>
#include <semaphore>

class Engine
{
public:
    Engine();

    void run();

private:
    SystemStatus init();
    void configureWindow();
    void physicThread();
    void renderThread();
    void audioThread();
    void resourceThread();

private:
    std::shared_ptr<GlobalData> sysData;
    std::thread m_physicThread;
    std::thread m_renderThread;
    std::thread m_audioThread;
    std::thread m_resourceThread;
    std::binary_semaphore m_inputProducer{ 1 };
    std::binary_semaphore m_inputConsumer{ 0 };
};

