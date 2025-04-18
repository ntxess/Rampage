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
    , m_enableQuadTreeVisualizer(false)
    , m_startButtonEnabled(true)
    , m_totalEntity(0)
    , m_reg(nullptr)
{}

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
    , m_enableQuadTreeVisualizer(false)
    , m_startButtonEnabled(true)
    , m_totalEntity(0)
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

    // Disable flags for the dockspace panels; Make panel non-interactive 
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

    // Enable and setup dockspaces
    ImGui::SFML::Init(m_data->window);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    // ImGUI bug: Setting this true, will prevent you from using SetNextWindowSizeConstraints
    // which is needed for ratio-resizing of the scene view rendering panel
    ImGui::GetIO().ConfigDockingAlwaysTabBar = true;

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
    if (m_startButtonEnabled)
    {
        m_sceneMap[m_selectedSceneKey]->scene->update();
    }
    else if (m_forwardFrameEnabled)
    {
        m_sceneMap[m_selectedSceneKey]->scene->update();
        m_forwardFrameEnabled = false;
    }
}

void Editor::render()
{
    ImGui::SFML::Update(m_data->window, sf::seconds(m_data->deltaTime));

    m_dockspaceId1 = ImGui::GetID("Dockspace1");
    m_dockspaceId2 = ImGui::GetID("Dockspace2");
    m_dockspaceId3 = ImGui::GetID("Dockspace3");
    m_dockspaceId4 = ImGui::GetID("Dockspace4");
    m_dockspaceId5 = ImGui::GetID("Dockspace5");

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });

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
{}

void Editor::resume()
{}

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

void Editor::renderDebugPanel(const ImVec2& pos, const ImVec2& size)
{
    ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);
    ImGui::Begin("Debug Panel", NULL, 0);

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
    ImGui::Begin("Performance Panel", NULL, 0);

    ImGui::End();
}

void Editor::renderLogViewPanel(const ImVec2& pos, const ImVec2& size)
{
    ImGui::SetNextWindowDockID(m_dockspaceId2, ImGuiCond_Once);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);
    ImGui::Begin("Log View Panel", NULL, 0);

    ImGui::End();
}

void Editor::renderSceneViewPanel(const ImVec2& pos, const ImVec2& size)
{
    ImGui::SetNextWindowDockID(m_dockspaceId3, ImGuiCond_Once);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
    ImGui::Begin("Scene View Panel", NULL, 0);
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
    ImGui::Begin("FileExplorer Panel", NULL, 0);

    ImGui::End();
}

