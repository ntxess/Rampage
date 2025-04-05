#pragma once

#include "IComponent.hpp"
#include "../common/IComponentVisitor.hpp"
#include <SFML/Graphics/VertexArray.hpp>

class Hitbox : public IComponent
{
public:
    Hitbox() = delete;
    Hitbox(const sf::VertexArray& verticies);
    Hitbox(const sf::PrimitiveType type, const std::size_t vertexCount = 0);

    void accept(IComponentVisitor* visitor, entt::entity entityID) override;

    inline sf::FloatRect getBounds() const
    {
        return hitbox.getBounds();
    }

    inline bool contains(const float x, const float y) const
    {
        return hitbox.getBounds().contains(x, y);
    }

    inline bool contains(const sf::Vector2<float>& point) const
    {
        return hitbox.getBounds().contains(point);
    }

    inline bool intersects(const sf::Rect<float>& rectangle) const
    {
        return hitbox.getBounds().intersects(rectangle);
    }

    inline bool intersects(const sf::Rect<float>& rectangle, sf::Rect<float>& intersection) const
    {
        return hitbox.getBounds().intersects(rectangle, intersection);
    }

    inline sf::Vector2f getPosition() const
    {
        return hitbox.getBounds().getPosition();
    }

    sf::VertexArray hitbox;
};