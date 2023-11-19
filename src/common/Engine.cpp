#include "Engine.hpp"

Engine::Engine()
    : sysData(std::make_shared<GlobalData>())
    , physic(nullptr)
    , render(nullptr)
    , audio(nullptr)
    , resource(nullptr)
    , menu(nullptr)
{
    if (init() == SystemStatus::OK)
    {
        physic = std::make_unique<std::thread>(&Engine::physicThread, this);
        physic->detach();

        render = std::make_unique<std::thread>(&Engine::renderThread, this);
        render->detach();

        //audio = std::make_unique<std::thread>(&Engine::soundThread, this);
        //audio->detach();

        //resource = std::make_unique<std::thread>(&Engine::resourceThread, this);
        //resource->detach();

        //menu = std::make_unique<std::thread>(&Engine::menuThread, this);
        //menu->detach();
    }
}

void Engine::run()
{
    sf::Event event;
    while (sysData->window.waitEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            sysData->window.close();
            break;

        case sf::Event::Resized:
        {
            float newWidth = event.size.width;
            float newHeight = newWidth / sysData->aspectRatio;
            if (newHeight > event.size.height)
            {
                newHeight = event.size.height;
                newWidth = newHeight * sysData->aspectRatio;
            }
            sysData->window.setSize(sf::Vector2u(newWidth, newHeight));
            sysData->viewport.setSize(newWidth, newHeight);
            break;
        }

        case sf::Event::LostFocus:
            // pause
            break;

        case sf::Event::GainedFocus:
            // resume
            break;

        default:
            sysData->sceneManager.getActiveScene()->processEvent(event);
            break;
        }
    }
}

SystemStatus Engine::init()
{
    if (sysData->configManager.init(sysData->configuration) == SystemStatus::FILE_MNGR_SUCCESS)
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

    std::string name = Configuration<std::string>(NAME);
    unsigned int width = Configuration<int>(WIDTH);
    unsigned int height = Configuration<int>(HEIGHT);
    sysData->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    sysData->deltaTime = 1.f / Configuration<float>(FRAMERATE);
    sysData->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    sysData->window.setActive(false);
    sysData->sceneManager.addScene(std::make_unique<Sandbox>(sysData.get()));

    return SystemStatus::OK;
}
  
void Engine::physicThread()
{
    float newTime, frameTime;
    float currentTime = sysData->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (sysData->window.isOpen())
    {
        newTime = sysData->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= sysData->deltaTime)
        {
            sysData->sceneManager.getActiveScene()->update();
            accumulator -= sysData->deltaTime;
        }
    }
}

void Engine::renderThread()
{
    float newTime, frameTime;
    float currentTime = sysData->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (sysData->window.isOpen())
    {
        newTime = sysData->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        sysData->sceneManager.processChange();
        sysData->sceneManager.getActiveScene()->processInput();
        sysData->sceneManager.getActiveScene()->render();

        sysData->window.display();
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
