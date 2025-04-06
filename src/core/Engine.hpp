#pragma once

#include "GlobalData.hpp"
#include "util/Logger.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <memory>
#include <semaphore>
#include <thread>

class Engine
{
public:
	Engine() = delete;
	Engine(std::unique_ptr<IScene> initialScene);

	void run();

private:
	void startThreads();
	void configureWindow(std::unique_ptr<IScene> initialScene);
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