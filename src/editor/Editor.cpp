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
    ImGui::GetIO().ConfigDockingAlwaysTabBar = true; // ImGUI bug: Setting this true, will prevent you from using SetNextWindowSizeConstraints

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
    if (m_startButtonEnabled)
        m_sceneMap[m_selectedSceneKey]->scene->processInput();
}

void Editor::update()
{
    if (m_startButtonEnabled)
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

void Editor::setupDockspace()
{
    m_dockspaceId1 = ImGui::GetID("Dockspace1");
    m_dockspaceId2 = ImGui::GetID("Dockspace2");
    m_dockspaceId3 = ImGui::GetID("Dockspace3");
    m_dockspaceId4 = ImGui::GetID("Dockspace4");
    m_dockspaceId5 = ImGui::GetID("Dockspace5");

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);
    const float fifthWidth = width / 5.f;
    const float halfHeight = height / 2.f;
    const float scalingWidth = width / m_data->aspectRatio;
    const float scalingHeight = height / m_data->aspectRatio;
    const float scalingFifthWidth = fifthWidth + scalingWidth;

    setupDockPanel({ 0, 0 }, { fifthWidth, halfHeight }, "##LP1", m_dockspaceId1);
    setupDockPanel({ 0, halfHeight }, { fifthWidth, halfHeight }, "##LP2", m_dockspaceId2);
    setupDockPanel({ fifthWidth, 0 }, { scalingWidth, scalingHeight }, "##MP1", m_dockspaceId3);
    setupDockPanel({ fifthWidth, scalingHeight }, { scalingWidth, height - scalingHeight }, "##MP2", m_dockspaceId4);
    setupDockPanel({ scalingFifthWidth, 0 }, { width - scalingFifthWidth, height }, "##RP1", m_dockspaceId5);

    ImGui::PopStyleVar(3);
}

void Editor::renderDebugPanel()
{
    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);

    ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ width / 5, height / 2 }, ImGuiCond_Once);
    ImGui::Begin("Debug Panel", NULL, 0);

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

            const float len = 64 * (ImGui::GetWindowWidth() / ImGui::GetWindowHeight());
            if (m_startButtonEnabled)
            {
                if (ImGui::Button("Pause", { len, len }))
                {
                    m_startButtonEnabled = false;
                }
            }
            else
            {
                if (ImGui::Button("Play", { len, len }))
                {
                    m_startButtonEnabled = true;
                }
            } 
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
    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);

    ImGui::SetNextWindowDockID(m_dockspaceId1, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ width / 5, height / 2 }, ImGuiCond_Once);
    ImGui::Begin("Performance Panel", NULL, 0);

    ImGui::End();
}

void Editor::renderLogViewPanel()
{
    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);

    ImGui::SetNextWindowDockID(m_dockspaceId2, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ 0, height / 2 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ width / 5, height / 2 }, ImGuiCond_Once);
    ImGui::Begin("Log View Panel", NULL, 0);

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
    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);
    const float aspectRatio = 16.0f / 9.0f;
    ImGui::GetIO().ConfigDockingAlwaysTabBar = false;
    ImGui::SetNextWindowDockID(m_dockspaceId3, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ width / 5, 0 }, ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints
    (
        { width / (m_data->aspectRatio), height / m_data->aspectRatio },
        { FLT_MAX, FLT_MAX },
        constrainedByAspectRatio,
        (void*)&aspectRatio
    );
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::Begin("Scene View Panel", NULL, 0);
    ImGui::PopStyleVar();

    m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd.clear();

    // Call to the actual scenes render function
    m_sceneMap[m_selectedSceneKey]->scene->render();

    displayEntityVisualizers();
    displayCollisionSystemVisualizer();

    sf::Sprite gameView;
    gameView.setTexture(m_reg->get<SceneViewRenderer>(m_sceneMap[m_selectedSceneKey]->renderTextureID).rd.getTexture());
    gameView.setScale(ImGui::GetWindowWidth() / width, (ImGui::GetWindowHeight() - ImGui::GetFrameHeight()) / height);
    ImGui::Image(gameView);

    ImGui::End();
    ImGui::GetIO().ConfigDockingAlwaysTabBar = true;
}

void Editor::renderFileExplorerPanel()
{
    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);
    ImGui::SetNextWindowDockID(m_dockspaceId4, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ (width / 5), height / (m_data->aspectRatio) }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ width / (m_data->aspectRatio), height - (height / (m_data->aspectRatio)) }, ImGuiCond_Once);
    ImGui::Begin("FileExplorer Panel", NULL, 0);


    ImGui::End();
}

