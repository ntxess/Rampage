#include "GameOfLifeSim.hpp"

GameOfLifeSim::GameOfLifeSim(GlobalData* sysData)
	: m_data(sysData)
{}

GameOfLifeSim::~GameOfLifeSim()
{
	m_reg.clear();
}

void GameOfLifeSim::init()
{
	float width = static_cast<float>(m_data->Configuration<int>(WIDTH));
	float height = static_cast<float>(m_data->Configuration<int>(HEIGHT));

	m_gridWorld = std::vector<std::vector<int>>(width, std::vector<int>(height, 0));
	m_buffer = std::vector<std::vector<int>>(m_gridWorld.size(), std::vector<int>(m_gridWorld[0].size(), 0));
	for (auto& row : m_gridWorld) 
	{
		std::generate(row.begin(), row.end(), []() {
			return rand() % 10 == 0 ? 1 : 0;
		});
	}
}

void GameOfLifeSim::processEvent(const sf::Event& event)
{
	if (drawMode)
	{
		if (event.type == sf::Event::MouseButtonPressed)
			mouseHold = true;

		if (drawMode && event.type == sf::Event::MouseButtonReleased)
			mouseHold = false;
	}
}

void GameOfLifeSim::processInput()
{}

void GameOfLifeSim::update()
{
	for (int i = 0; i < m_gridWorld.size(); i++) {
		for (int j = 0; j < m_gridWorld[i].size(); j++) {
			int neighbors = getNeighbors(m_gridWorld, i, j);
			if (m_gridWorld[i][j] && neighbors < 2)
				m_buffer[i][j] = 0;
			else if (m_gridWorld[i][j] && (neighbors == 2 || neighbors == 3))
				m_buffer[i][j] = m_gridWorld[i][j];
			else if (m_gridWorld[i][j] && neighbors > 3)
				m_buffer[i][j] = 0;
			else if (!m_gridWorld[i][j] && neighbors == 3)
				m_buffer[i][j] = 1;
		}
	}
	m_gridWorld = m_buffer;
}

void GameOfLifeSim::render()
{
	//drawOptions();

	//if (drawMode)
	//{
	//	if (mouseHold)
	//	{
	//		int posX = sf::Mouse::getPosition(m_data->window).x;
	//		int posY = sf::Mouse::getPosition(m_data->window).y;
	//		m_gridWorld[posX][posY] = 1;
	//	}
	//}
	//else
	//{
	//	//for (int i = 0; i < m_gridWorld.size(); i++) {
	//	//	for (int j = 0; j < m_gridWorld[i].size(); j++) {
	//	//		int neighbors = getNeighbors(m_gridWorld, i, j);
	//	//		if (m_gridWorld[i][j] && neighbors < 2)
	//	//			m_buffer[i][j] = 0;
	//	//		else if (m_gridWorld[i][j] && (neighbors == 2 || neighbors == 3))
	//	//			m_buffer[i][j] = m_gridWorld[i][j];
	//	//		else if (m_gridWorld[i][j] && neighbors > 3)
	//	//			m_buffer[i][j] = 0;
	//	//		else if (!m_gridWorld[i][j] && neighbors == 3)
	//	//			m_buffer[i][j] = 1;
	//	//	}
	//	//}
	//	//m_gridWorld = m_buffer;
	//}

	const auto& scrView = m_reg.view<SceneViewRenderer>();
	for (const auto& sceneTextureID : scrView)
	{
		auto& sceneRenderTexture = m_reg.get<SceneViewRenderer>(sceneTextureID).rd;
		for (int i = 0; i < m_gridWorld.size(); i++) {
			for (int j = 0; j < m_gridWorld[i].size(); j++) {
				if (m_gridWorld[i][j]) {
					sf::RectangleShape rectangle;
					rectangle.setSize(sf::Vector2f(1, 1));
					rectangle.setFillColor(sf::Color(50, 168, 82));
					rectangle.setPosition(i, j);
					sceneRenderTexture.draw(rectangle);
				}
			}
		}
	}
}

