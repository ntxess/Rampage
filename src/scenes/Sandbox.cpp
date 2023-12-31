#include "Sandbox.hpp"

Sandbox::Sandbox(GlobalData* sysData)
	: m_data(sysData)
{}

void Sandbox::init()
{
    //m_data->configData[WIDTH] = 1280;
    //m_data->configManager.save("config.json", sysData->configData);
    //m_data->saveData[entt::hashed_string("TEST_DATA")] = std::make_unique<DataMap>();
    //m_data->saveData[entt::hashed_string("TEST_DATA")]->emplace("DAT1", 1);
    //m_data->saveData[entt::hashed_string("TEST_DATA")]->emplace("DAT2", 2);
    //m_data->saveData[entt::hashed_string("TEST_DATA")]->emplace("DAT3", "four");
    //m_data->saveData[entt::hashed_string("TEST_DATA")]->emplace("DAT4", std::vector<std::any>{"num1", "num2", "num3"});
    //m_data->saveData[entt::hashed_string("TEST_DATA")]->emplace("DAT5", std::vector<std::any>{100, 200, 300});
    //m_data->saveData[entt::hashed_string("TEST_DATA")]->emplace("DAT5", std::vector<std::any>{511.2, 611.3, 711.4});

    DataMap testData = {
        {"DAT0", false},
        {"DAT1", 1},
        {"DAT2", 2},
        {"DAT3", "four"},
        {"DAT4", std::vector<std::any>{"num1", "num2", "4234"}},
        {"DAT5", std::vector<std::any>{3212, 200, 300}},
        {"DAT6", std::vector<std::any>{511.2, 611.3, 711.4}},
    };
    m_data->saveManager.save("test.sav", testData);

    m_system.addSystem<CollisionSystem>(m_data->viewport.getCenter(), sf::Vector2f(m_data->window.getSize()));
}

void Sandbox::processEvent(const sf::Event& event)
{
}

void Sandbox::processInput()
{
}

void Sandbox::update()
{
    m_system.update(m_reg, m_data->deltaTime);
}

void Sandbox::render()
{
}

void Sandbox::pause()
{
}

void Sandbox::resume()
{
}

entt::registry& Sandbox::getRegistry()
{
	return m_reg;
}
