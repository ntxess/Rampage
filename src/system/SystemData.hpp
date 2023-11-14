#pragma once

#include "CommonEnum.hpp"
#include "FileManager.hpp"
#include "SceneManager.hpp"
#include "SpriteManager.hpp"
#include "WaypointManager.hpp"
#include "Logger.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

// Global System Data 
struct SystemData
{
	//FileManager      fileManager;
	//SceneManager     sceneManager;
	//SpriteManager    spriteManager;
	//WaypointManager  waypointManager;
	//Logger			 logger;

	sf::RenderWindow window;
	sf::View		 viewport;
	sf::Clock		 clock;
	float			 aspectRatio;
	float			 deltaTime;
	ID				 entityId;
	DataMap			 configuration;
};

