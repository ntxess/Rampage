#include "Sandbox.hpp"

Sandbox::Sandbox(GlobalData* sysData)
	: m_data(sysData)
{}

void Sandbox::init()
{
    m_system.addSystem<CollisionSystem>(m_data->viewport.getCenter(), sf::Vector2f(m_data->window.getSize()));
	
	if (!texture.loadFromFile("E:\\Dev\\Rampage\\assets\\bg_city\\city 1\\1.png"))
	{
		std::cout << "Texture load failed." << std::endl;
	}

	m_background = std::make_shared<Entity>(m_reg);
	m_background->addComponent<Sprite>(texture);
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

	if (m_background)
	{
		m_background->getComponent<Sprite>().sprite.move(5, 0);
		if (m_background->getComponent<Sprite>().sprite.getPosition().x > m_data->Configuration<int>(WIDTH))
		{
			m_background->getComponent<Sprite>().sprite.setPosition(0, 0);
		}

		if (static int i = 0; i++ > 500)
		{
			entt::entity handle = m_background->getId();

			std::cout << "Entity that are alive:\n";
			for (auto ent : m_reg.view<Sprite>())
				std::cout << (int)ent << std::endl;

			m_background = nullptr;

			std::cout << "Entity that are dead:\n";
			for (auto ent : m_reg.view<Sprite>())
				std::cout << (int)ent << std::endl;
		}
	}
}

void Sandbox::render()
{
	if (m_background)
		m_data->window.draw(m_background->getComponent<Sprite>().sprite);
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
