#pragma once

#include <entt/entt.hpp>
#include <any>
#include <atomic>
#include <string>
#include <unordered_map>

/**
 * @brief System Status codes.
*/
enum class SystemStatus
{
	OK,
	ERROR,
	FILE_MNGR_SUCCESS,
	FILE_MNGR_CREATED_FILE,
	FILE_MNGR_FAIL_WRITE,
	FILE_MNGR_FAIL_READ,
	FILE_MNGR_FAIL_FILE_EMPTY,
};

/**
 * @brief Enum key for configurations values.
*/
enum ConfigKey
{
	NAME,
	VERSION,
	PATH_CONFIG,
	SAVE_FOLDER,
	SCRIPT_FOLDER,
	WIDTH,
	HEIGHT,
	FRAMERATE,
	FULLSCREEN,
	DMG_NUMBER,
	VISUAL_EFFECTS,
	SCREEN_SHAKE,
	MATURE,
	MUSIC_VOL,
	EFFECTS_VOL,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_JUMP,
	KEY_EXIT,
	KEY_ENTER,
	KEY_BACK,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	DUB,
	SUB,
	SUBTITLE,
	COLOR_BLIND
};

/**
 * @brief [FOR DEBUG] SystemStatus enum to string for debugging.
 * @param [IN] status 
 * @return SystemStatus in std::string.
*/
inline const std::string ssToStr(SystemStatus status)
{
	switch (status)
	{
	case SystemStatus::OK:
		return "OK";
	case SystemStatus::ERROR:
		return "ERROR";
	case SystemStatus::FILE_MNGR_SUCCESS:
		return "FILE_MNGR_SUCCESS";
	case SystemStatus::FILE_MNGR_CREATED_FILE:
		return "FILE_MNGR_CREATED_FILE";
	case SystemStatus::FILE_MNGR_FAIL_WRITE:
		return "FILE_MNGR_FAIL_WRITE";
	case SystemStatus::FILE_MNGR_FAIL_READ:
		return "FILE_MNGR_FAIL_READ";
	default:
		return "DEFAULT";
	}
}

/**
 * @brief [FOR DEBUG] Converts std::any value to std::string.
 * @param [IN] val 
 * @return std::any in std::string
*/
inline std::string anyToString(const std::any& val)
{
	if (auto x = std::any_cast<std::string>(&val))
		return *x;
	if (auto x = std::any_cast<const char*>(&val))
		return *x;
	else if (auto x = std::any_cast<bool>(&val))
		return (std::to_string(*x) == "1") ? "true" : "false";
	else if (auto x = std::any_cast<int>(&val))
		return std::to_string(*x);
	else if (auto x = std::any_cast<float>(&val))
		return std::to_string(*x);
	else if (auto x = std::any_cast<double>(&val))
		return std::to_string(*x);
	return "";
}

typedef entt::hashed_string::hash_type HashKey;
typedef std::unordered_map<ConfigKey, std::any> ConfigData;
typedef std::unordered_map<HashKey, std::any> DataMap;
typedef std::unordered_map<HashKey, DataMap*> SaveData;