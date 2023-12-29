#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

struct Sprite
{
	sf::Sprite sprite;

	Sprite(sf::Texture& texture)
	{
		sprite.setTexture(texture);
	}
	Sprite(sf::Texture& texture, int width, int height, bool repeat)
	{
		texture.setRepeated(repeat);
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}
};