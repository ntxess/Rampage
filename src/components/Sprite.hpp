#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

struct Sprite
{
	sf::Sprite sprite;

	Sprite(sf::Texture& texture, bool repeat = false)
	{
		texture.setRepeated(repeat);
		sprite.setTexture(texture);
	}

	Sprite(sf::Texture& texture, int width, int height, bool repeat = false)
	{
		texture.setRepeated(repeat);
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, width, height));
	}

	inline const sf::Vector2f& getOrigin() const
	{
		return sprite.getOrigin();
	}

	inline const sf::Vector2f& getPosition() const
	{
		return sprite.getPosition();
	}

	inline float getRotation() const
	{
		return sprite.getRotation();
	}

	inline const sf::Vector2f& getScale() const
	{
		return sprite.getScale();
	}

	inline void move(float offsetX, float offsetY)
	{
		sprite.move(offsetX, offsetY);
	}

	inline void rotate(float angle)
	{
		sprite.rotate(angle);
	}

	inline void scale(float factorX, float factorY)
	{
		sprite.scale(factorX, factorY);
	}

	inline void setOrigin(float x, float y)
	{
		sprite.setOrigin(x, y);
	}

	inline void setPosition(float x, float y)
	{
		sprite.setPosition(x, y);
	}

	inline void setRotation(float angle)
	{
		sprite.setRotation(angle);
	}

	inline void setScale(float factorX, float factorY)
	{
		sprite.setScale(factorX, factorY);
	}

	inline void setTexture(const sf::Texture& texture, bool resetRect = false)
	{
		sprite.setTexture(texture, resetRect);
	}

	inline void setTextureRect(const sf::IntRect& rectangle)
	{
		sprite.setTextureRect(rectangle);
	}
};