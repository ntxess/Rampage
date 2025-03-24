#pragma once

#include "../scenes/IScene.hpp"
#include "../scenes/Sandbox.hpp"
#include "../common/CommonEnum.hpp"
#include "../common/GlobalData.hpp"
#include "../common/Logger.hpp"
#include <entt/entity/registry.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
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
	Editor(GlobalData* sysData);
	~Editor();

	void init();
	void processEvent(const sf::Event& event);
	void processInput();
	void update();
	void render();
	void pause();
	void resume();
	entt::registry& getRegistry();

private:
	void setupDockspace();
	void renderDebugPanel();
	void renderPerformancePanel();
	void renderLogViewPanel();
	void renderSceneViewPanel();
	void renderFileExplorerPanel();
	void renderPropertiesPanel();
	void displayEntityVisualizers();
	void displayCollisionSystemVisualizer();

	template<typename... Args>
	entt::entity findEntityID();

private:
	GlobalData* m_data;

	sf::Font m_defaultFont;
	ImGuiWindowFlags m_panelFlags;
	ImGuiWindowFlags m_expandablePanelFlags;

	ImGuiID m_dockspaceId1;
	ImGuiID m_dockspaceId2;
	ImGuiID m_dockspaceId3;
	ImGuiID m_dockspaceId4;
	ImGuiID m_dockspaceId5;

	bool m_enableEntityID;
	bool m_enableEntityCollider;
	bool m_enableEntityHeading;
	bool m_enableQuadTreeVisualizer;
	int m_totalEntity;

	// Current loaded scene data
	std::unordered_map<std::string, std::unique_ptr<SceneData>> m_sceneMap;
	std::string m_selectedSceneKey;
	entt::registry* m_reg;
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