void Editor::renderPropertiesPanel(const ImVec2& pos, const ImVec2& size)
{
    ImGui::SetNextWindowDockID(m_dockspaceId5, ImGuiCond_Once);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
    ImGui::SetNextWindowSize(size, ImGuiCond_Once);
    ImGui::Begin("Properties Panel", NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    static std::unordered_map<entt::entity, bool> closableGroups;
    static std::unordered_map<entt::entity, ComponentPropData> entities;

    const auto& view = m_reg->view<Sprite>();
    for (const auto& entityID : view)
    {
        // Initialize the closable groups and components if it is a new entityID
        if (!closableGroups.count(entityID) || !entities.count(entityID))
        {
            closableGroups[entityID] = true;
            entities.emplace(entityID, ComponentPropData(entityID));
        }

        //std::string ID = "Entity " + std::to_string(static_cast<unsigned int>(entityID));
        if (ImGui::CollapsingHeader(entities[entityID].name.c_str(), &closableGroups[entityID]))
        {
            ImGui::BeginDisabled(m_startButtonEnabled);
            ImGui::BeginChild(("##EntityColm" + entities[entityID].name).c_str(), { ImGui::GetWindowWidth() - 26.f, 0.f }, ImGuiChildFlags_AutoResizeY);

            renderComponentProperties<Sprite>(
                entityID,
                "Sprite##" + entities[entityID].name,
                entities[entityID].closableComponents[0],
                m_componentVisitor
            );

            renderComponentProperties<UpdateEntityPolling>(
                entityID,
                "UpdateEntityPolling##" + entities[entityID].name,
                entities[entityID].closableComponents[1],
                m_componentVisitor
            );

            renderComponentProperties<UpdateEntityEvent>(
                entityID,
                "UpdateEntityEvent##" + entities[entityID].name,
                entities[entityID].closableComponents[2],
                m_componentVisitor
            );

            renderComponentProperties<EntityStatus>(
                entityID,
                "EntityStatus##" + entities[entityID].name,
                entities[entityID].closableComponents[3],
                m_componentVisitor
            );

            renderComponentProperties<EffectsList>(
                entityID,
                "EffectsList##" + entities[entityID].name,
                entities[entityID].closableComponents[4],
                m_componentVisitor
            );

            if (renderComponentProperties<MovementPattern>(
                entityID,
                "MovementPattern##" + entities[entityID].name,
                entities[entityID].closableComponents[5],
                m_componentVisitor
            ))
            {
                if (ImGui::Button(
                    ("Set New Path##PathDesigner" + entities[entityID].name).c_str(), 
                    { ImGui::GetWindowWidth(), 22.f }
                ))
                {
                    entities[entityID].isWaypointEditorOpen = true;
                }
                ImGui::NewLine();

                if (entities[entityID].isWaypointEditorOpen)
                {
                    displayCanvas(entities[entityID]);
                }
            }

            renderComponentProperties<TeamTag>(
                entityID,
                "TeamTag##" + entities[entityID].name,
                entities[entityID].closableComponents[6],
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
            sf::Text IDText(ID, m_defaultFont, 14);
            IDText.setPosition(
                spriteEntity.getPosition().x + spriteEntity.getGlobalBounds().getSize().x,
                spriteEntity.getPosition().y + spriteEntity.getGlobalBounds().getSize().y
            );
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
            border.setRotation(spriteEntity.getRotation());
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

void Editor::displayCanvas(ComponentPropData& cmpntData)
{
    ImGui::Begin(("WayPoint Editor | " + cmpntData.name).c_str(), &cmpntData.isWaypointEditorOpen);

    ImGui::Text("Mouse Left: drag to add lines, Mouse Right: drag to scroll, click for context menu.");

    // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool is_hovered = ImGui::IsItemHovered(); // Hovered
    const bool is_active = ImGui::IsItemActive();   // Held
    const ImVec2 origin(canvas_p0.x + cmpntData.scrolling.x, canvas_p0.y + cmpntData.scrolling.y); // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

    // Add first and second point
    if (is_hovered && !cmpntData.adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        cmpntData.points.push_back(mouse_pos_in_canvas);
        cmpntData.points.push_back(mouse_pos_in_canvas);
        cmpntData.adding_line = true;
    }
    if (cmpntData.adding_line)
    {
        cmpntData.points.back() = mouse_pos_in_canvas;
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
            cmpntData.adding_line = false;
    }

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
    const float mouse_threshold_for_pan = -1.0f;
    if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        cmpntData.scrolling.x += io.MouseDelta.x;
        cmpntData.scrolling.y += io.MouseDelta.y;
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

    if (drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

    if (ImGui::BeginPopup("context"))
    {
        if (cmpntData.adding_line)
            cmpntData.points.resize(cmpntData.points.size() - 2);

        cmpntData.adding_line = false;
        if (ImGui::MenuItem("Remove one", NULL, false, cmpntData.points.Size > 0)) { cmpntData.points.resize(cmpntData.points.size() - 2); }
        if (ImGui::MenuItem("Remove all", NULL, false, cmpntData.points.Size > 0)) { cmpntData.points.clear(); }
        ImGui::EndPopup();
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);

    const float GRID_STEP = 64.0f;
    for (float x = fmodf(cmpntData.scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
        draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
    for (float y = fmodf(cmpntData.scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
        draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));

    for (int n = 0; n < cmpntData.points.Size; n += 2)
        draw_list->AddLine(ImVec2(origin.x + cmpntData.points[n].x, origin.y + cmpntData.points[n].y), ImVec2(origin.x + cmpntData.points[n + 1].x, origin.y + cmpntData.points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
    draw_list->PopClipRect();

    ImGui::End();
}

void Editor::setupDockPanel(const ImVec2& panPos, const ImVec2& panSize, const char* panID, const ImGuiID& dockID) const
{
    ImGui::SetNextWindowPos(panPos);
    ImGui::SetNextWindowSize(panSize);
    ImGui::Begin(panID, NULL, m_panelFlags);
    ImGui::DockSpace(dockID, panSize, ImGuiDockNodeFlags_NoResize);
    ImGui::End();
}