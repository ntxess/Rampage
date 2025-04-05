#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"
#include <SFML/Graphics/RenderTexture.hpp>

class SceneViewRenderer : public IComponent
{
public:
    SceneViewRenderer() = delete;
    SceneViewRenderer(unsigned int width, unsigned int height, const sf::ContextSettings& settings);

    void accept(IComponentVisitor* visitor, entt::entity entityID) override;

    sf::RenderTexture rd;
};