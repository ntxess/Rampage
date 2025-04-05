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
	if (!m_defaultFont.loadFromFile("E:\\Dev\\Rampage\\assets\\font\\Prototype.ttf"))
	{
		LOG_FATAL(Logger::get()) << "Failed to load default font.";
	}

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
	ImGui::GetIO().ConfigDockingAlwaysTabBar = true; // ImGUI bug: Setting this true, will prevent you from using SetNextWindowSizeConstraints

	// Initializing all the scenes for selection
	std::unique_ptr<IScene> sandbox = std::make_unique<Sandbox>(m_data);
	m_sceneMap["Sandbox"] = std::make_unique<SceneData>(std::move(sandbox), m_data->Configuration<int>(WIDTH), m_data->Configuration<int>(HEIGHT), settings);

	std::unique_ptr<IScene> mainmenu1 = std::make_unique<MainMenu>(m_data);
	m_sceneMap["MainMenu1"] = std::make_unique<SceneData>(std::move(mainmenu1), m_data->Configuration<int>(WIDTH), m_data->Configuration<int>(HEIGHT), settings);

	std::unique_ptr<IScene> gameOfLifeSim = std::make_unique<GameOfLifeSim>(m_data);
	m_sceneMap["GameOfLifeSim"] = std::make_unique<SceneData>(std::move(gameOfLifeSim), m_data->Configuration<int>(WIDTH), m_data->Configuration<int>(HEIGHT), settings);

	// Load in first scene of map
	m_selectedSceneKey = m_sceneMap.begin()->first;
	m_reg = &m_sceneMap[m_selectedSceneKey]->scene->getRegistry();
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
	m_sceneMap[m_selectedSceneKey]->scene->processInput();
}

void Editor::update()
{
	m_sceneMap[m_selectedSceneKey]->scene->update();
}

void Editor::render()
{
	ImGui::SFML::Update(m_data->window, sf::seconds(m_data->deltaTime));

	setupDockspace();
	renderDebugPanel();
	renderPerformancePanel();
	renderLogViewPanel();
	renderSceneViewPanel();
	renderFileExplorerPanel();
	renderPropertiesPanel();
	ImGui::ShowDemoWindow();

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

void Editor::setupDockspace()
{
	m_dockspaceId1 = ImGui::GetID("Dockspace1");
	m_dockspaceId2 = ImGui::GetID("Dockspace2");
	m_dockspaceId3 = ImGui::GetID("Dockspace3");
	m_dockspaceId4 = ImGui::GetID("Dockspace4");
	m_dockspaceId5 = ImGui::GetID("Dockspace5");

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x, m_data->window.getSize().y));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2));
	ImGui::Begin("##LP1", NULL, m_panelFlags);
	ImGui::DockSpace(m_dockspaceId1, ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiDockNodeFlags_NoResize);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, m_data->window.getSize().y / 2));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2));
	ImGui::Begin("##LP2", NULL, m_panelFlags);
	ImGui::DockSpace(m_dockspaceId2, ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiDockNodeFlags_NoResize);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5, 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y / m_data->aspectRatio));
	ImGui::Begin("##MP1", NULL, m_panelFlags);
	ImGui::DockSpace(m_dockspaceId3, ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y / m_data->aspectRatio), ImGuiDockNodeFlags_NoResize);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2((m_data->window.getSize().x / 5), m_data->window.getSize().y / (m_data->aspectRatio)));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y - (m_data->window.getSize().y / (m_data->aspectRatio))));
	ImGui::Begin("##MP2", NULL, m_panelFlags);
	ImGui::DockSpace(m_dockspaceId4, ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y - (m_data->window.getSize().y / (m_data->aspectRatio))), ImGuiDockNodeFlags_NoResize);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio)), 0));
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x - (m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio))), m_data->window.getSize().y));
	ImGui::Begin("##RP1", NULL, m_panelFlags);
	ImGui::DockSpace(m_dockspaceId5, ImVec2(m_data->window.getSize().x - (m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio))), m_data->window.getSize().y), ImGuiDockNodeFlags_NoResize);
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void Editor::renderDebugPanel()
{
	ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::Begin("Debug Panel", NULL, m_expandablePanelFlags);

	if (ImGui::CollapsingHeader("Scene View Option"))
	{
		if (ImGui::BeginTable("split", 1))
		{
			ImGui::TableNextColumn(); ImGui::Checkbox("Display Entity ID", &m_enableEntityID);
			ImGui::TableNextColumn(); ImGui::Checkbox("Display Entity Box Visualizer", &m_enableEntityCollider);
			ImGui::TableNextColumn(); ImGui::Checkbox("Display Entity Heading", &m_enableEntityHeading);
			ImGui::TableNextColumn(); ImGui::Checkbox("Display Quad-Tree Visualizer", &m_enableQuadTreeVisualizer);

			ImGui::EndTable();
			ImGui::SliderInt("Spawn Entity", &m_totalEntity, 0, 10000);
		}
	}

	if (ImGui::CollapsingHeader("Scene Manager"))
	{
		for (const auto& [sceneName, sceneObj] : m_sceneMap)
		{
			if (ImGui::Selectable(sceneName.c_str(), m_selectedSceneKey == sceneName))
			{
				m_selectedSceneKey = sceneName;
				m_reg = &m_sceneMap[sceneName]->scene->getRegistry();
			}
		}
	}

	if (ImGui::CollapsingHeader("Animation Manager"))
	{

	}

	if (ImGui::CollapsingHeader("Resource Manager"))
	{

	}

	if (ImGui::CollapsingHeader("Save Manager"))
	{

	}

	ImGui::End();
}

