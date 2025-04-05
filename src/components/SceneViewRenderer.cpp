#include "SceneViewRenderer.hpp"

SceneViewRenderer::SceneViewRenderer(unsigned int width, unsigned int height, const sf::ContextSettings& settings)
{
	rd.create(width, height, settings);
}

void SceneViewRenderer::accept(IComponentVisitor* visitor) const
{
	visitor->visit(this);
}
