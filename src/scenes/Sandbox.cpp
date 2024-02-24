#include "Sandbox.hpp"

Sandbox::Sandbox(GlobalData* sysData)
	: m_data(sysData)
{}

void Sandbox::init()
{
    m_system.addSystem<CollisionSystem>(m_data->viewport.getCenter(), sf::Vector2f(m_data->window.getSize()));

	DataMap resourcePath;
	m_data->saveManager.load("/config/resource.json", resourcePath);
	m_data->textureManager.load(resourcePath, thor::Resources::Reuse);

	m_object = std::make_unique<Entity>(m_reg);
	m_object->addComponent<Sprite>(m_data->textureManager["player"]);


	//std::unique_ptr<Modification> specialAtk = std::make_unique<Heal>(std::make_unique<Damage>(std::make_unique<BaseModification>()));
	//specialAtk->type();
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

	std::scoped_lock<std::mutex> guard(mtx);
	if (m_object)
	{
		m_object->getComponent<Sprite>().move(5, 0);
		if (m_object->getComponent<Sprite>().getPosition().x > m_data->Configuration<int>(WIDTH))
		{
			m_object->getComponent<Sprite>().setTexture(m_data->textureManager["bg"]);
			m_object->getComponent<Sprite>().setPosition(0, j++ * 10);
		}
	}
}

void Sandbox::render()
{
	std::scoped_lock<std::mutex> guard(mtx);
	if (m_object)
		m_data->window.draw(m_object->getComponent<Sprite>().sprite);
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
