#include "Engine.hpp"

Engine::Engine()
    : sysData(std::make_shared<SystemData>())
    //, window(&Engine::windowThread, this)
    //, physic(&Engine::physicThread, this)
    //, render(&Engine::renderThread, this)
    //, audio(&Engine::soundThread, this)
    //, resource(&Engine::resourceThread, this)
    //, menu(&Engine::menuThread, this)
{
    if (init() == SystemStatus::OK)
    {
        window.detach();
        physic.detach();
        render.detach();
        audio.detach();
        resource.detach();
        menu.detach();
    }
}

void Engine::run()
{
    while (true)
    {

    }

}

void Engine::test()
{
    while (true)
    {

    }
}

SystemStatus Engine::init()
{
    if (fileManager.init(sysData->configuration) == SystemStatus::FILE_MNGR_SUCCESS)
    {
        configureWindow();
    }

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

    unsigned int width = std::any_cast<int>(sysData->configuration[WIDTH]);
    unsigned int height = std::any_cast<int>(sysData->configuration[HEIGHT]);
    std::string name = std::any_cast<std::string>(sysData->configuration[NAME]);
    sysData->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);

    return SystemStatus::OK;
}

void Engine::windowThread()
{
    while (true)
    {

    }
}

void Engine::physicThread()
{
    while (true)
    {

    }
}

void Engine::renderThread()
{
    while (true)
    {

    }
}

void Engine::soundThread()
{
    while (true)
    {

    }
}

void Engine::resourceThread()
{
    while (true)
    {

    }
}

void Engine::menuThread()
{
    while (true)
    {

    }
}
