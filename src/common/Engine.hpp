#pragma once

#include "Logger.hpp"
#include "CommonEnum.hpp"
#include "GlobalData.hpp"
#include "../scenes/Editor.hpp"
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
    void startThreads();
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
    std::binary_semaphore m_inputProducer{ 0 };
    std::binary_semaphore m_inputConsumer{ 1 };
};

