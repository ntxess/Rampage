#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

struct SceneViewRenderer
{
	SceneViewRenderer(unsigned int width, unsigned int height, const sf::ContextSettings& settings)
	{
		rd.create(width, height, settings);
	}

	sf::RenderTexture rd;
};