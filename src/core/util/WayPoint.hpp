#pragma once

#include "SFML/Graphics/Vertex.hpp"
#include <cmath>
#include <memory>

class WayPoint
{
public:
	WayPoint();
	WayPoint(sf::Vector2f coordinate);

	WayPoint* link(WayPoint* waypoint);

public:
	const sf::Vector2f coordinate;
	float distanceToNext;
	float distanceTotal;
	WayPoint* nextWP;
};