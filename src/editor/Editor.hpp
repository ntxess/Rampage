#pragma once

#include "EditorSceneVisitor.hpp"
#include "EditorComponentVisitor.hpp"
#include "../core/Components.hpp"
#include "../core/GlobalData.hpp"
#include "../core/Systems.hpp"
#include "../core/interface/IScene.hpp"
#include "../core/util/Logger.hpp"
#include "../scene/Scenes.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "entt/entity/entity.hpp"
#include "entt/entity/registry.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include <memory>

struct SceneData
{
    std::unique_ptr<IScene> scene;
    entt::entity renderTextureID;

    SceneData(std::unique_ptr<IScene> scn, unsigned int width, unsigned int height, const sf::ContextSettings& settings) : scene(std::move(scn))
    {
        scene->init();
        renderTextureID = scene->getRegistry().create();
        scene->getRegistry().emplace<SceneViewRenderer>(renderTextureID, width, height, settings);
    }
};

class Editor : public IScene
{
public:
    Editor();
    Editor(GlobalData* sysData);
    ~Editor();

    void init() override;
    void processEvent(const sf::Event& event) override;
    void processInput() override;
    void update() override;
    void render() override;
    void pause() override;
    void resume() override;
    void addData(GlobalData* data) override;
    void accept(ISceneVisitor* visitor) override;
    entt::registry& getRegistry() override;

private:
    void renderDebugPanel(const ImVec2& pos, const ImVec2& size);
    void renderPerformancePanel(const ImVec2& pos, const ImVec2& size);
    void renderLogViewPanel(const ImVec2& pos, const ImVec2& size);
    void renderSceneViewPanel(const ImVec2& pos, const ImVec2& size);
    void renderFileExplorerPanel(const ImVec2& pos, const ImVec2& size);
    void renderPropertiesPanel(const ImVec2& pos, const ImVec2& size);
    void displayEntityVisualizers();
    void displayCollisionSystemVisualizer();

    void setupDockPanel(const ImVec2& panPos, const ImVec2& panSize, const char* panID, const ImGuiID& dockID);

    template<typename... Args>
    entt::entity findEntityID();

private:
    GlobalData* m_data;

    sf::Font m_defaultFont;
    ImGuiWindowFlags m_panelFlags;

    ImGuiID m_dockspaceId1;
    ImGuiID m_dockspaceId2;
    ImGuiID m_dockspaceId3;
    ImGuiID m_dockspaceId4;
    ImGuiID m_dockspaceId5;

    bool m_enableEntityID;
    bool m_enableEntityCollider;
    bool m_enableEntityHeading;
    bool m_enableQuadTreeVisualizer;
    std::atomic<bool> m_startButtonEnabled;
    int m_totalEntity;

    // Current loaded scene data
    std::unordered_map<std::string, std::unique_ptr<SceneData>> m_sceneMap;
    std::string m_selectedSceneKey;
    entt::registry* m_reg;

    EditorComponentVisitor m_componentVisitor;
    EditorSceneVisitor m_sceneVisitor;
};

template<typename... Args>
inline entt::entity Editor::findEntityID()
{
    // Hacky way of getting entity ID from a unique component
    const auto& view = m_reg->view<Args...>();
    for (const auto& entityID : view)
    {
        return entityID;
    }

    return entt::null;
}