void GameOfLifeSim::pause()
{}

void GameOfLifeSim::resume()
{}

entt::registry& GameOfLifeSim::getRegistry()
{
	return m_reg;
}

void GameOfLifeSim::drawOptions()
{
	//ImGui::SFML::Update(*_data->_window, sf::seconds(deltaTime));
	//ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize({ 10, 50 });
	//ImGui::Begin("Demo", NULL, _windowFlags);

	//if (ImGui::Button("Max"))
	//{
	//	gridWorld = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	buffer = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	ReadFile("resources/gameOfLifePatterns/max.txt");
	//}

	//if (ImGui::Button("Half Max"))
	//{
	//	gridWorld = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	buffer = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	ReadFile("resources/gameOfLifePatterns/halfmax.txt");
	//}

	//if (ImGui::Button("Space Filler 1"))
	//{
	//	gridWorld = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	buffer = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	ReadFile("resources/gameOfLifePatterns/spacefill1.txt");
	//}

	//if (ImGui::Button("Space Filler 2"))
	//{
	//	gridWorld = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	buffer = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	ReadFile("resources/gameOfLifePatterns/spacefill2.txt");
	//}

	//if (ImGui::Button("Random"))
	//{
	//	gridWorld = std::vector<std::vector<int>>(_data->_window->getSize().x, std::vector<int>(_data->_window->getSize().y, 0));
	//	buffer = std::vector<std::vector<int>>(gridWorld.size(), std::vector<int>(gridWorld[0].size(), 0));
	//	for (auto& row : gridWorld) {
	//		std::generate(row.begin(), row.end(), []() {
	//			return rand() % 10 == 0 ? 1 : 0;
	//			});
	//	}
	//}

	//if (ImGui::Button("Draw Mode"))
	//{
	//	drawMode = true;
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Apply"))
	//{
	//	drawMode = false;
	//}

	//ImGui::End();
	//ImGui::SFML::Render(*_data->_window);
}

int GameOfLifeSim::getNeighbors(std::vector<std::vector<int>>& board, int i, int j) {
	int neighbors = 0;

	// top row
	if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1])
		neighbors++;

	if (i - 1 >= 0 && board[i - 1][j])
		neighbors++;

	if (i - 1 >= 0 && j + 1 < board[i - 1].size() && board[i - 1][j + 1])
		neighbors++;

	// mid row
	if (j - 1 >= 0 && board[i][j - 1])
		neighbors++;

	if (j + 1 < board[i].size() && board[i][j + 1])
		neighbors++;

	// bot row
	if (i + 1 < board.size() && j - 1 >= 0 && board[i + 1][j - 1])
		neighbors++;

	if (i + 1 < board.size() && board[i + 1][j])
		neighbors++;

	if (i + 1 < board.size() && j + 1 < board[i + 1].size() && board[i + 1][j + 1])
		neighbors++;

	return neighbors;
}

void GameOfLifeSim::readFile(const std::string& filename)
{
	std::ifstream input(filename);
	if (!input.is_open())
	{
		std::cout << "Failed to open: " << filename << "\n";
	}
	else
	{
		float w_midWidth = static_cast<float>(m_data->Configuration<int>(WIDTH)) / 2.f;
		float w_midHeight = static_cast<float>(m_data->Configuration<int>(HEIGHT)) / 2.f;
		int midWidth = 0;
		int midheight = 0;

		std::string line;
		input >> line;
		midWidth = stoi(line) / 2;
		input >> line;
		midheight = stoi(line);

		std::string pattern;
		int j = w_midHeight - midheight;
		while (input >> line) {
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == 'O')
				{
					m_gridWorld[i + (w_midWidth - midWidth)][j] = 1;
				}
			}
			j++;
			pattern += line;
		}
	}
}