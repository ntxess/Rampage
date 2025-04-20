#pragma once

#include "manager/ConfigKey.hpp"
#include "manager/ResourceManager.hpp"
#include "manager/SaveManager.hpp"
#include "manager/SceneManager.hpp"
#include "entt/core/hashed_string.hpp"
#include "entt/entity/entity.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include <any>
#include <memory>
#include <unordered_map>

typedef entt::hashed_string::hash_type HashKey;
typedef std::unordered_map<HashKey, std::unique_ptr<std::unordered_map<std::string, std::any>>> SaveData;
typedef std::unordered_map<ConfigKey, std::any> ConfigData;

// Global System Data 
struct GlobalData
{
	// Application window data
	sf::Clock							clock;
	sf::RenderWindow					window;
	sf::View							viewport;
	float								aspectRatio;
	float								deltaTime;
	ConfigData							configData;
	SaveData							saveData;
	SaveManager							saveManager;
	ResourceManager<sf::Texture>		textureManager;
	ResourceManager<sf::SoundBuffer>	audioManager;
	SceneManager                        sceneManager;

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