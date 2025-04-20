#include "Editor.hpp"

Editor::Editor()
	: m_data(nullptr)
	, m_panelFlags(0)
	, m_dockspaceId1(0)
	, m_dockspaceId2(0)
	, m_dockspaceId3(0)
	, m_dockspaceId4(0)
	, m_dockspaceId5(0)
	, m_enableEntityID(false)
	, m_enableEntityCollider(false)
	, m_enableEntityHeading(false)
	, m_enableEntityPosition(false)
	, m_enableQuadTreeVisualizer(false)
	, m_sceneDrawScaleX(0.f)
	, m_sceneDrawScaleY(0.f)
	, m_sceneDrawOffset{ 0.f, 0.f }
	, m_totalEntity(0)
	, m_startButtonEnabled(false)
	, m_forwardFrameEnabled(false)
	, m_reg(nullptr)
{
}

Editor::Editor(GlobalData* sysData)
	: m_data(sysData)
	, m_panelFlags(0)
	, m_dockspaceId1(0)
	, m_dockspaceId2(0)
	, m_dockspaceId3(0)
	, m_dockspaceId4(0)
	, m_dockspaceId5(0)
	, m_enableEntityID(false)
	, m_enableEntityCollider(false)
	, m_enableEntityHeading(false)
	, m_enableEntityPosition(false)
	, m_enableQuadTreeVisualizer(false)
	, m_sceneDrawScaleX(0.f)
	, m_sceneDrawScaleY(0.f)
	, m_sceneDrawOffset{ 0.f, 0.f }
	, m_totalEntity(0)
	, m_startButtonEnabled(false)
	, m_forwardFrameEnabled(false)
	, m_reg(nullptr)
{
}

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

	// Disable flags for the dockspace panels; Make panel non-interactive 
	m_panelFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav |
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs |
		ImGuiWindowFlags_NoNavFocus;

	// Enable and setup dockspaces
	ImGui::SFML::Init(m_data->window);
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigDockingAlwaysTabBar = true; // ImGUI bug: Setting this true, will prevent you from using SetNextWindowSizeConstraints
	// which is needed for ratio-resizing of the scene view rendering panel


// Initializing all the scenes for selection
	std::unique_ptr<IScene> sandbox = std::make_unique<Sandbox>(m_data);
	m_sceneMap["Sandbox"] = std::make_unique<SceneData>(
		std::move(sandbox),
		m_data->Configuration<int>(WIDTH),
		m_data->Configuration<int>(HEIGHT),
		settings
	);

	std::unique_ptr<IScene> mainmenu = std::make_unique<MainMenu>(m_data);
	m_sceneMap["MainMenu"] = std::make_unique<SceneData>(
		std::move(mainmenu),
		m_data->Configuration<int>(WIDTH),
		m_data->Configuration<int>(HEIGHT),
		settings
	);

	std::unique_ptr<IScene> gameOfLifeSim = std::make_unique<GameOfLifeSim>(m_data);
	m_sceneMap["GameOfLifeSim"] = std::make_unique<SceneData>(
		std::move(gameOfLifeSim),
		m_data->Configuration<int>(WIDTH),
		m_data->Configuration<int>(HEIGHT),
		settings
	);

	// Load in first scene of map
	m_selectedSceneKey = m_sceneMap.begin()->first;
	m_reg = &m_sceneMap[m_selectedSceneKey]->scene->getRegistry();
	m_gameView.setTexture(m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd.getTexture());
}

void Editor::processEvent(const sf::Event& event)
{
	ImGui::SFML::ProcessEvent(m_data->window, event);
}

void Editor::processInput()
{
	if (m_startButtonEnabled)
		m_sceneMap[m_selectedSceneKey]->scene->processInput();
}

void Editor::update()
{
	if (m_startButtonEnabled || m_forwardFrameEnabled) {
		m_sceneMap[m_selectedSceneKey]->scene->update();
		m_forwardFrameEnabled = false;
	}
}

