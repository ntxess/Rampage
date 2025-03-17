#include "Engine.hpp"

/**
 * @brief [Public] Default constuctor.
*/
Engine::Engine()
    : sysData(std::make_shared<GlobalData>())
{
    // Start threads on successful init of the application configs
    if (init() == SystemStatus::SUCCESS)
    {
        m_physicThread = std::thread(&Engine::physicThread, this);
        m_physicThread.detach();

        m_renderThread = std::thread(&Engine::renderThread, this);
        m_renderThread.detach();

        m_audioThread = std::thread(&Engine::audioThread, this);
        m_audioThread.detach();

        m_resourceThread = std::thread(&Engine::resourceThread, this);
        m_resourceThread.detach();
    }
}

/**
 * @brief [Public] Main thread of the application. Responsible for handling opengl context and
 * SFML event handling.
*/
void Engine::run()
{
    std::cout << RED << "[Thread] | Main thread start\n";

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
    if (sysData->saveManager.init(sysData->configData) == SystemStatus::SAVE_MNGR_SUCCESS)
    {
        configureWindow();
        return SystemStatus::SUCCESS;
    }

    return SystemStatus::ERROR;
}

/**
 * @brief [Private] Setup the SFML window and opengl context.
*/
void Engine::configureWindow()
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
    sysData->deltaTime = static_cast<float>(1.f / sysData->Configuration<double>(FRAMERATE));
    sysData->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    sysData->window.setActive(false);
    sysData->sceneManager.addScene(std::make_unique<Sandbox>(sysData.get()));
    sysData->sceneManager.processChange();
}

/**
 * @brief [Private] Seperate thread for physics simulation.
*/
void Engine::physicThread()
{
    std::cout << GREEN << "[Thread] | Physic thread start\n" << RESET;

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
            m_inputConsumer.acquire();
            sysData->sceneManager.getActiveScene()->update();
            m_inputProducer.release();
            accumulator -= sysData->deltaTime;
        }
    }
}

/**
 * @brief [Private] Seperate thread for rendering.
*/
void Engine::renderThread()
{
    std::cout << YELLOW << "[Thread] | Render thread start\n" << RESET;

    sysData->window.setActive(true);

    while (sysData->window.isOpen())
    {
        sysData->window.clear();
        sysData->sceneManager.processChange();
        m_inputProducer.acquire();
        sysData->sceneManager.getActiveScene()->processInput();
        m_inputConsumer.release();
        sysData->sceneManager.getActiveScene()->render();
        sysData->window.display();
    }
}

/**
 * @brief [Private] Seperate thread for audio management.
*/
void Engine::audioThread()
{
    std::cout << BLUE << "[Thread] | Audio thread start\n" << RESET;

    while (true)
    {

    }
}

/**
 * @brief [Private] Seperate thread for resource management.
*/
void Engine::resourceThread()
{
    std::cout << MAGENTA << "[Thread] | Resource thread start\n" << RESET;

    while (true)
    {

    }
}