void Editor::renderPerformancePanel()
{
	ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::Begin("Performance Panel", NULL, m_expandablePanelFlags);

	ImGui::End();
}

void Editor::renderLogViewPanel()
{
	ImGui::SetNextWindowDockID(m_dockspaceId2, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / 5, m_data->window.getSize().y / 2), ImGuiCond_Once);
	ImGui::Begin("Log View Panel", NULL, m_expandablePanelFlags);


	ImGui::End();
}

static void constrainedByAspectRatio(ImGuiSizeCallbackData* data) 
{ 
	float aspectRatio = *(float*)data->UserData;
	data->DesiredSize.y = (float)(int)(data->DesiredSize.x / aspectRatio);
	LOG_TRACE(Logger::get()) << "ImGuiSizeCallback(): Desired Window Size: " << data->DesiredSize.x << " x " << data->DesiredSize.y;
}

void Editor::renderSceneViewPanel()
{
	const float aspectRatio = 16.0f / 9.0f;
	ImGui::GetIO().ConfigDockingAlwaysTabBar = false;
	ImGui::SetNextWindowDockID(m_dockspaceId3, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints
	(
		ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y / m_data->aspectRatio), 
		ImVec2(FLT_MAX, FLT_MAX), 
		constrainedByAspectRatio,
		(void*)&aspectRatio
	);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scene View Panel", NULL, m_expandablePanelFlags);
	ImGui::PopStyleVar();

	m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd.clear();

	// Call to the actual scenes render function
	m_sceneMap[m_selectedSceneKey]->scene->render();

	displayEntityVisualizers();
	displayCollisionSystemVisualizer();

	sf::Sprite gameView;
	gameView.setTexture(m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd.getTexture());
	gameView.setScale(ImGui::GetWindowWidth() / m_data->window.getSize().x, (ImGui::GetWindowHeight() - ImGui::GetFrameHeight()) / m_data->window.getSize().y);
	ImGui::Image(gameView);

	ImGui::End();
	ImGui::GetIO().ConfigDockingAlwaysTabBar = true;
}

void Editor::renderFileExplorerPanel()
{
	ImGui::SetNextWindowDockID(m_dockspaceId4, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2((m_data->window.getSize().x / 5), m_data->window.getSize().y / (m_data->aspectRatio)), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x / (m_data->aspectRatio), m_data->window.getSize().y - (m_data->window.getSize().y / (m_data->aspectRatio))), ImGuiCond_Once);
	ImGui::Begin("FileExplorer Panel", NULL, m_expandablePanelFlags);


	ImGui::End();
}

