#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

struct SceneViewRenderer
{
	sf::RenderTexture rd;

	SceneViewRenderer(unsigned int width, unsigned int height, const sf::ContextSettings& settings)
	{
		rd.create(width, height, settings);
	}
};