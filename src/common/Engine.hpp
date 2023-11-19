#pragma once

#include "../scene/Sandbox.hpp"
#include "CommonEnum.hpp"
#include "GlobalData.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <memory>

class Engine
{
private:
    std::shared_ptr<GlobalData> sysData;
    std::unique_ptr<std::thread> physic;
    std::unique_ptr<std::thread> render;
    std::unique_ptr<std::thread> audio;
    std::unique_ptr<std::thread> resource;
    std::unique_ptr<std::thread> menu;

public:
    Engine();
    Engine(const Engine&) = delete;
    ~Engine() = default;

    void run();
    template<typename T>
    T Configuration(ConfigKey key);

    template<typename T>
    T Data(const std::any& val);

private:
    SystemStatus init();
    SystemStatus configureWindow();
    void physicThread();
    void renderThread();
    void soundThread();
    void resourceThread();
    void menuThread();
};

template<typename T>
inline T Engine::Configuration(ConfigKey key)
{
    std::any& val = sysData->configuration[key];
    return (val.type() == typeid(T)) ? std::any_cast<T>(val) : T();
}

template<typename T>
inline T Engine::Data(const std::any& val)
{
    return (val.type() == typeid(T)) ? std::any_cast<T>(val) : T();
}