void Editor::renderPropertiesPanel()
{
	ImGui::SetNextWindowDockID(m_dockspaceId5, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio)), 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_data->window.getSize().x - (m_data->window.getSize().x / 5 + (m_data->window.getSize().x / (m_data->aspectRatio))), m_data->window.getSize().y), ImGuiCond_Once);
	ImGui::Begin("Properties Panel", NULL, m_expandablePanelFlags | ImGuiWindowFlags_AlwaysVerticalScrollbar);
	static std::unordered_map<entt::entity, bool> closableGroups;

	const auto& view = m_reg->view<Sprite>();
	for (const auto& entityID : view)
	{
		// Check if the entity ID is already in the map and set it to true if not
		// This ensures that the group is closable by default
		if (!closableGroups.count(entityID))
		{
			closableGroups[entityID] = true;
		}

		if (closableGroups[entityID] == false)
		{
			m_reg->destroy(entityID);
			continue;
		}

		std::string ID = "Entity " + std::to_string(static_cast<unsigned int>(entityID));
		if (ImGui::CollapsingHeader(ID.c_str(), &closableGroups[entityID]))
		{
			const float divider1Pos = ImGui::GetWindowWidth() / 3.f;
			const float divider2Pos = (ImGui::GetWindowWidth() * 2.f) / 3.f;
			const float elDivider1Pos = ImGui::GetWindowWidth() / 6.f;

			sf::Sprite& sprite = m_reg->get<Sprite>(entityID).sprite;

			ImGui::BeginChild(("##EntityColm" + ID).c_str(), { ImGui::GetWindowWidth(), ImGui::GetWindowHeight() });
			
			ImGui::PushItemWidth((ImGui::GetWindowWidth() / 4.f) - 25.f);

			ImGui::Separator();
			ImGui::SeparatorText("Sprite");
			ImGui::Separator();

			float xOrigin = sprite.getOrigin().x;
			float yOrigin = sprite.getOrigin().y;
			float xTextureWidth = sprite.getTextureRect().width;
			float yTextureHeight = sprite.getTextureRect().height;
			bool isRepeated = sprite.getTexture()->isRepeated();
			int rHexVal = sprite.getColor().r;
			int gHexVal = sprite.getColor().g;
			int bHexVal = sprite.getColor().b;

			ImGui::Text("Texture");
			ImGui::SameLine(divider1Pos);
			ImGui::Text("Color");
			ImGui::NewLine();

			ImGui::SameLine(30.f);
			ImGui::ImageButton(("##EntityButton" + ID).c_str(), *sprite.getTexture(), {48.f, 48.f}, sf::Color::Transparent, sf::Color::White);
			//ImGui::SameLine();
			//if (ImGui::Checkbox(("Repeat##SetRepeat" + ID).c_str(), &isRepeated))
			//{
			//	m_reg->emplace<Sprite>(entityID, m_data->textureManager["player"], isRepeated);
			//	sprite = m_reg->get<Sprite>(entityID).sprite;
			//}

			ImGui::SameLine(divider1Pos);
			auto rHexValInput = ImGui::InputInt(("R##SpColorHexInput" + ID).c_str(), &rHexVal); // NOTE: 23 is the spacing between InputInt and the next item
			ImGui::SetCursorPos({ divider1Pos, 71.f});
			auto gHexValInput = ImGui::InputInt(("G##SpColorHexInput" + ID).c_str(), &gHexVal);
			ImGui::SetCursorPos({ divider1Pos, 94.f});
			auto bHexValInput = ImGui::InputInt(("B##SpColorHexInput" + ID).c_str(), &bHexVal);

			if (rHexValInput || gHexValInput || bHexValInput) 
				sprite.setColor(sf::Color(rHexVal, gHexVal, bHexVal));

			ImGui::NewLine();
			ImGui::Text("Origin");
			ImGui::SameLine(divider1Pos);
			ImGui::Text("Dimension");

			auto xOriginInput = ImGui::InputFloat(("X##TfOriginInput" + ID).c_str(), &xOrigin);
			ImGui::SameLine(divider1Pos);
			auto textureWidthInput = ImGui::InputFloat(("Width##TfTextureInput" + ID).c_str(), &xTextureWidth);

			auto yOriginInput = ImGui::InputFloat(("Y##TfOriginInput" + ID).c_str(), &yOrigin);
			ImGui::SameLine(divider1Pos);
			auto textureHeightInput = ImGui::InputFloat(("Height##TfTextureInput" + ID).c_str(), &yTextureHeight);

			if (xOriginInput || yOriginInput) 
				sprite.setOrigin(xOrigin, yOrigin);

			if (textureWidthInput || textureHeightInput) 
				sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(xTextureWidth), static_cast<int>(yTextureHeight)));
			
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::SeparatorText("Transform");
			ImGui::Separator();

			float xPos = sprite.getPosition().x;
			float yPos = sprite.getPosition().y;
			float rotationDegree = sprite.getRotation();
			float xScale = sprite.getScale().x;
			float yScale = sprite.getScale().y;

			ImGui::Text("Position");
			ImGui::SameLine(divider1Pos);
			ImGui::Text("Rotation");
			ImGui::SameLine(divider2Pos);
			ImGui::Text("Scale");

			auto xPosInput = ImGui::InputFloat(("X##TfPosInput" + ID).c_str(), &xPos);
			ImGui::SameLine(divider1Pos);
			auto degRotationInput = ImGui::InputFloat(("Deg##TfRotationInput" + ID).c_str(), &rotationDegree);
			ImGui::SameLine(divider2Pos);
			auto xScaleInput = ImGui::InputFloat(("X##TfScaleInput" + ID).c_str(), &xScale);

			auto yPosInput = ImGui::InputFloat(("Y##TfPosInput" + ID).c_str(), &yPos);
			ImGui::SameLine(divider1Pos);
			auto degRotationSlider = ImGui::SliderFloat(("Deg##TfRotationSlider" + ID).c_str(), &rotationDegree, -360.f, 360.f);
			ImGui::SameLine(divider2Pos);
			auto yScaleInput = ImGui::InputFloat(("Y##TfScaleInput" + ID).c_str(), &yScale);

			if (xPosInput || yPosInput)
				sprite.setPosition(xPos, yPos);

			if (degRotationInput || degRotationSlider)
				sprite.setRotation(rotationDegree);

			if (xScaleInput || yScaleInput)
				sprite.setScale(xScale, yScale);

			if (m_reg->all_of<UpdateEntityEvent>(entityID))
			{
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::SeparatorText("Entity Event Collider");
				ImGui::Separator();

				ImGui::Text("I-Frame duration");
				auto& updateEvent = m_reg->get<UpdateEntityEvent>(entityID);
				int maxDuration = updateEvent.maxDuration.count();
				if (ImGui::InputInt(("Sec##EventMaxDurationInput" + ID).c_str(), &maxDuration))
					updateEvent.maxDuration = std::chrono::milliseconds(maxDuration);
			}

			if (m_reg->all_of<UpdateEntityPolling>(entityID))
			{
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::SeparatorText("Entity Polling Collider");
				ImGui::Separator();

				ImGui::Text("I-Frame duration");
				auto& updatePolling = m_reg->get<UpdateEntityPolling>(entityID);
				int maxDuration = updatePolling.maxDuration.count();
				if (ImGui::InputInt(("Sec##PollingMaxDurationInput" + ID).c_str(), &maxDuration))
					updatePolling.maxDuration = std::chrono::milliseconds(maxDuration);
			}

			ImGui::PopItemWidth();

			if (m_reg->all_of<EntityStatus>(entityID))
			{
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::SeparatorText("Entity Status");
				ImGui::Separator();

				ImGui::Text("Status");
				ImGui::SameLine(divider1Pos);
				ImGui::Text("Value");

				std::string discardedStat = "";
				auto& statsMap = m_reg->get<EntityStatus>(entityID).values;
				ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f, 0, 0, 1.0f });
				for (auto& [name, val] : statsMap)
				{
					ImGui::Text(name.c_str());
					ImGui::SameLine(divider1Pos);
					ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 6.f);
					ImGui::InputFloat(("##StatValInput" + name + ID).c_str(), &val);
					ImGui::SameLine();
					if (ImGui::Button(("##DiscardStatButton" + name + ID).c_str(), { 20.f, 20.f }))
						discardedStat = name;
				}
				ImGui::PopStyleColor();

				if (!discardedStat.empty())
				{
					statsMap.erase(discardedStat);
				}

				ImGui::NewLine();
				ImGui::Text("Add New Stat");
				ImGui::Text("Status");
				ImGui::SameLine(divider1Pos);
				ImGui::Text("Value");
				static char newStatName[64] = { 0 };
				static float newStatVal = 0.f;
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 6.f);
				ImGui::InputText(("##NewStatName" + ID).c_str(), newStatName, 64);
				ImGui::SameLine(divider1Pos);
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth() / 6.f);
				ImGui::InputFloat(("##NewStatVal" + ID).c_str(), &newStatVal);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0.5f, 0, 1.0f });
				if (ImGui::Button(("##AddStatButton" + ID).c_str(), { 20.f, 20.f }))
				{
					if (newStatName[0] != '\0')
					{
						statsMap[newStatName] = newStatVal;
						newStatName[0] = '\0';
						newStatVal = 0.f;
					}
				}
				ImGui::PopStyleColor();
			}

			if (m_reg->all_of<EffectsList>(entityID))
			{
				const char* effectTypes[] = { "NULLTYPE", "INSTANT", "OVERTIME", "TEMPTIMED" }; // NOTE: Update this when adding new effect types
				
				ImGui::PushItemWidth(elDivider1Pos);

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::SeparatorText("Entity Effects");
				ImGui::Separator();

				ImGui::Text("Effect Type");
				ImGui::SameLine(elDivider1Pos + 10.f);
				ImGui::Text("Aff. Status");
				ImGui::SameLine(elDivider1Pos * 2 + 20.f);
				ImGui::Text("Modifier");
				ImGui::SameLine(elDivider1Pos * 3 + 30.f);
				ImGui::Text("Max Duration");
				ImGui::SameLine(elDivider1Pos * 4 + 40.f);
				ImGui::Text("Tick Rate");

				int index = 0;
				int discardIndex = -1;
				auto& effectVec = m_reg->get<EffectsList>(entityID).effectsList;
				for (auto& [effectType, effect] : effectVec)
				{
					int selectedEffectType = static_cast<int>(effectType);
					if (ImGui::Combo(("##EffectType" + std::to_string(index) + ID).c_str(), &selectedEffectType, effectTypes, IM_ARRAYSIZE(effectTypes)))
						effectType = static_cast<EffectType>(selectedEffectType);

					ImGui::SameLine(elDivider1Pos + 10.f);

					char* targetStat = const_cast<char*>(effect.targetStat.c_str());
					if (ImGui::InputText(("##EffectName" + std::to_string(index) + ID).c_str(), targetStat, 64, 0))
						effect.targetStat = targetStat;

					ImGui::SameLine(elDivider1Pos * 2 + 20.f);

					ImGui::InputFloat(("##EffectVal" + std::to_string(index) + ID).c_str(), &effect.modificationVal);

					ImGui::SameLine(elDivider1Pos * 3 + 30.f);

					int maxDuration = effect.maxDuration.count();
					if (ImGui::InputInt(("##EffectMaxDuration" + std::to_string(index) + ID).c_str(), &maxDuration))
						effect.maxDuration = std::chrono::milliseconds(maxDuration);

					ImGui::SameLine(elDivider1Pos * 4 + 40.f);

					int tickRate = effect.tickRate.count();
					if (ImGui::InputInt(("##EffectTickRate" + std::to_string(index) + ID).c_str(), &tickRate))
						effect.tickRate = std::chrono::milliseconds(tickRate);

					ImGui::SameLine(elDivider1Pos * 5 + 50.f);

					ImGui::PushStyleColor(ImGuiCol_Button, { 0.5f, 0, 0, 1.0f });
					if (ImGui::Button(("##DiscardEffectButton" + std::to_string(index) + ID).c_str(), { 20.f, 20.f }))
						discardIndex = index;
					ImGui::PopStyleColor();

					++index;
				}

				if (discardIndex != -1)
				{
					effectVec.erase(effectVec.begin() + discardIndex);
				}

				ImGui::NewLine();
				ImGui::Text("Add New Effects");
				ImGui::Text("Effect Type");
				ImGui::SameLine(elDivider1Pos + 10.f);
				ImGui::Text("Aff. Status");
				ImGui::SameLine(elDivider1Pos * 2 + 20.f);
				ImGui::Text("Modifier");
				ImGui::SameLine(elDivider1Pos * 3 + 30.f);
				ImGui::Text("Max Duration");
				ImGui::SameLine(elDivider1Pos * 4 + 40.f);
				ImGui::Text("Tick Rate");

				static int newEffectType = 0;
				static char newStatusToModify[64] = { 0 };
				static float newEffectVal = 0.f;
				static int newMaxDuration = 0;
				static int newTickRate = 0;

				ImGui::Combo(("##NewEffectType" + std::to_string(index) + ID).c_str(), &newEffectType, effectTypes, IM_ARRAYSIZE(effectTypes));
				ImGui::SameLine(elDivider1Pos + 10.f);
				ImGui::InputText(("##NewEffectName" + std::to_string(index) + ID).c_str(), newStatusToModify, 64, 0);
				ImGui::SameLine(elDivider1Pos * 2 + 20.f);
				ImGui::InputFloat(("##NewEffectVal" + std::to_string(index) + ID).c_str(), &newEffectVal);
				ImGui::SameLine(elDivider1Pos * 3 + 30.f);
				ImGui::InputInt(("##NewEffectMaxDuration" + std::to_string(index) + ID).c_str(), &newMaxDuration);
				ImGui::SameLine(elDivider1Pos * 4 + 40.f);
				ImGui::InputInt(("##NewEffectTickRate" + std::to_string(index) + ID).c_str(), &newTickRate);
				ImGui::SameLine(elDivider1Pos * 5 + 50.f);
				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0.5f, 0, 1.0f });
				if (ImGui::Button(("##AddEffectButton" + std::to_string(index) + ID).c_str(), { 20.f, 20.f }))
				{
					if (newStatusToModify[0] != '\0')
					{
						effectVec.push_back({ 
							static_cast<EffectType>(newEffectType), 
							Effects{newStatusToModify, 
								newEffectVal, 
								std::chrono::milliseconds(newMaxDuration), 
								std::chrono::milliseconds(newTickRate
							)} 
						});

						newStatusToModify[0] = '\0';
						newEffectVal = 0.f;
						newMaxDuration = 0;
						newTickRate = 0;
					}
				}
				ImGui::PopStyleColor();
				ImGui::PopItemWidth();
			}

			ImGui::EndChild();
		}
	}

	ImGui::End();
}