void Editor::render()
{
	ImGui::SFML::Update(m_data->window, sf::seconds(m_data->deltaTime));

	// Setup dockspace IDs
	m_dockspaceId1 = ImGui::GetID("Dockspace1");
	m_dockspaceId2 = ImGui::GetID("Dockspace2");
	m_dockspaceId3 = ImGui::GetID("Dockspace3");
	m_dockspaceId4 = ImGui::GetID("Dockspace4");
	m_dockspaceId5 = ImGui::GetID("Dockspace5");

	// Style modifications
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });

	// Setup docked panels
	const float width = static_cast<float>(m_data->window.getSize().x);
	const float height = static_cast<float>(m_data->window.getSize().y);
	const float fifthWidth = width / 5.f;
	const float halfHeight = height / 2.f;
	const float scalingWidth = width / m_data->aspectRatio;
	const float scalingHeight = height / m_data->aspectRatio;
	const float scalingFifthWidth = fifthWidth + scalingWidth;

	setupDockPanel({ 0.f, 0.f }, { fifthWidth, halfHeight }, "##LP1", m_dockspaceId1);
	setupDockPanel({ 0.f, halfHeight }, { fifthWidth, halfHeight }, "##LP2", m_dockspaceId2);
	setupDockPanel({ fifthWidth, 0.f }, { scalingWidth, scalingHeight }, "##MP1", m_dockspaceId3);
	setupDockPanel({ fifthWidth, scalingHeight }, { scalingWidth, height - scalingHeight }, "##MP2", m_dockspaceId4);
	setupDockPanel({ scalingFifthWidth, 0.f }, { width - scalingFifthWidth, height }, "##RP1", m_dockspaceId5);

	ImGui::PopStyleVar(3);

	// Render each panel
	renderDebugPanel({ 0.f, 0.f }, { fifthWidth, halfHeight });
	renderPerformancePanel({ 0.f, 0.f }, { fifthWidth, halfHeight });
	renderLogViewPanel({ 0.f, halfHeight }, { fifthWidth, halfHeight });
	renderSceneViewPanel({ fifthWidth, 0.f }, { width, height });
	renderFileExplorerPanel({ fifthWidth, scalingHeight }, { scalingWidth, height - scalingHeight });
	renderPropertiesPanel({ scalingFifthWidth, 0.f }, { width - scalingFifthWidth, height });

	ImGui::ShowDemoWindow();

	ImGui::SFML::Render(m_data->window);
}

void Editor::pause()
{
	// Nothing needed for pause
}

void Editor::resume()
{
	// Nothing needed for resume
}

void Editor::addData(GlobalData* data)
{
	m_data = data;
}

void Editor::accept(ISceneVisitor* visitor)
{
	visitor->visit(this);
}

entt::registry& Editor::getRegistry()
{
	return *m_reg;
}

void Editor::setupDockPanel(const ImVec2& panPos, const ImVec2& panSize, const char* panID, const ImGuiID& dockID) const
{
	ImGui::SetNextWindowPos(panPos);
	ImGui::SetNextWindowSize(panSize);
	ImGui::Begin(panID, nullptr, m_panelFlags);
	ImGui::DockSpace(dockID, panSize, ImGuiDockNodeFlags_NoResize);
	ImGui::End();
}

