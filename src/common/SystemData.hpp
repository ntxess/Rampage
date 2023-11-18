#pragma once

#include "CommonEnum.hpp"
#include "../manager/FileManager.hpp"
#include "../manager/SceneManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

// Global System Data 
struct SystemData
{
	FileManager      fileManager;
	SceneManager     sceneManager;
	sf::RenderWindow window;
	sf::View		 viewport;
	sf::Clock		 clock;
	float			 aspectRatio;
	float			 deltaTime;
	ID				 entityId;
	DataMap			 configuration;
};
