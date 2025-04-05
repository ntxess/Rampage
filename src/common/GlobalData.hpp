#pragma once

#include "CommonEnum.hpp"
#include "../managers/SaveManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/ResourceManager.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

// Global System Data 
struct GlobalData
{
    // Application window data
    sf::RenderWindow					window;
    sf::View							viewport;
    sf::Clock							clock;
    float								aspectRatio;
    float								deltaTime;
    ConfigData							configData;
    SaveData							saveData;
    SaveManager							saveManager;
    SceneManager						sceneManager;
    ResourceManager<sf::Texture>		textureManager;
    ResourceManager<sf::SoundBuffer>	audioManager;

    // Helper functions for converting std::any to underlining data type
    template<typename T>
    T Configuration(ConfigKey key);

    template<typename T>
    T Data(HashKey group, HashKey key);
};

template<typename T>
T GlobalData::Configuration(ConfigKey key)
{
    std::any& val = configData[key];
    return (val.type() == typeid(T)) ? std::any_cast<T>(val) : T();
}

template<typename T>
T GlobalData::Data(HashKey group, HashKey key)
{
    std::any& val = saveData[group][key];
    return (val.type() == typeid(T)) ? std::any_cast<T>(val) : T();
}