void Editor::renderDebugPanel(const ImVec2& pos, const ImVec2& size)
{
	ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::Begin("Debug Panel", nullptr, 0);

	const float totalWidth = ImGui::GetContentRegionAvail().x - 16.f;
	const float buttonWidth = totalWidth / 3.f;
	const float buttonHeight = 20.f;

	if (ImGui::Button("Play", ImVec2(buttonWidth, buttonHeight)))
	{
		m_startButtonEnabled = true;
		m_forwardFrameEnabled = false;
	}
	ImGui::SameLine();

	if (ImGui::Button("Pause", ImVec2(buttonWidth, buttonHeight)))
	{
		m_startButtonEnabled = false;
		m_forwardFrameEnabled = false;
	}
	ImGui::SameLine();

	if (ImGui::Button("Forward", ImVec2(buttonWidth, buttonHeight)))
	{
		m_startButtonEnabled = false;
		m_forwardFrameEnabled = true;
	}

	if (ImGui::CollapsingHeader("Scene View Option"))
	{
		if (ImGui::BeginTable("split", 1))
		{
			ImGui::TableNextColumn();
			ImGui::Checkbox("Display Entity ID", &m_enableEntityID);
			ImGui::TableNextColumn();
			ImGui::Checkbox("Display Entity Box Visualizer", &m_enableEntityCollider);
			ImGui::TableNextColumn();
			ImGui::Checkbox("Display Entity Heading", &m_enableEntityHeading);
			ImGui::TableNextColumn();
			ImGui::Checkbox("Display Entity Position", &m_enableEntityPosition);
			ImGui::TableNextColumn();
			ImGui::Checkbox("Display Quad-Tree Visualizer", &m_enableQuadTreeVisualizer);

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

void Editor::renderPerformancePanel(const ImVec2& pos, const ImVec2& size)
{
	ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::Begin("Performance Panel", nullptr, 0);

	ImGui::End();
}

void Editor::renderLogViewPanel(const ImVec2& pos, const ImVec2& size)
{
	ImGui::SetNextWindowDockID(m_dockspaceId2, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::Begin("Log View Panel", nullptr, 0);

	ImGui::End();
}

void Editor::renderSceneViewPanel(const ImVec2& pos, const ImVec2& size)
{
	ImGui::SetNextWindowDockID(m_dockspaceId3, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	ImGui::Begin("Scene View Panel", nullptr, 0);
	ImGui::PopStyleVar();

	auto& renderTexture = m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd;

	// Clear the previous buffer then call to the actual scenes render function
	renderTexture.clear();
	m_sceneMap[m_selectedSceneKey]->scene->render();

	displayEntityVisualizers();
	displayCollisionSystemVisualizer();

	// Get free space inside the window and calculate size that scaled with ratio
	ImVec2 region = ImGui::GetContentRegionAvail();
	const float aspect = m_data->aspectRatio;
	float drawWidth = region.x;
	float drawHeight = drawWidth / aspect;
	if (drawHeight > region.y)
	{
		drawHeight = region.y;
		drawWidth = drawHeight * aspect;
	}

	//Draw the scene texture with correct scaling
	m_gameView.setTexture(renderTexture.getTexture(), true);
	m_gameView.setScale(
		drawWidth / m_gameView.getTexture()->getSize().x,
		drawHeight / m_gameView.getTexture()->getSize().y
	);

	// Center the image in the window content area
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImVec2 viewPos(
		cursorPos.x + (region.x - drawWidth) * 0.5f,
		cursorPos.y + (region.y - drawHeight) * 0.5f
	);

	m_sceneDrawScaleX = drawWidth / m_gameView.getTexture()->getSize().x;
	m_sceneDrawScaleY = drawHeight / m_gameView.getTexture()->getSize().y;
	m_sceneDrawOffset = viewPos;

	ImGui::SetCursorScreenPos(viewPos);

	// Render the selected scene as an image inside ImGui window
	ImGui::Image(m_gameView);

	ImGui::End();
}

void Editor::renderFileExplorerPanel(const ImVec2& pos, const ImVec2& size)
{
	ImGui::SetNextWindowDockID(m_dockspaceId4, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::Begin("FileExplorer Panel", nullptr, 0);

	ImGui::End();
}

void Editor::renderPropertiesPanel(const ImVec2& pos, const ImVec2& size)
{
	ImGui::SetNextWindowDockID(m_dockspaceId5, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::Begin("Properties Panel", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);

	static std::unordered_map<entt::entity, bool> closableGroups;

	const auto& view = m_reg->view<Sprite>();
	for (const auto& entityID : view)
	{
		// Initialize the closable groups and components if it is a new entityID
		if (!closableGroups.count(entityID) || !m_entities.count(entityID))
		{
			closableGroups[entityID] = true;
			m_entities.emplace(entityID, ComponentPropData(entityID));
			updateWayPointCanvas(entityID, m_entities[entityID]);
		}

		//std::string ID = "Entity " + std::to_string(static_cast<unsigned int>(entityID));
		if (ImGui::CollapsingHeader(m_entities[entityID].name.c_str(), &closableGroups[entityID]))
		{
			ImGui::BeginDisabled(m_startButtonEnabled);
			ImGui::BeginChild(("##EntityColm" + m_entities[entityID].name).c_str(), { ImGui::GetWindowWidth() - 26.f, 0.f }, ImGuiChildFlags_AutoResizeY);

			renderComponentProperties<Sprite>(
				entityID,
				"Sprite##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[0],
				m_componentVisitor
			);

			renderComponentProperties<UpdateEntityPolling>(
				entityID,
				"UpdateEntityPolling##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[1],
				m_componentVisitor
			);

			renderComponentProperties<UpdateEntityEvent>(
				entityID,
				"UpdateEntityEvent##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[2],
				m_componentVisitor
			);

			renderComponentProperties<EntityStatus>(
				entityID,
				"EntityStatus##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[3],
				m_componentVisitor
			);

			renderComponentProperties<EffectsList>(
				entityID,
				"EffectsList##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[4],
				m_componentVisitor
			);

			if (renderComponentProperties<MovementPattern>(
				entityID,
				"MovementPattern##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[5],
				m_componentVisitor
			))
			{
				if (ImGui::Button(
					("Set New Path##PathDesigner" + m_entities[entityID].name).c_str(),
					{ ImGui::GetWindowWidth(), 22.f }
				))
				{
					m_entities[entityID].isWaypointEditorOpen = true;
				}
				ImGui::NewLine();

				if (m_entities[entityID].isWaypointEditorOpen)
				{
					displayWayPointCanvas(entityID, m_entities[entityID]);
				}
			}

			renderComponentProperties<TeamTag>(
				entityID,
				"TeamTag##" + m_entities[entityID].name,
				m_entities[entityID].closableComponents[6],
				m_componentVisitor
			);

			ImGui::EndChild();
			ImGui::EndDisabled();
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
			sf::Text IDText(ID, m_defaultFont, 20);
			IDText.setPosition(
				spriteEntity.getPosition().x + spriteEntity.getGlobalBounds().getSize().x,
				spriteEntity.getPosition().y + spriteEntity.getGlobalBounds().getSize().y
			);
			sceneRenderTexture.rd.draw(IDText);
		}

		if (m_enableEntityCollider)
		{
			const auto& globalBounds = spriteEntity.getGlobalBounds();
			sf::RectangleShape border({ globalBounds.width, globalBounds.height });
			border.setOrigin(globalBounds.width / 2.f, globalBounds.height / 2.f);
			border.setPosition(spriteEntity.getPosition());
			border.setRotation(spriteEntity.getRotation());
			border.setFillColor(sf::Color::Transparent);
			border.setOutlineThickness(2);

			entt::entity player = findEntityID<PlayerInput>();
			if (player != entt::null &&
				player != entityID &&
				m_reg->get<Sprite>(player).getGlobalBounds().intersects(spriteEntity.getGlobalBounds()))
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

		if (m_enableEntityPosition)
		{
			sf::String ID("\t " + std::to_string(spriteEntity.getPosition().x) + ", " + std::to_string(spriteEntity.getPosition().y) + ")");
			sf::Text IDText(ID, m_defaultFont, 20);
			IDText.setPosition(
				spriteEntity.getPosition().x + spriteEntity.getGlobalBounds().getSize().x,
				spriteEntity.getPosition().y
			);
			sceneRenderTexture.rd.draw(IDText);
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

void Editor::displayWayPointCanvas(const entt::entity& entityID, ComponentPropData& cmpntData)
{
	ImGui::SetNextWindowDockID(m_dockspaceId3, ImGuiCond_Once);
	ImGui::SetNextWindowBgAlpha(0.4f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
	ImGui::Begin(("WayPoint Editor | " + cmpntData.name).c_str(), &cmpntData.isWaypointEditorOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();

	// Center the image in the window content area
	ImVec2 region = ImGui::GetContentRegionAvail();
	ImVec2 canvasSize = {
		m_gameView.getTexture()->getSize().x * m_sceneDrawScaleX,
		m_gameView.getTexture()->getSize().y * m_sceneDrawScaleY
	};

	// Same offset as scene
	ImVec2 canvasPosTL = m_sceneDrawOffset;
	ImVec2 canvasPosBR = { canvasPosTL.x + canvasSize.x, canvasPosTL.y + canvasSize.y };

	// Draw border and background color
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Canvas is a large button that will used to catch mouse interactions
	ImGui::InvisibleButton("canvas", canvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool isHovered = ImGui::IsItemHovered();
	const bool isActive = ImGui::IsItemActive();

	const ImVec2 origin(
		canvasPosTL.x + cmpntData.scrolling.x,
		canvasPosTL.y + cmpntData.scrolling.y
	);

	const ImVec2 mousePosInCanvas(
		(io.MousePos.x - origin.x) / (cmpntData.zoom * m_sceneDrawScaleX),
		(io.MousePos.y - origin.y) / (cmpntData.zoom * m_sceneDrawScaleY)
	);

	// Adjust camera scrolling using change in mouse position in canvas
	const float mouseThresholdForPan = -1.0f;
	if (isActive && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouseThresholdForPan))
	{
		cmpntData.scrolling.x += io.MouseDelta.x;
		cmpntData.scrolling.y += io.MouseDelta.y;
	}

	// Context menu (under default mouse threshold)
	ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

	if (dragDelta.x == 0.0f && dragDelta.y == 0.0f)
		ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

	if (ImGui::BeginPopup("context"))
	{
		if (ImGui::MenuItem("Remove one", nullptr, false, cmpntData.points.Size > 0))
		{
			cmpntData.points.resize(cmpntData.points.size() - 1);
			updateWayPointComponent(entityID, cmpntData);
		}

		if (ImGui::MenuItem("Remove all", nullptr, false, cmpntData.points.Size > 0))
		{
			cmpntData.points.clear();
			updateWayPointComponent(entityID, cmpntData);
		}

		if (ImGui::MenuItem("Reset Camera", nullptr, false))
		{
			cmpntData.scrolling = { 0.f, 0.f };
			cmpntData.zoom = 1.0f;
		}

		ImGui::EndPopup();
	}

	if (isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		cmpntData.points.push_back(mousePosInCanvas);
		updateWayPointComponent(entityID, cmpntData);
	}

	if (isHovered && io.MouseWheel != 0)
	{
		const float zoomSpeed = 0.1f;
		cmpntData.zoom += io.MouseWheel * zoomSpeed;

		// Clamp zoom to prevent flipping or disappearing
		cmpntData.zoom = ImClamp(cmpntData.zoom, 0.1f, 10.0f);
	}

	// Draw grid + all lines in the canvas
	drawList->PushClipRect(canvasPosTL, canvasPosBR, true);

	const float gridStep = 32.0f;
	for (float x = fmodf(cmpntData.scrolling.x * cmpntData.zoom, gridStep * cmpntData.zoom); x < canvasSize.x; x += gridStep * cmpntData.zoom)
	{
		drawList->AddLine(
			{ canvasPosTL.x + x, canvasPosTL.y },
			{ canvasPosTL.x + x, canvasPosBR.y },
			IM_COL32(200, 200, 200, 40));
	}

	for (float y = fmodf(cmpntData.scrolling.y * cmpntData.zoom, gridStep * cmpntData.zoom); y < canvasSize.y; y += gridStep * cmpntData.zoom)
	{
		drawList->AddLine(
			{ canvasPosTL.x, canvasPosTL.y + y },
			{ canvasPosBR.x, canvasPosTL.y + y },
			IM_COL32(200, 200, 200, 40));
	}

	for (int i = 0; i < cmpntData.points.Size; i++)
	{
		// Draw a line to next point, but only if n < Size-1
		if (i < cmpntData.points.Size - 1)
		{
			drawList->AddLine(
				{
					origin.x + cmpntData.points[i].x * m_sceneDrawScaleX * cmpntData.zoom,
					origin.y + cmpntData.points[i].y * m_sceneDrawScaleY * cmpntData.zoom
				},
				{
					origin.x + cmpntData.points[i + 1].x * m_sceneDrawScaleX * cmpntData.zoom,
					origin.y + cmpntData.points[i + 1].y * m_sceneDrawScaleY * cmpntData.zoom
				},
				IM_COL32(255, 255, 0, 255), 2.0f
			);
		}

		// Set the draw cursor for rendering the label next to the point 
		ImVec2 labelPos = {
			origin.x + cmpntData.points[i].x * m_sceneDrawScaleX * cmpntData.zoom,
			origin.y + cmpntData.points[i].y * m_sceneDrawScaleY * cmpntData.zoom
		};

		ImGui::SetCursorScreenPos(labelPos);

		ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f); // Default white
		if (i == 0) // First waypoint will always be green
			color = ImVec4(0.f, 1.f, 0.f, 1.f);
		else if (i == cmpntData.points.Size - 1) // Last waypoint will alaywas be red
			color = ImVec4(1.f, 0.f, 0.f, 1.f);

		ImGui::TextColored(color, "(%.2f, %.2f)", cmpntData.points[i].x, cmpntData.points[i].y);
	}

	drawList->PopClipRect();

	ImGui::SetCursorScreenPos(canvasPosTL);
	ImGui::Text(" Mouse Left: click to add waypoints, Mouse Right: drag to pan, click for context menu.");
	ImGui::SameLine();

	ImGui::End();
}

void Editor::updateWayPointCanvas(const entt::entity& entityID, ComponentPropData& cmpntData)
{
	if (m_reg->all_of<MovementPattern, Sprite>(entityID))
	{
		auto& movement = m_reg->get<MovementPattern>(entityID);

		WayPoint* current = movement.movePattern;

		if (!current) return;

		cmpntData.points.clear();
		cmpntData.points.push_back({ current->coordinate.x, current->coordinate.y });

		while (current->nextWP != nullptr)
		{
			current = current->nextWP;
			cmpntData.points.push_back({ current->coordinate.x, current->coordinate.y });
		}
	}
}

void Editor::updateWayPointComponent(const entt::entity& entityID, ComponentPropData& cmpntData, bool reset)
{
	if (m_reg->all_of<MovementPattern, Sprite>(entityID))
	{
		auto& movement = m_reg->get<MovementPattern>(entityID);
		auto& sprite = m_reg->get<Sprite>(entityID);

		movement.distance = 0.f;

		// Clear waypoint data
		// Move sprite to the newest waypoint
		// Add waypoint to MovementPattern
	}
}
