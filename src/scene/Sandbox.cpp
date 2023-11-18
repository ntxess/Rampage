#include "Sandbox.hpp"

Sandbox::Sandbox(SystemData* sysData)
	: m_data(sysData)
{}

void Sandbox::init()
{
}

void Sandbox::processEvent(const sf::Event& event)
{
}

void Sandbox::processInput()
{
}

void Sandbox::update()
{
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
