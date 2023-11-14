#include "Engine.hpp"

Engine::Engine()
    : sys_data(std::make_unique<SystemData>())
    , mngr_file(sys_data.get())
{
    init();
}

void Engine::run()
{

}

void Engine::test()
{
    while (true)
    {

    }
}

SystemStatus Engine::init()
{
    configureWindow();

    return SystemStatus::OK;
}

SystemStatus Engine::configureWindow()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 3;

    unsigned int width = std::any_cast<int>(sys_data->configData[WIDTH]);
    unsigned int height = std::any_cast<int>(sys_data->configData[HEIGHT]);
    std::string name = std::any_cast<std::string>(sys_data->configData[NAME]);
    sys_data->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    return SystemStatus::OK;
}
