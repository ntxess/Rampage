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

class Editor : public IScene
{
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

	struct ComponentPropData
	{
		entt::entity entityID{};
		std::string name;
		std::array<bool, 7> closableComponents{};

		// Waypoint Editor
		bool isWaypointEditorOpen{ false };
		ImVector<ImVec2> points;
		ImVec2 scrolling{ 0.f, 0.f };
		bool adding_line{ false };

		ComponentPropData() = default;
		ComponentPropData(entt::entity entityID)
			: entityID(entityID)
			, name("Entity " + std::to_string(static_cast<unsigned int>(entityID)))
		{
			closableComponents.fill(true);
		}
	};

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
	void displayCanvas(ComponentPropData& cmpntData);
	void setupDockPanel(const ImVec2& panPos, const ImVec2& panSize, const char* panID, const ImGuiID& dockID) const;

	template<typename... Args>
	entt::entity findEntityID();

	template<typename T>
	bool renderComponentProperties(const entt::entity& entityID, std::string_view componentID, bool& visible, EditorComponentVisitor& visitor);

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
	std::atomic<bool> m_forwardFrameEnabled;
	int m_totalEntity;

	// Current loaded scene data
	std::unordered_map<std::string, std::unique_ptr<SceneData>> m_sceneMap;
	std::string m_selectedSceneKey;
	entt::registry* m_reg;

	sf::Sprite m_gameView;

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

template<typename T>
inline bool Editor::renderComponentProperties(const entt::entity& entityID, std::string_view componentID, bool& visible, EditorComponentVisitor& visitor)
{
	if (!m_reg->all_of<T>(entityID))
	{
		return false;
	}
		
	if (ImGui::CollapsingHeader(componentID.data(), &visible, ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto& component = m_reg->get<T>(entityID);
		component.accept(&visitor, entityID);
	}

	if (!visible)
	{
		// TODO: Remove the sprite component from QuadTree
		m_reg->remove<T>(entityID);
		//static_cast<Sandbox*>(m_sceneMap[m_selectedSceneKey]->scene.get())->getSystemManager()->getSystem<CollisionSystem>()->remove(*m_reg, entityID);
		return false;
	}

	return true;
}
