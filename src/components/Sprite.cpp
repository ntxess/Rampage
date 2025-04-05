#include "Sprite.hpp"

Sprite::Sprite(sf::Texture& texture, bool repeat)
{
	texture.setRepeated(repeat);
	sprite.setTexture(texture);
}

Sprite::Sprite(sf::Texture& texture, int width, int height, bool repeat)
{
	texture.setRepeated(repeat);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, width, height));
}

void Sprite::accept(IComponentVisitor* visitor, entt::entity entityID)
{
	visitor->visit(this, entityID);
}
