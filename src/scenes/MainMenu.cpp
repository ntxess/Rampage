#include "MainMenu.hpp"

MainMenu::MainMenu(GlobalData* sysData)
    : m_data(sysData)
{}

void MainMenu::init()
{
    std::cout << "inside MainMenu" << std::endl;
}

void MainMenu::processEvent(const sf::Event& event)
{}

void MainMenu::processInput()
{}

void MainMenu::update()
{}

void MainMenu::render()
{}

void MainMenu::pause()
{}

void MainMenu::resume()
{}

entt::registry& MainMenu::getRegistry()
{
    return m_reg;
}