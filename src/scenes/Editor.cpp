#include "Editor.hpp"

Editor::Editor(GlobalData* sysData)
	: m_data(sysData)
	, m_reg(nullptr)
{}

Editor::~Editor()
{
	ImGui::SFML::Shutdown();
}

void Editor::init()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 4;
	settings.minorVersion = 3;

	m_panelFlags = 0;
	m_panelFlags |= ImGuiWindowFlags_NoMove;
	m_panelFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	m_panelFlags |= ImGuiWindowFlags_NoBackground;
	m_panelFlags |= ImGuiWindowFlags_NoSavedSettings;
	m_panelFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	m_panelFlags |= ImGuiWindowFlags_NoNav;
	m_panelFlags |= ImGuiWindowFlags_NoDecoration;
	m_panelFlags |= ImGuiWindowFlags_NoNavInputs;
	m_panelFlags |= ImGuiWindowFlags_NoNavFocus;

	m_expandablePanelFlags = 0;
	//m_expandablePanelFlags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::SFML::Init(m_data->window);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	m_game = std::make_unique<Sandbox>(m_data);
	m_game->init();
	m_reg = &m_game->getRegistry();
	m_sceneViewTextureID = m_reg->create();
	m_reg->emplace<SceneViewRenderer>(m_sceneViewTextureID, m_data->Configuration<int>(WIDTH), m_data->Configuration<int>(HEIGHT), settings);
}

void Editor::processEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{

	}

	ImGui::SFML::ProcessEvent(m_data->window, event);
}

void Editor::processInput()
{
	m_game->processInput();
}

void Editor::update()
{
	m_game->update();
}

void Editor::render()
{
	ImGui::SFML::Update(m_data->window, sf::seconds(m_data->deltaTime));

	renderPerformancePanel();
	renderLogViewPanel();
	renderSceneViewPanel();
	renderFileExplorerPanel();
	renderPropertiesPanel();

	ImGui::SFML::Render(m_data->window);
}

void Editor::pause()
{}

void Editor::resume()
{}

entt::registry& Editor::getRegistry()
{
	return *m_reg;
}

void Editor::renderPerformancePanel()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2));
	ImGui::Begin("Left Panel 1", NULL, m_panelFlags);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::Begin("Performance Panel", NULL, m_expandablePanelFlags);

	ImGui::End();
}

void Editor::renderLogViewPanel()
{
	ImGui::SetNextWindowPos(ImVec2(0, m_data->window.getSize().y / 2));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2));
	ImGui::Begin("Left Panel 2", NULL, m_panelFlags);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::Begin("Log View Panel", NULL, m_expandablePanelFlags);


	ImGui::End();
}

static void aspectRatio(ImGuiSizeCallbackData* data) 
{ 
	float aspect_ratio = *(float*)data->UserData;
	data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspect_ratio);
}

void Editor::renderSceneViewPanel()
{
	//ImVec2 extraSizeNeeded = ImVec2(0.0f, ImGui::GetFrameHeight()); // For title-bar. Multiply by 2 if you have a menu.

	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5, 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y / m_data->aspectRatio));
	ImGui::Begin("Middle Panel 1", NULL, m_panelFlags);
	ImGui::End();

	const float aspect_ratio = 16.0f / 9.0f;
	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), aspectRatio, (void*)&aspect_ratio);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene View Panel", NULL, m_expandablePanelFlags);
	ImGui::PopStyleVar();

	m_reg->get<SceneViewRenderer>(m_sceneViewTextureID).rd.clear();

	m_game->render();

	sf::Sprite gameView;
	gameView.setTexture(m_reg->get<SceneViewRenderer>(m_sceneViewTextureID).rd.getTexture());
	gameView.setScale(ImGui::GetWindowWidth() / m_data->window.getSize().x, (ImGui::GetWindowHeight() - ImGui::GetFrameHeight()) / m_data->window.getSize().y);

	//gameView.setScale(ImGui::GetWindowWidth() / m_data->window.getSize().x, ImGui::GetWindowHeight() / m_data->window.getSize().y);
	ImGui::Image(gameView);

	ImGui::End();
}

void Editor::renderFileExplorerPanel()
{
	ImGui::SetNextWindowPos(ImVec2((m_data->window.getSize().x / 5), m_data->window.getSize().y / (m_data->aspectRatio)));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y - (m_data->window.getSize().y / (m_data->aspectRatio))));
	ImGui::Begin("Middle Panel 2", NULL, m_panelFlags);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2((m_data->window.getSize().x / 5), m_data->window.getSize().y / (m_data->aspectRatio)), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y - (m_data->window.getSize().y / (m_data->aspectRatio))), ImGuiCond_Once);
	ImGui::Begin("FileExplorer Panel", NULL, m_expandablePanelFlags);

	ImGui::ShowDemoWindow();

	ImGui::End();
}

void Editor::renderPropertiesPanel()
{
	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio)), 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x - (m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio))), m_data->window.getSize().y));
	ImGui::Begin("Right Panel 1", NULL, m_panelFlags);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio)), 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x - (m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio))), m_data->window.getSize().y), ImGuiCond_Once);
	ImGui::Begin("Properties Panel", NULL, m_expandablePanelFlags);


	ImGui::End();
}
