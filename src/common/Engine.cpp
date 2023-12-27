#include "Engine.hpp"

/**
 * @brief [Public] Default constuctor.
*/
Engine::Engine()
    : sysData(std::make_shared<GlobalData>())
    , physic(nullptr)
    , render(nullptr)
    , audio(nullptr)
    , resource(nullptr)
{
    // Start threads on successful init of the application configs
    if (init() == SystemStatus::OK)
    {
        physic = std::make_unique<std::thread>(&Engine::physicThread, this);
        physic->detach();

        render = std::make_unique<std::thread>(&Engine::renderThread, this);
        render->detach();

        audio = std::make_unique<std::thread>(&Engine::audioThread, this);
        audio->detach();

        resource = std::make_unique<std::thread>(&Engine::resourceThread, this);
        resource->detach();
    }
}

/**
 * @brief [Public] Main thread of the application. Responsible for handling opengl context and
 * SFML event handling.
*/
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
            float newWidth = static_cast<float>(event.size.width);
            float newHeight = newWidth / sysData->aspectRatio;
            if (newHeight > event.size.height)
            {
                newHeight = static_cast<float>(event.size.height);
                newWidth = newHeight * sysData->aspectRatio;
            }

            sysData->window.setSize(sf::Vector2u(
                static_cast<unsigned int>(newWidth), 
                static_cast<unsigned int>(newHeight))
            );

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

/**
 * @brief [Private] Initialize the application configurations. Success depends on loading and main config file.
 * @return SystemStatus error code if failure; otherwise, SystemStatus::Success.
*/
SystemStatus Engine::init()
{
    SystemStatus status = sysData->configManager.init(sysData->configData);
    if (status == SystemStatus::CFG_MNGR_SUCCESS)
    {
        configureWindow();
    }

    return status;
}

/**
 * @brief [Private] Setup the SFML window and opengl context.
 * @return SystemStatus error code if failure; otherwise, SystemStatus::Success.
*/
SystemStatus Engine::configureWindow()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 3;

    std::string name = sysData->Configuration<std::string>(NAME);
    unsigned int width = sysData->Configuration<int>(WIDTH);
    unsigned int height = sysData->Configuration<int>(HEIGHT);
    sysData->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    sysData->deltaTime = 1.f / sysData->Configuration<float>(FRAMERATE);
    sysData->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    sysData->window.setActive(false);
    sysData->sceneManager.addScene(std::make_unique<Sandbox>(sysData.get()));
    sysData->sceneManager.processChange();

    return SystemStatus::OK;
}

/**
 * @brief [Private] Seperate thread for physics simulation.
*/
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

/**
 * @brief [Private] Seperate thread for rendering.
*/
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

/**
 * @brief [Private] Seperate thread for audio management.
*/
void Engine::audioThread()
{
    while (true)
    {

    }
}

/**
 * @brief [Private] Seperate thread for resource management.
*/
void Engine::resourceThread()
{
    while (true)
    {

    }
}
