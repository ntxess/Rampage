#include "Engine.hpp"

Engine::Engine()
    : sysData(std::make_shared<SystemData>())
    , physic(nullptr)
    , render(nullptr)
    , audio(nullptr)
    , resource(nullptr)
    , menu(nullptr)
{
    if (init() == SystemStatus::OK)
    {
        //physic = std::make_unique<std::thread>(&Engine::physicThread, this);
        //physic->detach();

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
    std::cout << "Starting Main Thread" << std::endl;

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
        default:
            // sysData->sceneManager.GetActiveScene()->ProcessEvent(event);
            break;
        }
    }
}

SystemStatus Engine::init()
{
    if (sysData->fileManager.init(sysData->configuration) == SystemStatus::FILE_MNGR_SUCCESS)
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
    sysData->deltaTime = 1.f / static_cast<float>(Configuration<int>(FRAMERATE));
    sysData->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    sysData->window.setActive(false);

    return SystemStatus::OK;
}

void Engine::physicThread()
{
    std::cout << "Starting Physics Thread" << std::endl;

    while (true)
    {

    }
}

void Engine::renderThread()
{
    std::cout << "Starting Render Thread" << std::endl;

    float newTime, frameTime, interpolation;
    float currentTime = sysData->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (sysData->window.isOpen())
    {
        newTime = sysData->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        // sysData->sceneManager.ProcessSceneChange();
        // sysData->sceneManager.GetActiveScene()->ProcessInput();

        while (accumulator >= sysData->deltaTime)
        {
            // sysData->sceneManager.GetActiveScene()->Update(sysData->deltaTime);
            accumulator -= sysData->deltaTime;
        }

        interpolation = accumulator / sysData->deltaTime;

        sysData->window.clear(sf::Color::Black);
        // sysData->sceneManager.GetActiveScene()->Render(m_data->window, sysData->deltaTime, interpolation);
        sysData->window.display();
    }
}

void Engine::soundThread()
{
    std::cout << "Starting Sound Thread" << std::endl;

    while (true)
    {

    }
}

void Engine::resourceThread()
{
    std::cout << "Starting Resource Thread" << std::endl;

    while (true)
    {

    }
}

void Engine::menuThread()
{
    std::cout << "Starting Menu Thread" << std::endl;

    while (true)
    {

    }
}
