#pragma once

#include "CommonEnum.hpp"
#include <atomic>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>


// Global System Data 
struct SystemData
{
	typedef std::atomic<size_t>		ID;
	sf::RenderWindow				window;
	sf::View						viewport;
	sf::Clock						clock;
	float							aspectRatio;
	float							deltaTime;
	ID								entityId;
	DataMap							configData;
};

