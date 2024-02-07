#include "Sandbox.hpp"

Sandbox::Sandbox(GlobalData* sysData)
	: m_data(sysData)
{}

void Sandbox::init()
{


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
