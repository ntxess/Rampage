#include "MainMenu.hpp"

MainMenu::MainMenu()
    : m_data(nullptr)
    , m_wallpaper(entt::null)
{}

MainMenu::MainMenu(GlobalData* sysData)
    : m_data(sysData)
    , m_wallpaper(entt::null)
{}

void MainMenu::init()
{
    std::unordered_map<std::string, std::any> texturePath;
    m_data->saveManager.load("config/texture.json", texturePath);     // Load config file
    m_data->textureManager.load(texturePath, thor::Resources::Reuse); // Load the texture from loaded paths

    m_wallpaper = m_reg.create();
    m_reg.emplace<Sprite>(m_wallpaper, m_data->textureManager["bg"]);
}

void MainMenu::processEvent(const sf::Event& event)
{}

void MainMenu::processInput()
{}

void MainMenu::update()
{}

void MainMenu::render()
{
    const auto& scrView = m_reg.view<SceneViewRenderer>();
    for (const auto& sceneTextureID : scrView)
    {
        auto& sceneRenderTexture = m_reg.get<SceneViewRenderer>(sceneTextureID).rd;
        auto& spriteEntity = m_reg.get<Sprite>(m_wallpaper).sprite;
        sceneRenderTexture.draw(spriteEntity);
    }
}

void MainMenu::pause()
{}

void MainMenu::resume()
{}

void MainMenu::addData(GlobalData* data)
{
    m_data = data;
}

void MainMenu::accept(ISceneVisitor* visitor)
{
    visitor->visit(this);
}

entt::registry& MainMenu::getRegistry()
{
    return m_reg;
}