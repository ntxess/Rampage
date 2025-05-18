#include "EditorSceneData.hpp"

EditorSceneData::EditorSceneData(std::unique_ptr<IScene> scn, unsigned int width, unsigned int height, const sf::ContextSettings& settings)
    : m_scene(std::move(scn))
{
    m_scene->init();
    m_renderTextureID = m_scene->getRegistry().create();
    m_scene->getRegistry().emplace<SceneViewRenderer>(m_renderTextureID, width, height, settings);
}

void EditorSceneData::processInput()
{    
    m_scene->processInput();
}

void EditorSceneData::processEvent(const sf::Event& event)
{
    m_scene->processEvent(event);
}

void EditorSceneData::render()
{
    m_scene->render();
}

void EditorSceneData::update()
{
    m_scene->update();
}

entt::registry& EditorSceneData::getRegistry() const
{
    return m_scene->getRegistry();
}

sf::RenderTexture& EditorSceneData::getRenderTexture() const
{
    return m_scene->getRegistry().get<SceneViewRenderer>(m_renderTextureID).rd;
}

IScene* EditorSceneData::get() const
{
    return m_scene.get();
}