void Editor::displayEntityVisualizers()
{
	auto& sceneRenderTexture = m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID);
	const auto& view = m_reg->view<Sprite>();
	for (const auto& entityID : view)
	{
		auto& spriteEntity = view.get<Sprite>(entityID).sprite;

		if (m_enableEntityID)
		{
			sf::String ID(std::to_string(static_cast<int>(entityID)));
			sf::Text IDText(ID, m_defaultFont, 14);
			IDText.setPosition(spriteEntity.getPosition().x + spriteEntity.getGlobalBounds().getSize().x, spriteEntity.getPosition().y + spriteEntity.getGlobalBounds().getSize().y);
			sceneRenderTexture.rd.draw(IDText);
		}

		if (m_enableEntityCollider)
		{
			const auto& spriteSize = spriteEntity.getGlobalBounds().getSize();
			sf::RectangleShape border;
			border.setSize({ spriteSize.x, spriteSize.y });
			border.setFillColor(sf::Color::Transparent);
			border.setPosition(spriteEntity.getPosition().x, spriteEntity.getPosition().y);
			border.setOrigin({ spriteEntity.getOrigin().x, spriteEntity.getOrigin().y });
			border.setOutlineThickness(2);

			entt::entity player = findEntityID<PlayerInput>();
			if (player != entt::null && player != entityID && m_reg->get<Sprite>(player).getGlobalBounds().intersects(spriteEntity.getGlobalBounds()))
			{
				border.setOutlineColor(sf::Color::Red);
				LOG_TRACE(Logger::get())
					<< "Render block found collision between ["
					<< static_cast<unsigned int>(player)
					<< "] and ["
					<< static_cast<unsigned int>(entityID) << "]";
			}
			else
			{
				border.setOutlineColor(sf::Color::Green);
			}
			sceneRenderTexture.rd.draw(border);
		}
	}
}

void Editor::displayCollisionSystemVisualizer()
{
	if (m_enableQuadTreeVisualizer)
	{
		auto& sceneRenderTexture = m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd;
		auto manager = static_cast<Sandbox*>(m_sceneMap[m_selectedSceneKey]->scene.get())->getSystemManager();
		manager->getSystem<CollisionSystem>()->draw(sceneRenderTexture);
	}
}