void Editor::renderPropertiesPanel()
{
    const float width = static_cast<float>(m_data->window.getSize().x);
    const float height = static_cast<float>(m_data->window.getSize().y);
    ImGui::SetNextWindowDockID(m_dockspaceId5, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ width / 5 + (width / (m_data->aspectRatio)), 0 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize({ width - (width / 5 + (width / (m_data->aspectRatio))), height }, ImGuiCond_Once);
    ImGui::Begin("Properties Panel", NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    static std::unordered_map<entt::entity, bool> closableGroups;
    static std::unordered_map<entt::entity, std::array<bool, 6>> closableComponents;

    const auto& view = m_reg->view<Sprite>();
    for (const auto& entityID : view)
    {
        if (!closableGroups.count(entityID) || !closableComponents.count(entityID))
        {
            closableGroups[entityID] = true;
            closableComponents[entityID].fill(true);
        }

        std::string ID = "Entity " + std::to_string(static_cast<unsigned int>(entityID));
        if (ImGui::CollapsingHeader(ID.c_str(), &closableGroups[entityID]))
        {
            ImGui::BeginChild(("##EntityColm" + ID).c_str(), { ImGui::GetWindowWidth() - 26.f, 0.f }, ImGuiChildFlags_AutoResizeY);

            if (m_reg->all_of<Sprite>(entityID) && ImGui::CollapsingHeader(("Sprite##Header" + ID).c_str(), &closableComponents[entityID][0], ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& sprite = m_reg->get<Sprite>(entityID);
                sprite.accept(&m_componentVisitor, entityID);
            }
            if (!closableComponents[entityID][0])
            {
                // TODO: Remove the sprite component from QuadTree
                m_reg->remove<Sprite>(entityID);
                static_cast<Sandbox*>(m_sceneMap[m_selectedSceneKey]->scene.get())->getSystemManager()->getSystem<CollisionSystem>()->remove(*m_reg, entityID);
            }

            // Unstable
            //if (m_reg->all_of<UpdateEntityEvent>(entityID) && ImGui::CollapsingHeader(("UpdateEntityEvent##Header" + ID).c_str(), &closableComponents[entityID][1], ImGuiTreeNodeFlags_DefaultOpen))
            //{
            //    auto& updateEntityEvent = m_reg->get<UpdateEntityEvent>(entityID);
            //    updateEntityEvent.accept(&m_componentVisitor, entityID);
            //}
            //if (!closableComponents[entityID][1])
            //{
            //    m_reg->remove<UpdateEntityEvent>(entityID);
            //}

            if (m_reg->all_of<UpdateEntityPolling>(entityID) && ImGui::CollapsingHeader(("UpdateEntityPolling##Header" + ID).c_str(), &closableComponents[entityID][2], ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& updateEntityPolling = m_reg->get<UpdateEntityPolling>(entityID);
                updateEntityPolling.accept(&m_componentVisitor, entityID);
            }
            if (!closableComponents[entityID][2])
            {
                m_reg->remove<UpdateEntityPolling>(entityID);
            }

            if (m_reg->all_of<EntityStatus>(entityID) && ImGui::CollapsingHeader(("EntityStatus##Header" + ID).c_str(), &closableComponents[entityID][3], ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& entityStatus = m_reg->get<EntityStatus>(entityID);
                entityStatus.accept(&m_componentVisitor, entityID);
            }
            if (!closableComponents[entityID][3])
            {
                m_reg->remove<EntityStatus>(entityID);
            }

            if (m_reg->all_of<EffectsList>(entityID) && ImGui::CollapsingHeader(("EffectsList##Header" + ID).c_str(), &closableComponents[entityID][4], ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& effectsList = m_reg->get<EffectsList>(entityID);
                effectsList.accept(&m_componentVisitor, entityID);
            }
            if (!closableComponents[entityID][4])
            {
                m_reg->remove<EffectsList>(entityID);
            }

            if (m_reg->all_of<MovementPattern>(entityID) && ImGui::CollapsingHeader(("MovementPattern##Header" + ID).c_str(), &closableComponents[entityID][5], ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& movementPattern = m_reg->get<MovementPattern>(entityID);
                movementPattern.accept(&m_componentVisitor, entityID);
            }
            if (!closableComponents[entityID][5])
            {
                m_reg->remove<MovementPattern>(entityID);
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

void Editor::setupDockPanel(const ImVec2& panPos, const ImVec2& panSize, const char* panID, const ImGuiID& dockID)
{
    ImGui::SetNextWindowPos(panPos);
    ImGui::SetNextWindowSize(panSize);
    ImGui::Begin(panID, NULL, m_panelFlags);
    ImGui::DockSpace(dockID, panSize, ImGuiDockNodeFlags_NoResize);
    ImGui::End();
}
