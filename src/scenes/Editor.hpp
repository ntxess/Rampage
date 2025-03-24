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

private:
	GlobalData* m_data;

	sf::Font m_defaultFont;
	ImGuiWindowFlags m_panelFlags;
	ImGuiWindowFlags m_expandablePanelFlags;

	entt::entity m_sceneViewTextureID;
	std::unique_ptr<IScene> m_game;
	entt::registry* m_reg;

	ImGuiID m_dockspaceId1;
	ImGuiID m_dockspaceId2;
	ImGuiID m_dockspaceId3;
	ImGuiID m_dockspaceId4;
	ImGuiID m_dockspaceId5;
};

