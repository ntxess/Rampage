#include "Sandbox.hpp"

Sandbox::Sandbox(GlobalData* sysData)
	: m_data(sysData)
	, m_player(entt::null)
{}

void Sandbox::init()
{	
	DataMap texturePath;
	m_data->saveManager.load("config/texture.json", texturePath);     // Load config file
	m_data->textureManager.load(texturePath, thor::Resources::Reuse); // Load the texture from loaded paths

	if (!m_defaultFont.loadFromFile("E:\\Dev\\Rampage\\assets\\font\\Prototype.ttf"))
	{
		LOG_FATAL(Logger::get()) << "Failed to load default font.";
	}

	// Create the main player object
	m_player = m_reg.create();
	m_reg.emplace<Sprite>(m_player, m_data->textureManager["player"]);
	m_reg.emplace<TeamTag>(m_player, Team::FRIENDLY);
	m_reg.emplace<PlayerInput>(m_player);
	m_reg.emplace<EffectsList>(m_player);
	m_reg.emplace<EntityStatus>(m_player).value["HP"] = 100.f;
	m_reg.emplace<UpdateEntityPolling>(m_player, std::chrono::milliseconds(0));
	m_reg.get<PlayerInput>(m_player).input =
	{
		{ sf::Keyboard::W, new Movement(m_player, { 0, -1 }) },
		{ sf::Keyboard::A, new Movement(m_player, { -1, 0 }) },
		{ sf::Keyboard::S, new Movement(m_player, { 0,  1 }) },
		{ sf::Keyboard::D, new Movement(m_player, { 1,  0 }) }
	};

	float width = static_cast<float>(m_data->Configuration<int>(WIDTH));
	float height = static_cast<float>(m_data->Configuration<int>(HEIGHT));
	
	m_reg.get<Sprite>(m_player).setPosition(width / 2, height / 2);

	// Create event effect for collecting coins
	Effects collect;
	collect.statusToModify = "HP";
	collect.modificationVal = -10.f;
	m_reg.get<EffectsList>(m_player).effectsList.push_back({ EffectType::INSTANT, collect });

	Effects poison;
	poison.statusToModify = "HP";
	poison.modificationVal = -1.f;
	poison.maxDuration = std::chrono::milliseconds(5000);
	poison.tickRate = std::chrono::milliseconds(1000);
	m_reg.get<EffectsList>(m_player).effectsList.push_back({ EffectType::OVERTIME, poison });

	Effects hpLimiter;
	hpLimiter.statusToModify = "HP";
	hpLimiter.modificationVal = -5.f;
	hpLimiter.maxDuration = std::chrono::milliseconds(5000);
	m_reg.get<EffectsList>(m_player).effectsList.push_back({ EffectType::TEMPTIMED, hpLimiter });

	// Generate a ton of sprite for testing in random places within the boundary of the window
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, static_cast<unsigned int>(width));

	for (size_t i = 0; i < 50; i++)
	{
		// Entity create and store into the scene's ENTT::entity registry
		entt::entity mob = m_reg.create();
		m_reg.emplace<TeamTag>(mob, Team::ENEMY);
		m_reg.emplace<Sprite>(mob, m_data->textureManager["coin"]);
		m_reg.emplace<EntityStatus>(mob);
		m_reg.get<EntityStatus>(mob).value["HP"] = 1000.f;
		m_reg.get<Sprite>(mob).setPosition(float(dist6(rng)), float(dist6(rng) % int(height)));
	}

	m_system.addSystem<CollisionSystem>(m_reg, sf::Vector2f{ 0.f, 0.f }, m_data->window.getSize());
	m_system.addSystem<EventSystem>(std::chrono::milliseconds(36000));
}

void Sandbox::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
			m_data->sceneManager.addScene(std::make_unique<MainMenu>(m_data));

		//auto& controller = m_reg.get<PlayerInput>(m_player);
		//for (auto& [key, action] : controller.input)
		//	if (event.key.code == key)
		//		action->execute(m_reg);
	}
}

void Sandbox::processInput()
{
	auto& controller = m_reg.get<PlayerInput>(m_player);
	for (auto& [key, action] : controller.input)
		if (sf::Keyboard::isKeyPressed(key))
			action->execute(m_reg);

	//m_reg.get<PlayerInput>(m_player).processInput(m_reg);
}

void Sandbox::update()
{
    m_system.update(m_reg, m_data->deltaTime);

	// Delete anything that has zero or less HP
	const auto& view = m_reg.view<EntityStatus>();
	for (const auto& entity : view)
	{
		if (m_reg.get<EntityStatus>(entity).value["HP"] <= 0)
		{
			LOG_INFO(Logger::get()) << "Destroying entity [" << static_cast<unsigned int>(entity) << "]";

			m_system.getSystem<CollisionSystem>()->remove(m_reg, entity);
			m_reg.destroy(entity);
		}
	}
}

void Sandbox::render()
{
	const auto& view = m_reg.view<Sprite>();
	for (const auto& entity : view)
	{
		const auto& spriteEntity = view.get<Sprite>(entity).sprite;
		const auto& spriteSize = spriteEntity.getGlobalBounds().getSize();

		sf::RectangleShape border;
		border.setSize({ spriteSize.x, spriteSize.y });
		border.setFillColor(sf::Color::Transparent);
		border.setPosition(spriteEntity.getPosition().x, spriteEntity.getPosition().y);
		border.setOrigin({ spriteEntity.getOrigin().x, spriteEntity.getOrigin().y });
		border.setOutlineThickness(1);

		if (m_player != entity && m_reg.get<Sprite>(m_player).getGlobalBounds().intersects(spriteEntity.getGlobalBounds()))
		{
			border.setOutlineColor(sf::Color::Red);
		}
		else
		{
			border.setOutlineColor(sf::Color::Green);
		}

		std::string hpStdString = std::to_string(static_cast<int>(m_reg.get<EntityStatus>(entity).value["HP"]));
		sf::String hpString(hpStdString);
		sf::Text hpText(hpString, m_defaultFont, 11);
		hpText.setPosition(border.getPosition().x + border.getGlobalBounds().getSize().x, border.getPosition().y + border.getGlobalBounds().getSize().y);

		m_data->window.draw(hpText);
		m_data->window.draw(border);
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
