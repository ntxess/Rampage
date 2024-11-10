#include "Sandbox.hpp"

Sandbox::Sandbox(GlobalData* sysData)
	: m_data(sysData)
{}

void Sandbox::init()
{	
	DataMap resourcePath;
	m_data->saveManager.load("config/resource.json", resourcePath);   // Load config file
	m_data->textureManager.load(resourcePath, thor::Resources::Reuse); // Load the resources from loaded resource paths

	// Create the main player object
	m_player = m_reg.create();
	m_reg.emplace<Sprite>(m_player, m_data->textureManager["player"]);
	m_reg.emplace<TeamTag>(m_player, Team::FRIENDLY);
	m_reg.emplace<PlayerInput>(m_player);
	m_reg.emplace<EffectsList>(m_player);

	m_reg.emplace_or_replace<DirtyMovement>(m_player);
	m_reg.get<PlayerInput>(m_player).input =
	{
		{ sf::Keyboard::W, new Movement(m_player, {0, -1})},
		{ sf::Keyboard::A, new Movement(m_player, {-1, 0})},
		{ sf::Keyboard::S, new Movement(m_player, {0,  1})},
		{ sf::Keyboard::D, new Movement(m_player, {1,  0})}
	};

	float width = m_data->Configuration<int>(WIDTH);
	float height = m_data->Configuration<int>(HEIGHT);
	
	m_reg.get<Sprite>(m_player).setPosition(width / 2, height / 2);

	// Create event effect for collecting coins
	Effects collect;
	collect.statusToModify = "HP";
	collect.modificationVal = -10.f;
	collect.duration = 0;
	m_reg.get<EffectsList>(m_player).effectsList.push_back({ EffectType::INSTANT, collect });

	// Generate a ton of sprite for testing in random places within the boundary of the window
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, width);


	for (size_t i = 0; i < 20000; i++)
	{
		// Entity create and store into the scene's ENTT::entity registry
		entt::entity mob = m_reg.create();
		m_reg.emplace<TeamTag>(mob, Team::ENEMY);
		m_reg.emplace<Sprite>(mob, m_data->textureManager["coin"]);
		m_reg.emplace<EntityStatus>(mob);
		m_reg.get<EntityStatus>(mob).value["HP"] = 1.f;
		m_reg.get<Sprite>(mob).setPosition(float(dist6(rng)), float(dist6(rng) % int(height)));
	}

	m_system.addSystem<CollisionSystem>(m_reg, sf::Vector2f{ 0.f, 0.f }, m_data->window.getSize());
	m_system.addSystem<EventSystem>();
}

void Sandbox::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			m_data->sceneManager.addScene(std::make_unique<MainMenu>(m_data));
	}
}

void Sandbox::processInput()
{
	auto& controller = m_reg.get<PlayerInput>(m_player);
	for (auto& [key, action] : controller.input)
		if (sf::Keyboard::isKeyPressed(key))
			action->execute(m_reg);
}

void Sandbox::update()
{
    m_system.update(m_reg, m_data->deltaTime);

	std::scoped_lock<std::mutex> guard(mtx);

	// Delete anything that has zero or less HP
	auto view = m_reg.view<EntityStatus>();
	for (auto entity : view)
	{
		if (m_reg.get<EntityStatus>(entity).value["HP"] < 0)
		{
			m_system.getSystem<CollisionSystem>()->remove(m_reg, entity);
			m_reg.destroy(entity);
		}
	}
}

void Sandbox::render()
{
	std::scoped_lock<std::mutex> guard(mtx);
	auto view = m_reg.view<Sprite>();
	for (auto entity : view)
	{
		m_data->window.draw(view.get<Sprite>(entity).sprite);
	}
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
