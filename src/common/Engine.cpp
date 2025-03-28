#include "Engine.hpp"

/**
 * @brief [Public] Default constuctor.
*/
Engine::Engine()
    : sysData(std::make_shared<GlobalData>())
{
    // Start logging in the console until configuration file is read.
    Logger::getInstance().setupConsoleLog();

    LOG_INFO(Logger::get()) << "Initializing system. Reading main configuration file...";

    if (sysData->saveManager.init(sysData->configData) == SystemStatus::SAVE_MNGR_SUCCESS)
    {
        LOG_INFO(Logger::get()) << "Successfully read config file.";

        std::string logPath = sysData->saveManager.resolvePath(sysData->Configuration<std::string>(DEBUG_LOG_FOLDER)).string();
        Logger::getInstance().toggleLogging(sysData->Configuration<bool>(DEBUG_MODE));
        Logger::getInstance().setFilterSeverity(sysData->Configuration<std::string>(DEBUG_LOG_FILTER_SEVERITY));

        LOG_INFO(Logger::get()) << "Now logging to file. Log file located at: " << logPath;

        Logger::getInstance().removeAllSinks();
        Logger::getInstance().setupFileLog(logPath);

        configureWindow();
    }

    LOG_INFO(Logger::get()) << "System initialize. Starting main thread...";
}

/**
 * @brief [Public] Main thread of the application. Responsible for handling opengl context and
 * SFML event handling.
*/
void Engine::run()
{
    LOG_TRACE(Logger::get()) << "----- Main thread started! -----";

    startThreads();
    LOG_INFO(Logger::get()) << "All threads started";

    sf::Event event;
    while (sysData->window.waitEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            LOG_INFO(Logger::get()) << "Triggered Event::Closed";
            sysData->window.close();
            break;

        case sf::Event::Resized:
        {
            LOG_INFO(Logger::get()) << "Triggered Event::Resized";
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
            // Pause
            LOG_INFO(Logger::get()) << "Triggered Event::LostFocus";
            break;

        case sf::Event::GainedFocus:
            // Resume
            LOG_INFO(Logger::get()) << "Triggered Event::GainedFocus";
            break;

        default:
            sysData->sceneManager.getActiveScene()->processEvent(event);
            break;
        }
    }

    LOG_TRACE(Logger::get()) << "----- Main thread ended! -----";
}

void Engine::startThreads()
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

/**
 * @brief [Private] Setup the SFML window and opengl context.
*/
void Engine::configureWindow()
{
    LOG_INFO(Logger::get()) << "Configuring window...";

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
    sysData->sceneManager.addScene(std::make_unique<Editor>(sysData.get()));
    sysData->sceneManager.processChange();

    LOG_DEBUG(Logger::get()) << "System info: \n"
        << "\tWindow name: " << name << "\n"
        << "\tWindow width: " << width << "\n"
        << "\tWindow height: " << height << "\n"
        << "\tDelta time: " << sysData->deltaTime;

    LOG_DEBUG(Logger::get()) << "Configuration data: \n" << sysData->configData;
}

/**
 * @brief [Private] Seperate thread for physics simulation.
*/
void Engine::physicThread()
{
    LOG_TRACE(Logger::get()) << "----- Physic thread started! -----";

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

    LOG_TRACE(Logger::get()) << "----- Physic thread ended! -----";
}

/**
 * @brief [Private] Seperate thread for rendering.
*/
void Engine::renderThread()
{
    LOG_TRACE(Logger::get()) << "----- Render thread started! -----";

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

    LOG_TRACE(Logger::get()) << "----- Render thread ended! -----";
}

/**
 * @brief [Private] Seperate thread for audio management.
*/
void Engine::audioThread()
{
    LOG_TRACE(Logger::get()) << "----- Audio thread started! -----";

    while (true)
    {

    }

    LOG_TRACE(Logger::get()) << "----- Audio thread ended! -----";
}

/**
 * @brief [Private] Seperate thread for resource management.
*/
void Engine::resourceThread()
{
    LOG_TRACE(Logger::get()) << "----- Resource thread started! -----";

    while (true)
    {

    }

    LOG_TRACE(Logger::get()) << "----- Resource thread ended! -----";
}