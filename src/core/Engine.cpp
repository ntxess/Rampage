#include "Engine.hpp"

/**
 * @brief [Public] Normal constuctor.
*/
Engine::Engine(std::unique_ptr<IScene> initialScene)
    : m_sysData(std::make_shared<GlobalData>())
    , m_windowActive(true)
{
    // Start logging in the console until configuration file is read.
    Logger::getInstance().setupConsoleLog();

    LOG_INFO(Logger::get()) << "Initializing system. Reading main configuration file...";

    if (m_sysData->saveManager.init(m_sysData->configData))
    {
        LOG_INFO(Logger::get()) << "Successfully read config file.";

        std::string logPath = m_sysData->saveManager.resolvePath(m_sysData->Configuration<std::string>(DEBUG_LOG_FOLDER)).string();
        Logger::getInstance().toggleLogging(m_sysData->Configuration<bool>(DEBUG_MODE));
        Logger::getInstance().setFilterSeverity(m_sysData->Configuration<std::string>(DEBUG_LOG_FILTER_SEVERITY));

        LOG_INFO(Logger::get()) << "Now logging to file. Log file located at: " << logPath;

        Logger::getInstance().removeAllSinks();
        Logger::getInstance().setupFileLog(logPath);

        configureWindow(std::move(initialScene));
    }

    LOG_INFO(Logger::get()) << "System initialize. Starting main thread...";
}

/**
 * @brief [Public] Main thread of the application. Responsible for handling opengl context and
 * SFML event handling.
*/
void Engine::run()
{
    LOG_INFO(Logger::get()) << "----- Main thread started! -----";

    startThreads();

    sf::Event event;
    while (m_windowActive && m_sysData->window.waitEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            LOG_INFO(Logger::get()) << "Triggered Event::Closed";
            m_windowActive = false;
            m_physicThread.join();
            m_renderThread.join();
            m_audioThread.join();
            m_resourceThread.join();
            m_sysData->window.close();
            break;

        case sf::Event::Resized:
        {
            LOG_INFO(Logger::get()) << "Triggered Event::Resized";
            float newWidth = static_cast<float>(event.size.width);
            float newHeight = newWidth / m_sysData->aspectRatio;
            if (newHeight > event.size.height)
            {
                newHeight = static_cast<float>(event.size.height);
                newWidth = newHeight * m_sysData->aspectRatio;
            }

            m_sysData->window.setSize(sf::Vector2u(
                static_cast<unsigned int>(newWidth),
                static_cast<unsigned int>(newHeight))
            );

            m_sysData->viewport.setSize(newWidth, newHeight);
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
            m_sysData->sceneManager.getActiveScene()->processEvent(event);
            break;
        }
    }

    LOG_INFO(Logger::get()) << "----- Main thread ended! -----";
}

void Engine::startThreads()
{
    m_physicThread = std::thread(&Engine::physicThread, this);
    m_renderThread = std::thread(&Engine::renderThread, this);
    m_audioThread = std::thread(&Engine::audioThread, this);
    m_resourceThread = std::thread(&Engine::resourceThread, this);
}

/**
 * @brief [Private] Setup the SFML window and opengl context.
*/
void Engine::configureWindow(std::unique_ptr<IScene> initialScene)
{
    LOG_INFO(Logger::get()) << "Configuring window...";

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 3;

    std::string name = m_sysData->Configuration<std::string>(NAME);
    unsigned int width = m_sysData->Configuration<int>(WIDTH);
    unsigned int height = m_sysData->Configuration<int>(HEIGHT);
    m_sysData->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    m_sysData->deltaTime = static_cast<float>(1.f / m_sysData->Configuration<double>(FRAMERATE));
    m_sysData->window.create(sf::VideoMode(width, height), name, sf::Style::Default, settings);
    m_sysData->window.setActive(false);
    m_sysData->window.setFramerateLimit(m_sysData->Configuration<double>(FRAMERATE));
    m_sysData->sceneManager.addScene(std::move(initialScene), true, m_sysData.get());
    m_sysData->sceneManager.processChange();

    LOG_DEBUG(Logger::get()) << "System info: \n"
        << "\tWindow name: " << name << "\n"
        << "\tWindow width: " << width << "\n"
        << "\tWindow height: " << height << "\n"
        << "\tDelta time: " << m_sysData->deltaTime;

    LOG_DEBUG(Logger::get()) << "Configuration data: \n" << m_sysData->configData;
}

/**
 * @brief [Private] Seperate thread for physics simulation.
*/
void Engine::physicThread()
{
    LOG_INFO(Logger::get()) << "----- Physic thread started! -----";

    float newTime, frameTime;
    float currentTime = m_sysData->clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;

    while (m_windowActive)
    {
        newTime = m_sysData->clock.getElapsedTime().asSeconds();
        frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        while (m_windowActive && accumulator >= m_sysData->deltaTime)
        {
            m_inputConsumer.acquire();
            m_sysData->sceneManager.getActiveScene()->update();
            m_inputProducer.release();
            accumulator -= m_sysData->deltaTime;
        }
    }

    m_inputConsumer.release();
    m_inputProducer.release();

    LOG_INFO(Logger::get()) << "----- Physic thread ended! -----";
}

/**
 * @brief [Private] Seperate thread for rendering.
*/
void Engine::renderThread()
{
    LOG_INFO(Logger::get()) << "----- Render thread started! -----";

    m_sysData->window.setActive(true);

    while (m_windowActive)
    {
        m_sysData->window.clear();
        m_sysData->sceneManager.processChange();
        m_inputProducer.acquire();
        m_sysData->sceneManager.getActiveScene()->processInput();
        m_inputConsumer.release();
        m_sysData->sceneManager.getActiveScene()->render();
        m_sysData->window.display();
    }

    m_inputConsumer.release();
    m_inputProducer.release();

    LOG_INFO(Logger::get()) << "----- Render thread ended! -----";
}

/**
 * @brief [Private] Seperate thread for audio management.
*/
void Engine::audioThread()
{
    LOG_INFO(Logger::get()) << "----- Audio thread started! -----";

    while (m_windowActive)
    {

    }

    LOG_INFO(Logger::get()) << "----- Audio thread ended! -----";
}

/**
 * @brief [Private] Seperate thread for resource management.
*/
void Engine::resourceThread()
{
    LOG_INFO(Logger::get()) << "----- Resource thread started! -----";

    while (m_windowActive)
    {

    }

    LOG_INFO(Logger::get()) << "----- Resource thread ended! -----";
}