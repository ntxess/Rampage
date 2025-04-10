#include "WayPoint.hpp"

WayPoint::WayPoint()
	: coordinate(sf::Vector2f(0.f, 0.f))
	, distanceToNext(0.f)
	, distanceTotal(0.f)
	, nextWP(nullptr)
{}

WayPoint::WayPoint(sf::Vector2f coordinate)
	: coordinate(coordinate)
	, distanceToNext(0.f)
	, distanceTotal(0.f)
	, nextWP(nullptr)
{}

WayPoint* WayPoint::link(WayPoint* waypoint)
{
	nextWP = waypoint;

	distanceToNext = sqrt(
		(nextWP->coordinate.x - coordinate.x) * 
		(nextWP->coordinate.x - coordinate.x) +
		(nextWP->coordinate.y - coordinate.y) *
		(nextWP->coordinate.y - coordinate.y)
	);

	return nextWP;
}
