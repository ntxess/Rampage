#pragma once

#include "CommonEnum.hpp"
#include "../manager/ConfigManager.hpp"
#include "../manager/SceneManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>

// Global System Data 
struct GlobalData
{
	ConfigManager    configManager;
	ConfigMap	     configuration;

	SceneManager     sceneManager;


	sf::RenderWindow window;
	sf::View		 viewport;
	sf::Clock		 clock;
	float			 aspectRatio;
	float			 deltaTime;
};
